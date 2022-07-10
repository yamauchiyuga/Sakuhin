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
const float RunDistance{ 2.5f };
//走るスピード
const float RunSpeed{ 0.06f };
//回転量
const float TurnAngle{ 2.5f };
//回転する角度
const float TurnAroundAngle{ 10.0f };
//
const float AnimationDelaytime{ 60.0f };
//ダメージ
const int HitDamage{ 5 };

Skeleton::Skeleton(std::shared_ptr<IWorld> world, const GSvector3& position) :
	mesh_{ Mesh_Skeleton,Mesh_Skeleton, Mesh_Skeleton, MotionGeneration,false },
	state_{ State::Generation },
	motion_{ MotionGeneration }
{
	//ワールド設定
	world_ = world;
	//タグ
	tag_ = "EnemyTag";
	//名前
	name_ = "Skeleton";
	is_dead_ = false;
	player_ = nullptr;
	//判定用球
	collider_ = BoundingSphere{ 0.4f, GSvector3{0.0f, 1.0f, 0.0f} };
	HP_ = { MaxHP };
	//座標の初期化
	transform_.position(position);
	// メッシュの変換行列を初期化
	mesh_.transform(transform_.localToWorldMatrix());
	//イベント登録
	mesh_.add_event(MotionAttack, 10.0f, [=] {slash(); });
	mesh_.add_event(MotionAttack, 10.0f, [] {gsPlaySE(Se_PlayerAttack); });
}

//更新
void Skeleton::update(float delta_time)
{
	//プレイヤー検索
	player_ = world_->find_actor("Player");
	//状態の更新
	update_state(delta_time);
	//重力加算
	velocity_.y += Gravity * delta_time;
	transform_.translate(0.0f, velocity_.y, 0.0f);
	//フィールドの判定
	collide_field();
	//状態の変更
	mesh_.change_motion(motion_, motion_loop_);
	//アニメーション更新
	mesh_.update(delta_time);
	//// メッシュの変換行列を更新
	mesh_.transform(transform_.localToWorldMatrix());
}

//描画
void Skeleton::draw() const
{
	mesh_.draw();
}

void Skeleton::react(Actor& other)
{
	//生成中なら判定しない
	if (state_ == State::Generation)return;
	//敵との判定
	if (other.tag() == "EnemyTag")
	{
		collide_actor(other);
	}
	//プレイヤーとの判定
	if (other.tag() == "PlayerAttackTag")
	{
		//SEを鳴らす
		gsPlaySE(Se_EnemyDamage);
		//修正値
		GSvector3 Offset{ 0.0f,0.6f,0.0f };
		//エフェクを出す位置
		GSvector3 Pos = transform_.position() + Offset;
		//エフェク描画
		gsPlayEffect(Effect_Blood, &Pos);
		//ダメージ
		HP_.hit_damage(HitDamage);
	}

	//死亡したら
	if (HP_.is_end())
	{
		//当たり判定を無効か
		enable_collider_ = false;
		//死亡
		is_dead_ = true;
		gsPlaySE(Se_SkeletonDetate);
		change_state(Skeleton::State::Dead, MotionDead, false);
	}

}

//状態の更新
void Skeleton::update_state(float delta_time)
{
	switch (state_) {
	case Skeleton::State::Generation: generation(delta_time); break;
	case Skeleton::State::Idle:idle(delta_time); break;
	case Skeleton::State::Run:run(delta_time); break;
	case Skeleton::State::Turn:turn(delta_time); break;
	case Skeleton::State::Attack:attack(delta_time); break;
	case Skeleton::State::Dead:dead(delta_time); break;
	}
	state_timer_ += delta_time;
}

//状態の変更
void Skeleton::change_state(State state, int motion, bool loop) {
	motion_ = motion;
	motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.0f;
}

//生成
void Skeleton::generation(float delta_time)
{
	if (state_timer_ >= mesh_.motion_end_time())
	{
		change_state(State::Idle, MotionIdle, true);
	}
}

//アイドル
void Skeleton::idle(float delta_time)
{
	if (state_timer_ >= mesh_.motion_end_time() + AnimationDelaytime)
	{
		//振り向くか？
		if (is_trun())
		{
			GSint motion = (target_signed_angle() >= 0.0f) ? MotionTurnLeft : MotionTurnRight;
			// 振り向き状態に遷移
			change_state(State::Turn, motion, true);
			return;
		}
		//走るか？
		if (is_run())
		{
			change_state(State::Run, MotionRun, true);
			return;
		}
		//攻撃範囲か？
		if (is_attack())
		{
			change_state(State::Attack, MotionAttack, false);
			return;
		}
	}
}

//走る
void Skeleton::run(float delta_time) 
{
	// ターゲット方向の角度を求める
	float angle = CLAMP(target_signed_angle(), -TurnAngle, TurnAngle);
	// 向きを変える
	transform_.rotate(0.0f, angle, 0.0f);
	// 前進する（ローカル座標基準）
	transform_.translate(0.0f, 0.0f, RunSpeed * delta_time);
	if (is_attack())
	{
		change_state(State::Attack, MotionAttack, false);
	}
}

//振り向き
void Skeleton::turn(float delta_time) 
{
	if (state_timer_ >= mesh_.motion_end_time())
	{
		//走るか？
		if (is_run())
		{
			change_state(State::Run, MotionRun, true);
			return;
		}
		//攻撃範囲か？
		if (is_attack())
		{
			change_state(State::Attack, MotionAttack, false);
			return;
		}
	}
	// 振り向きモーションをしながらターゲット方向を向く
	float angle = (target_signed_angle() >= 0.0f) ? TurnAngle : -TurnAngle;
	transform_.rotate(0.0f, angle * delta_time, 0.0f);
}

//死亡
void Skeleton::dead(float delta_time) 
{
	if (state_timer_ >= mesh_.motion_end_time()) {
		dead_ = true;
	}
}

//攻撃
void Skeleton::attack(float delta_time)
{
	if (state_timer_ >= mesh_.motion_end_time()) 
	{
		if (is_trun()) {
			GSint motion = (target_signed_angle() >= 0.0f) ? MotionTurnLeft : MotionTurnRight;
			// 振り向き状態に遷移
			change_state(State::Turn, motion);
		}
		change_state(State::Idle, MotionIdle, true);
	}
}

//攻撃の判定生成
void Skeleton::slash() 
{
	// 攻撃判定の半径
	const float AttackColliderRadius{ 0.5f };
	// 攻撃判定を出現させる場所の距離
	const float AttackColliderDistance{ 1.5f };
	// 攻撃判定を出す場所の高さ
	const float AttackColliderHeight{ 1.0f };
	// 攻撃判定が有効になるまでの遅延時間
	const float AttackCollideDelay{ 0.0f };
	// 攻撃判定の寿命
	const float AttackCollideLifeSpan{ 5.0f };
	//攻撃用球生成
	generate_attac_collider(AttackColliderRadius, AttackColliderDistance, AttackColliderHeight, 0.0f, AttackCollideDelay, AttackCollideLifeSpan);
}

//走しるか？
bool Skeleton::is_run() const
{
	return target_distance() > RunDistance;
}

//振り向くか？
bool Skeleton::is_trun() const
{
	return target_angle() >= TurnAroundAngle;
}

//攻撃範囲か？
bool Skeleton::is_attack() const 
{
	return target_distance() <= RunDistance && target_angle() <= TurnAroundAngle;
}
