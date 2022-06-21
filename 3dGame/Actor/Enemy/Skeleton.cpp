#include "Skeleton.h"
#include"../Player/Player.h"
#include"../../World/IWorld.h"
#include"../../Collision/Field.h"
#include"../../Collision/Line.h"
#include"../AttackCollider.h"
#include"../../Assets.h"

enum
{
	MotionAttack,
	MotionDead,
	MotionRun,
	MotionTurnLeft,
	MotionTurnRight,
	MotionGeneration,
	MotionIdle
};

const float Gravity{ -0.016f };

//
const float MaxHP{ 10 };
//
const float RunDistance{ 1.5f };
//
const float RunSpeed{ 0.06f };
//
const float TurnAngle{ 2.5f };
//
const float TurnAroundAngle{ 4.0f };
//
const int HitDamage{ 5 };

Skeketon::Skeketon(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Skeleton,Mesh_Skeleton, Mesh_Skeleton, MotionGeneration,false },
	state_{ State::Generation },
	attack_time_{ 0.0f },
	motion_{ MotionGeneration }{
	world_ = world;
	tag_ = "EnemyTag";
	name_ = "Skeketon";
	player_ = nullptr;
	collider_ = BoundingSphere{ 0.4f, GSvector3{0.0f, 1.0f, 0.0f} };
	HP_ = { MaxHP };
	//À•W‚Ì‰Šú‰»
	transform_.position(position);
	mesh_.transform(transform_.localToWorldMatrix());

	mesh_.add_animation_event(MotionAttack, 12.0f, [=] {slash(); });
}

void Skeketon::update(float delta_time) {
	//
	player_ = world_->find_actor("Player");
	//
	update_state(delta_time);
	//
	velocity_.y += Gravity * delta_time;

	transform_.translate(0.0f, velocity_.y, 0.0f);
	//
	collide_field();
	//
	mesh_.change_motion(motion_, motion_loop_);
	//
	mesh_.update(delta_time);
	//
	mesh_.transform(transform_.localToWorldMatrix());
}

void Skeketon::draw() const {
	//
	mesh_.draw();

	//
	if (enable_collider_) {
		//
		collider().draw();
	}
}

void Skeketon::react(Actor& other) {

	if (state_ == State::Generation)return;

	if (other.tag() == "EnemyTag") {
		collide_actor(other);
	}
	if (other.tag() == "PlayerAttackTag") {
		HP_.hit_damage(HitDamage);
	}

	if (HP_.is_end()) {
		enable_collider_ = false;
		change_state(Skeketon::State::Dead, MotionDead, false);
	}

}

void Skeketon::update_state(float delta_time)
{
	switch (state_) {
	case Skeketon::State::Generation: generation(delta_time); break;
	case Skeketon::State::Idle:idle(delta_time); break;
	case Skeketon::State::Run:run(delta_time); break;
	case Skeketon::State::Turn:turn(delta_time); break;
	case Skeketon::State::Attack:attack(delta_time); break;
	case Skeketon::State::Dead:dead(delta_time); break;
	}
	state_timer_ += delta_time;
}

void Skeketon::change_state(State state, int motion, bool loop) {
	motion_ = motion;
	motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.0f;
}

void Skeketon::generation(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::Idle, MotionIdle, true);
	}
}

void Skeketon::idle(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()+60.0f) {
		//
		if (is_trun()) {
			GSint motion = (target_signed_angle() >= 0.0f) ? MotionTurnLeft : MotionTurnRight;
			// U‚èŒü‚«ó‘Ô‚É‘JˆÚ
			change_state(State::Turn, motion, true);
			return;
		}
		//
		if (is_run()) {
			change_state(State::Run, MotionRun, true);
			return;
		}

		if (is_attack()) {
			change_state(State::Attack, MotionAttack, false);
			return;
		}
	}
}

void Skeketon::run(float delta_time) {
	// ƒ^[ƒQƒbƒg•ûŒü‚ÌŠp“x‚ğ‹‚ß‚é
	float angle = CLAMP(target_signed_angle(), -TurnAngle, TurnAngle);
	// Œü‚«‚ğ•Ï‚¦‚é
	transform_.rotate(0.0f, angle, 0.0f);
	// ‘Oi‚·‚éiƒ[ƒJƒ‹À•WŠî€j
	transform_.translate(0.0f, 0.0f, RunSpeed * delta_time);
	if (is_attack()) {
		change_state(State::Attack, MotionAttack, false);
	}
}

void Skeketon::turn(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		if (is_run()) {
			change_state(State::Run, MotionRun, true);
			return;
		}

		if (is_attack()) {
			change_state(State::Attack, MotionAttack, false);
			return;
		}
	}
	// U‚èŒü‚«ƒ‚[ƒVƒ‡ƒ“‚ğ‚µ‚È‚ª‚çƒ^[ƒQƒbƒg•ûŒü‚ğŒü‚­
	float angle = (target_signed_angle() >= 0.0f) ? TurnAngle : -TurnAngle;
	transform_.rotate(0.0f, angle * delta_time, 0.0f);
}

void Skeketon::dead(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		dead_ = true;
	}
}

void Skeketon::attack(float delta_time) {

	if (state_timer_ >= mesh_.motion_end_time()) {
		if (is_trun()) {
			GSint motion = (target_signed_angle() >= 0.0f) ? MotionTurnLeft : MotionTurnRight;
			// U‚èŒü‚«ó‘Ô‚É‘JˆÚ
			change_state(State::Turn, motion);
		}
		change_state(State::Idle, MotionIdle, true);
	}
}

void Skeketon::slash() {

	// UŒ‚”»’è‚Ì”¼Œa
	const float AttackColliderRadius{ 0.5f };
	// UŒ‚”»’è‚ğoŒ»‚³‚¹‚éêŠ‚Ì‹——£
	const float AttackColliderDistance{ 1.5f };
	// UŒ‚”»’è‚ğo‚·êŠ‚Ì‚‚³
	const float AttackColliderHeight{ 1.0f };
	//
	const float AttackColliderWidth{ 0.0f };
	// UŒ‚”»’è‚ª—LŒø‚É‚È‚é‚Ü‚Å‚Ì’x‰„ŠÔ
	const float AttackCollideDelay{ 0.0f };
	// UŒ‚”»’è‚Ìõ–½
	const float AttackCollideLifeSpan{ 5.0f };
	//
	generate_attac_collider(AttackColliderRadius, AttackColliderDistance, AttackColliderHeight, AttackColliderWidth, AttackCollideDelay, AttackCollideLifeSpan);
}

bool Skeketon::is_run() const {
	return target_distance() > 2.0f;
}

bool Skeketon::is_trun() const
{
	return target_angle() >= 10.0f;
}

bool Skeketon::is_attack() const {
	return target_distance() <= 2.0f && target_angle() <= 10.0f;
}
