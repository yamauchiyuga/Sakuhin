#include "Dragon.h"
#include"../Player/Player.h"
#include"../Effect/FireSphere.h"
#include"../../World/IWorld.h"
#include"../../Collision/Field.h"
#include"../../Collision/Line.h"
#include"../AttackCollider.h"
#include"../../Asset.h"

enum {
	MotionBite,
	MotionDead,
	MotionFly,
	MotionFlySpitFireball,
	MotionGoInAir,
	MotionIdle,
	MotionLanding,
	MotionRun,
	MotionSpitFireball,
	MotionTailAttack,
	MotionTurnLeft,
	MotionTurnRight,
	MotionRunAttack
};
//
const float MaxHP{ 400 };
//
const float TurnDistance{ 1.5f };
//
const float RunDistance{ 6.0f };
//
const float RunSpeed{ 0.1f };
//
const float TurnAngle{ 3.0f };
//
const float TurnAroundAngle{ 20.0f };

Dragon::Dragon(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Dragon,Mesh_Dragon, Mesh_Dragon, MotionIdle },
	state_{ State::Idle },
	motion_{ MotionIdle },
	HP_{ MaxHP,GSvector2{250.0f, 650.0f } ,GSvector2{248.0f, 649.0f },750.0f }{
	world_ = world;
	tag_ = "EnemyTag";
	name_ = "Enemy";
	player_ = nullptr;
	collider_ = BoundingSphere{ 2.0f, GSvector3{0.0f, 2.0f, 0.0f} };
	//À•W‚Ì‰Šú‰»
	transform_.position(position);
	mesh_.transform(transform_.localToWorldMatrix());

	mesh_.add_animation_event(MotionBite, 36.0f, [=] {bite(); });
	mesh_.add_animation_event(MotionTailAttack, 45.0f, [=] {tail_attack(); });
	mesh_.add_animation_event(MotionSpitFireball, 23.0f, [=] {spit_fire(); });
}

void Dragon::update(float delta_time) {
	player_ = world_->find_actor("Player");
	//
	update_state(delta_time);
	//
	collide_field();
	//
	mesh_.change_motion(motion_, motion_loop_);
	//
	mesh_.update(delta_time);
	//
	mesh_.transform(transform_.localToWorldMatrix());

}

void Dragon::draw() const {
	//
	mesh_.draw();

	//
	if (enable_collider_) {
		//
		collider().draw();
	}
}

void Dragon::draw_gui() const {
	HP_.draw();
}

void Dragon::react(Actor& other) {
	if (other.tag() == "PlayerAttackTag") {
		HP_.hit_damage(400);
	}

	if (HP_.is_end()) {
		enable_collider_ = false;
		change_state(Dragon::State::Dead, MotionDead, false);
	}
}

void Dragon::update_state(float delta_time) {
	switch (state_) {
	case Dragon::State::Idle:idle(delta_time); break;
	case Dragon::State::Run:run(delta_time); break;
	case Dragon::State::Turn:turn(delta_time); break;
	case Dragon::State::Attack:attack(delta_time); break;
	case Dragon::State::FlyStart:fly_start(delta_time); break;
	case Dragon::State::FlyIdel:fly_idle(delta_time); break;
	case Dragon::State::FlyMove:fly_move(delta_time); break;
	case Dragon::State::FlyAttack:fly_attack(delta_time); break;
	case Dragon::State::Flyend:fly_end(delta_time); break;
	case Dragon::State::Dead:dead(delta_time); break;
	}
	state_timer_ += delta_time;
}

void Dragon::change_state(State state, int motion, bool loop) {
	motion_ = motion;
	motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.0f;
}

void Dragon::idle(float delta_time) {

	//
	if (is_trun()) {
		GSint motion = (target_signed_angle() >= 0.0f) ? MotionTurnLeft : MotionTurnRight;
		// U‚èŒü‚«ó‘Ô‚É‘JˆÚ
		change_state(State::Turn, motion);
		return;
	}
	//
	if (is_run()) {
		change_state(State::Run, MotionRun);
		return;
	}
	//
	if (is_attack()) {
		attack_selection();
		return;
	}
}


void Dragon::run(float delta_time) {
	// ƒ^[ƒQƒbƒg•ûŒü‚ÌŠp“x‚ð‹‚ß‚é
	float angle = target_signed_angle();
	// Œü‚«‚ð•Ï‚¦‚é
	transform_.rotate(0.0f, angle, 0.0f);
	// ‘Oi‚·‚éiƒ[ƒJƒ‹À•WŠî€j
	transform_.translate(0.0f, 0.0f, RunSpeed * delta_time);
	//
	if (is_attack()) {
		//
		attack_selection();
	}
}

void Dragon::turn(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		if (is_run()) {
			//
			int next = 0; gsRand(0, 1);
			if (next == 0) {
				change_state(State::FlyStart, MotionGoInAir, true);
			}
			else {
				change_state(State::Attack, MotionSpitFireball, false);
			}
			return;
		}

		if (is_attack()) {
			attack_selection();
		}
	}
	// U‚èŒü‚«ƒ‚[ƒVƒ‡ƒ“‚ð‚µ‚È‚ª‚çƒ^[ƒQƒbƒg•ûŒü‚ðŒü‚­
	float angle = (target_signed_angle() >= 0.0f) ? TurnAngle : -TurnAngle;
	transform_.rotate(0.0f, angle * delta_time, 0.0f);
}

void Dragon::attack(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		if (is_trun()) {
			GSint motion = (target_signed_angle() >= 0.0f) ? MotionTurnLeft : MotionTurnRight;
			// U‚èŒü‚«ó‘Ô‚É‘JˆÚ
			change_state(State::Turn, motion);
		}
		change_state(State::Idle, MotionIdle, false);
	}
}

void Dragon::fly_start(float delta_time) {
	const float Height{ 2.0f };
}

void Dragon::fly_idle(float delta_time) {

}

void Dragon::fly_move(float delta_time) {

}

void Dragon::fly_attack(float delta_time) {

}

void Dragon::fly_end(float delta_time) {

}

void Dragon::dead(float delta_time) {
	//
}

void Dragon::attack_selection() {
	//
	GSuint motion = NULL;
	//
	int attack = gsRand(0, 1);
	//
	if (attack == 0) {
		motion = MotionBite;
	}
	else if (attack == 1) {
		motion = MotionTailAttack;
	}
	change_state(State::Attack, motion, false);
}


void Dragon::bite() {
	// UŒ‚”»’è‚Ì”¼Œa
	const float AttackColliderRadius{ 1.5f };
	// UŒ‚”»’è‚ðoŒ»‚³‚¹‚éêŠ‚Ì‹——£
	const float AttackColliderDistance{ 4.5f };
	// UŒ‚”»’è‚ðo‚·êŠ‚Ì‚‚³
	const float AttackColliderHeight{ 1.0f };
	//
	float const AttackColliderWidth{ 0.0f };

	// UŒ‚”»’è‚ª—LŒø‚É‚È‚é‚Ü‚Å‚Ì’x‰„ŽžŠÔ
	const float AttackCollideDelay{ 9.0f };
	// UŒ‚”»’è‚ÌŽõ–½
	const float AttackCollideLifeSpan{ 10.0f };
	//
	generate_attac_collider(AttackColliderRadius, AttackColliderDistance, AttackColliderHeight, AttackColliderWidth, AttackCollideDelay, AttackCollideLifeSpan);
}


void Dragon::tail_attack() {
	// UŒ‚”»’è‚Ì”¼Œa
	const float AttackColliderRadius{ 2.0f };
	// UŒ‚”»’è‚ðoŒ»‚³‚¹‚éêŠ‚Ì‹——£
	const float AttackColliderDistance{ 4.5f };
	// UŒ‚”»’è‚ðo‚·êŠ‚Ì‚‚³
	const float AttackColliderHeight{ 1.5f };
	//
	const float AttackColliderWidth{ 0.0f };
	// UŒ‚”»’è‚ª—LŒø‚É‚È‚é‚Ü‚Å‚Ì’x‰„ŽžŠÔ
	const float AttackCollideDelay{ 9.0f };
	// UŒ‚”»’è‚ÌŽõ–½
	const float AttackCollideLifeSpan{ 10.0f };
	//
	generate_attac_collider(AttackColliderRadius, AttackColliderDistance, AttackColliderHeight, AttackColliderWidth, AttackCollideDelay, AttackCollideLifeSpan);
}


void Dragon::spit_fire() {
	// ’e‚ð¶¬‚·‚éêŠ‚Ì‹——£
	const float GenerateDistance{ 5.2f };
	// ¶¬‚·‚éˆÊ’u‚Ì‚‚³‚Ì•â³’l
	const float GenerateHeight{ 2.0f };
	// ’e‚ÌˆÚ“®ƒXƒs[ƒh
	const float Speed{ 1.0f };
	// ¶¬ˆÊ’u‚ÌŒvŽZ
	GSvector3 position = transform_.position() + transform_.forward() * GenerateDistance;
	// ¶¬ˆÊ’u‚Ì‚‚³‚ð•â³‚·‚é
	position.y += GenerateHeight;
	// ˆÚ“®—Ê‚ÌŒvŽZ
	GSvector3 velocity = transform_.forward() * Speed;
	// ’e‚Ì¶¬
	world_->add_actor(new FireSphere{ world_, position, velocity });
}



bool Dragon::is_trun() const {
	return target_angle() >= TurnAroundAngle;
}



bool Dragon::is_run() const {
	return target_distance() > RunDistance;

}

bool Dragon::is_attack()const {
	return target_distance() <= RunDistance && target_angle() <= 10.0f;
}