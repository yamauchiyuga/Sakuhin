#include "Witch.h"
#include"../Player/Player.h"
#include"../Effect/FireSphere.h"
#include"../Effect/Explosion.h"
#include"../../World/IWorld.h"
#include"../../Collision/Field.h"
#include"../../Collision/Line.h"
#include"../AttackCollider.h"
#include"../../Assets.h"

enum {
	MotionExplosion,
	MotionSpitFire,
	MotionDead,
	MotionIdle,
	MotionRun
};

const float Gravity{ -0.016f };
//
const float MaxHP{ 15 };
//
const float RunDistance{ 10.0f };
//
const float StaopDistance{ 12.0f };
//
const float RunSpeed{ 0.1f };
//
const int HitDamage{ 5 };

Witch::Witch(std::shared_ptr<IWorld> world, const GSvector3& position) :
	mesh_{ Mesh_Witch,Mesh_Witch, Mesh_Witch, MotionIdle },
	state_{ State::Idle },
	motion_{ MotionIdle }{
	world_ = world;
	tag_ = "EnemyTag";
	name_ = "Witch";
	player_ = nullptr;
	hit_wall = false;
	is_dead_=false;
	collider_ = BoundingSphere{ 0.5f, GSvector3{0.0f, 1.0f, 0.0f} };
	HP_ = { MaxHP };
	//座標の初期化
	transform_.position(position);
	mesh_.transform(transform_.localToWorldMatrix());

	mesh_.add_event(MotionSpitFire, 23.0f, [=] {spit_fire(); });
	//mesh_.add_animation_event(MotionExplosion, 100.0f, [=] {explosion(); });
}

void Witch::update(float delta_time) {
	//
	player_ = world_->find_actor("Player");
	//
	update_state(delta_time);
	//
	velocity_.y += Gravity * delta_time;
	//
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

void Witch::draw() const {
	//
	mesh_.draw();
	//
	if (enable_collider_) {
		//
		collider().draw();
	}
}

void Witch::react(Actor& other) {
	if (other.tag() == "EnemyTag") {
		collide_actor(other);
	}

	if (other.tag() == "PlayerAttackTag") {
		HP_.hit_damage(HitDamage);
	}

	if (HP_.is_end()) {
		enable_collider_ = false;
		is_dead_ = true;
		change_state(Witch::State::Dead, MotionDead, false);
	}
}

void Witch::update_state(float delta_time) {
	switch (state_)
	{
	case Witch::State::Idle:idle(delta_time); break;
	case Witch::State::Run:run(delta_time); break;
	case Witch::State::Dead:dead(delta_time); break;
	case Witch::State::Attack:attack(delta_time); break;
	}
	state_timer_ += delta_time;
}

void Witch::change_state(State state, int motion, bool loop) {
	motion_ = motion;
	motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.0f;
	hit_wall_ = false;
}

void Witch::idle(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		if (is_run()) {
			change_state(State::Run, MotionRun, true);
			return;
		}

		if (is_attack()) {
			attack_selection();
			return;
		}
	}

}

void Witch::run(float delta_time) {
	
	// 前進する（ローカル座標基準）
	transform_.translate(0.0f, 0.0f, RunSpeed * delta_time);
	if (is_attack()||is_hit_wall()) {
		attack_selection();
	}
}

void Witch::dead(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		dead_ = true;
	}
}

void Witch::attack(float delta_time) {

	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::Idle, MotionIdle, false);
	}

}

void Witch::attack_selection() {
	// ターゲット方向の角度を求める
	float angle = target_signed_angle();
	// 向きを変える
	transform_.rotate(0.0f, angle, 0.0f);
	change_state(State::Attack, MotionSpitFire, false);
}

//void Witch::explosion() {
//
//	// 爆発の生成
//	world_->add_actor(new Explosion{ world_, player_->transform().position() });
//}

void Witch::spit_fire() {
	// 弾を生成する場所の距離
	const float GenerateDistance{ 1.0f };
	// 生成する位置の高さの補正値
	const float GenerateHeight{ 1.0f };
	// 弾の移動スピード
	const float Speed{ 0.4f };
	// 生成位置の計算
	GSvector3 position = transform_.position() + transform_.forward() * GenerateDistance;
	// 生成位置の高さを補正する
	position.y += GenerateHeight;
	// 移動量の計算
	GSvector3 velocity = GSvector3::zero();
	velocity = transform_.forward() * Speed;

	// 弾の生成
	world_->add_actor(std::make_unique< FireSphere>( world_, position, velocity ));
}

bool Witch::is_run() const
{
	return target_distance() < RunDistance;
}

bool Witch::is_attack() const
{
	return target_distance() >= RunDistance;
}
