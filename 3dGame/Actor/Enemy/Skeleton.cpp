#include "Skeleton.h"
#include"../Player/Player.h"
#include"../../World/IWorld.h"
#include"../../Collision/Field.h"
#include"../../Collision/Line.h"
#include"../AttackCollider.h"
#include"../../Assets.h"
#include<GSeffect.h>

//モーション番号
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

// 重力
const float Gravity{ -0.016f }; 
//最大体力
const float MaxHP{ 10 };
//走り出す距離
const float RunDistance{ 1.5f };
//走るスピード
const float RunSpeed{ 0.06f };
//回転量
const float TurnAngle{ 2.5f };
//回転する角度
const float TurnAroundAngle{ 4.0f };
//ダメージ
const int HitDamage{ 5 };

Skeketon::Skeketon(std::shared_ptr<IWorld> world, const GSvector3& position) :
	mesh_{ Mesh_Skeleton,Mesh_Skeleton, Mesh_Skeleton, MotionGeneration,false },
	state_{ State::Generation },
	motion_{ MotionGeneration }
{
	//ワールド設定
	world_ = world;
	//タグ
	tag_ = "EnemyTag";
	//名前
	name_ = "Skeketon";
	is_dead_ = false;
	player_ = nullptr;
	//判定用球
	collider_ = BoundingSphere{ 0.4f, GSvector3{0.0f, 1.0f, 0.0f} };
	HP_ = { MaxHP };
	//座標の初期化
	transform_.position(position);
	mesh_.transform(transform_.localToWorldMatrix());
	//イベント登録
	mesh_.add_event(MotionAttack, 10.0f, [=] {slash(); });
	mesh_.add_event(MotionAttack, 10.0f, [] {gsPlaySE(Se_PlayerAttack); });
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
}

void Skeketon::react(Actor& other) {

	if (state_ == State::Generation)return;

	if (other.tag() == "EnemyTag") {
		collide_actor(other);
	}
	if (other.tag() == "PlayerAttackTag") {
		gsPlaySE(Se_EnemyDamage);
		GSvector3 Offset{ 0.0f,0.6f,0.0f };
		GSvector3 Pos = transform_.position() + Offset;
		gsPlayEffect(Effect_Blood, &Pos);
		HP_.hit_damage(HitDamage);
	}

	if (HP_.is_end()) {
		enable_collider_ = false;
		is_dead_ = true;
		gsPlaySE(Se_SkeletonDetate);
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
			// 振り向き状態に遷移
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
	// ターゲット方向の角度を求める
	float angle = CLAMP(target_signed_angle(), -TurnAngle, TurnAngle);
	// 向きを変える
	transform_.rotate(0.0f, angle, 0.0f);
	// 前進する（ローカル座標基準）
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
	// 振り向きモーションをしながらターゲット方向を向く
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
			// 振り向き状態に遷移
			change_state(State::Turn, motion);
		}
		change_state(State::Idle, MotionIdle, true);
	}
}

void Skeketon::slash() {

	// 攻撃判定の半径
	const float AttackColliderRadius{ 0.5f };
	// 攻撃判定を出現させる場所の距離
	const float AttackColliderDistance{ 1.5f };
	// 攻撃判定を出す場所の高さ
	const float AttackColliderHeight{ 1.0f };
	//
	const float AttackColliderWidth{ 0.0f };
	// 攻撃判定が有効になるまでの遅延時間
	const float AttackCollideDelay{ 0.0f };
	// 攻撃判定の寿命
	const float AttackCollideLifeSpan{ 5.0f };
	//
	generate_attac_collider(AttackColliderRadius, AttackColliderDistance, AttackColliderHeight, AttackColliderWidth, AttackCollideDelay, AttackCollideLifeSpan);
}

bool Skeketon::is_run() const {
	return target_distance() > RunDistance;
}

bool Skeketon::is_trun() const
{
	return target_angle() >= TurnAroundAngle;
}

bool Skeketon::is_attack() const {
	return target_distance() <= RunDistance && target_angle() <= TurnAroundAngle;
}
