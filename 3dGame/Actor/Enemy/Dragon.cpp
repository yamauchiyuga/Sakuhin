#include "Dragon.h"
#include"../Player/Player.h"
#include"../Effect/FireSphere.h"
#include"../../World/IWorld.h"
#include"../../Collision/Field.h"
#include"../../Collision/Line.h"
#include"../AttackCollider.h"
#include"../../Assets.h"

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
const float Gravity{ -0.003f };
//
const float TurnDistance{ 1.5f };
//
const float RunDistance{ 3.0f };
//
const float RunSpeed{ 0.1f };
//
const float FlySpeed{ 0.2f };
//
const float TurnAngle{ 2.0f };
//
const float FlyTurnAngle{ 2.0f };
//
const float TurnAroundAngle{ 10.0f };
//
const int HitDamage{10 };

Dragon::Dragon(std::shared_ptr<IWorld>world, const GSvector3& position) :
	mesh_{ Mesh_Dragon,Mesh_Dragon, Mesh_Dragon, MotionIdle },
	state_{ State::Idle },
	motion_{ MotionIdle }{
	world_ = world;
	tag_ = "EnemyTag";
	name_ = "Dragon";
	player_ = nullptr;
	collider_ = BoundingSphere{ 2.0f, GSvector3{0.0f, 2.0f, 0.0f} };
	HP_ = { MaxHP };
	//À•W‚Ì‰Šú‰»
	transform_.position(position);
	mesh_.transform(transform_.localToWorldMatrix());

	mesh_.add_event(MotionBite, 36.0f, [=] {bite(); });
	mesh_.add_event(MotionTailAttack, 45.0f, [=] {tail_attack(); });
	mesh_.add_event(MotionSpitFireball, 23.0f, [=] {spit_fire(); });
	mesh_.add_event(MotionFlySpitFireball, 30.0f, [=] {spit_fire(); });

	mesh_.add_event(MotionRun, 10, [] {gsPlaySE(Se_DragonFoot); });
	mesh_.add_event(MotionRun, 40, [] {gsPlaySE(Se_DragonFoot); });
	mesh_.add_event(MotionTurnRight, 15, [] {gsPlaySE(Se_DragonFoot); });
	mesh_.add_event(MotionTurnRight, 45, [] {gsPlaySE(Se_DragonFoot); });
	mesh_.add_event(MotionTurnLeft, 15, [] {gsPlaySE(Se_DragonFoot); });
	mesh_.add_event(MotionTurnLeft, 45, [] {gsPlaySE(Se_DragonFoot); });
	mesh_.add_event(MotionFly, 13, [] {gsPlaySE(Se_DragonFire); });
	mesh_.add_event(MotionLanding, 20, [] {gsPlaySE(Se_DragonLanding); });
	mesh_.add_event(MotionGoInAir, 30, [] {gsPlaySE(Se_DragonFire); });
	mesh_.add_event(MotionGoInAir, 80, [] {gsPlaySE(Se_DragonFire); });
	mesh_.add_event(MotionDead, 10, [] {gsPlaySE(Se_DragonDeath); });
}

void Dragon::update(float delta_time) {
	//
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

	if (is_gravity()) {
		//
		velocity_.y += Gravity * delta_time;
		//
		transform_.translate(0.0f, velocity_.y, 0.0f);
	}

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
	//
	HP_.draw_boss();
}

void Dragon::react(Actor& other) {
	if (other.tag() == "PlayerAttackTag") {
		gsPlaySE(Se_EnemyDamage);
		HP_.hit_damage(HitDamage);
	}
	if (HP_.is_end()) {
		enable_collider_ = false;
		is_dead_ = true;
		change_state(Dragon::State::Dead, MotionDead, false);
		world_->game_cler();
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
	case Dragon::State::FlyEnd:fly_end(delta_time); break;
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
		change_state(State::Turn, motion, true);
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
	float angle = CLAMP(target_signed_angle(), -TurnAngle, TurnAngle);
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
			int next = gsRand(0, 3);
			if (next == 0) {
				change_state(State::FlyStart, MotionGoInAir, false);
			}
			else if (next == 1) {
				change_state(State::Run, MotionRun, true);
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
	const int FlyStart{ 50 };
	if (state_timer_ >= FlyStart) {
		const float Height{ 6.0f };
		const float LerpTime{ 0.08f };
		GSvector3 fly_pos{ transform_.position().x,Height,transform_.position().z };
		transform_.position(GSvector3::lerp(transform_.position(), fly_pos, LerpTime * delta_time));
	}

	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::FlyIdel, MotionFly, true);
	}
}

void Dragon::fly_idle(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		int next = gsRand(0, 2);
		if (next == 0) {
			change_state(State::FlyAttack, MotionFlySpitFireball, false);
		}
		else {
			change_state(State::FlyMove, MotionFly, true);
		}
	}
}

void Dragon::fly_move(float delta_time) {
	float angle = CLAMP(target_signed_angle(), -FlyTurnAngle, FlyTurnAngle);
	transform_.rotate(0.0f, angle * delta_time, 0.0f);
	transform_.translate(0.0f, 0.0f, FlySpeed * delta_time);

	if (is_attack()) {
		int next = gsRand(0, 1);
		if (next == 0) {
			change_state(State::FlyAttack, MotionFlySpitFireball, false);
		}
		else {
			change_state(State::FlyEnd, MotionLanding, false);
		}

	}

	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::FlyMove, MotionFly, true);
	}
}

void Dragon::fly_attack(float delta_time) {
	float angle = CLAMP(target_signed_angle(), -TurnAngle, TurnAngle);
	transform_.rotate(0.0f, angle * delta_time, 0.0f);
	transform_.translate(0.0f, 0.0f, -FlySpeed * delta_time);
	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::FlyIdel, MotionFly, true);
	}
}

void Dragon::fly_end(float delta_time) {
	//
	velocity_.y += Gravity * delta_time;
	//
	transform_.translate(0.0f, velocity_.y, 0.0f);
	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::Idle, MotionIdle, false);
	}
}

void Dragon::dead(float delta_time) {
	//
	const float Gravity{ -0.016f };
	//
	velocity_.y += Gravity * delta_time;
	//
	transform_.translate(0.0f, velocity_.y, 0.0f);

}

void Dragon::attack_selection() {
	//
	GSuint motion = NULL;
	//
	int attack = gsRand(0, 2);
	//
	if (attack == 0) {
		motion = MotionBite;
	}
	else if (attack == 1) {
		motion = MotionTailAttack;
	}
	else if(attack==2)
	{
		motion = MotionSpitFireball;
	}
	change_state(State::Attack, motion, false);
}


void Dragon::bite() {
	// UŒ‚”»’è‚Ì”¼Œa
	const float AttackColliderRadius{ 1.5f };
	// UŒ‚”»’è‚ðoŒ»‚³‚¹‚éêŠ‚Ì‹——£
	const float AttackColliderDistance{ 4.0f };
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
	gsPlaySE(Se_DragonAttack2);
}


void Dragon::tail_attack() {
	// UŒ‚”»’è‚Ì”¼Œa
	const float AttackColliderRadius{ 3.0f };
	// UŒ‚”»’è‚ðoŒ»‚³‚¹‚éêŠ‚Ì‹——£
	const float AttackColliderDistance{ 5.0f };
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
	gsPlaySE(Se_DragonAttack1);
}


void Dragon::spit_fire() {
	// ’e‚ð¶¬‚·‚éêŠ‚Ì‹——£
	const float GenerateDistance{ 5.0f };
	// ¶¬‚·‚éˆÊ’u‚Ì‚‚³‚Ì•â³’l
	const float GenerateHeight{ 2.0f };
	// ’e‚ÌˆÚ“®ƒXƒs[ƒh
	const float Speed{ 0.6f };
	// ¶¬ˆÊ’u‚ÌŒvŽZ
	GSvector3 position = transform_.position() + transform_.forward() * GenerateDistance;
	// ¶¬ˆÊ’u‚Ì‚‚³‚ð•â³‚·‚é
	position.y += GenerateHeight;
	// ˆÚ“®—Ê‚ÌŒvŽZ
	GSvector3 velocity = GSvector3::zero();
	velocity = transform_.forward() * Speed;

	if (state_ == State::FlyAttack) {
		velocity = (player_->transform().position() - position).normalized() * Speed;
	}
	// ’e‚Ì¶¬
	world_->add_actor(std::make_unique<FireSphere>( world_, position, velocity ));
}



bool Dragon::is_trun() const {
	return target_angle() >= TurnAroundAngle;
}



bool Dragon::is_run() const {
	return target_distance() > 6.0f;

}

bool Dragon::is_attack()const {
	return target_distance() <= 6.0f && target_angle() <= 20.0f;
}

bool Dragon::is_gravity() const
{
	if (state_ == State::FlyStart)return false;
	if (state_ == State::FlyIdel)return false;
	if (state_ == State::FlyMove)return false;
	if (state_ == State::FlyAttack)return false;
	if (state_ == State::FlyEnd)return false;
	return true;
}
