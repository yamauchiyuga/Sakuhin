#include "Witch.h"
#include"../Effect/FireSphere.h"
#include"../../World/IWorld.h"
#include"../../Collision/Field.h"
#include"../../Collision/Line.h"
#include"../AttackCollider.h"
#include"../../Assets.h"
#include"../Effect/thunder.h"
#include<GSeffect.h>

//モーション番号
enum {
	MotionTemp,
	MotionThunder,
	MotionSpitFire,
	MotionDead,
	MotionGeneration,
	MotionDmage,
	MotionIdle,
	MotionRun
};
//重力
const float Gravity{ -0.016f };
//最大体力
const float MaxHP{ 20 };
//走り出す距離
const float RunDistance{ 10.0f };
//走るスピード
const float RunSpeed{ 0.1f };
//ダメージ量
const int HitDamage{ 5 };

Witch::Witch(std::shared_ptr<IWorld> world, const GSvector3& position) :
	mesh_{ Mesh_Witch,Mesh_Witch, Mesh_Witch, MotionGeneration },
	state_{ State::Generation },
	motion_{ MotionGeneration}{
	//ワールド設定
	world_ = world;
	//タグ
	tag_ = "EnemyTag";
	//名前
	name_ = "Witch";
	//プレイヤー検索よう
	player_ = nullptr;
	//壁に衝突下か？
	hit_wall = false;
	//死亡したか？
	is_dead_ = false;
	//当たり判定球
	collider_ = BoundingSphere{ 0.5f, GSvector3{0.0f, 1.0f, 0.0f} };
	HP_ = MaxHP ;
	//座標の初期化
	transform_.position(position);
	// メッシュの変換行列を初期化
	mesh_.transform(transform_.localToWorldMatrix());
	//エフェクを出す
	const GSvector3 Offset{ 0.0f,0.1f,0.0f };
	const GSvector3 Pos = transform_.position() + Offset;
	gsPlayEffect(Effect_AppearanceBlood, &Pos);
	//イベント登録
	mesh_.add_event(MotionSpitFire, 23.0f, [=] {spit_fire(); });
	mesh_.add_event(MotionSpitFire, 10.0f, [=] {gsPlaySE(Se_WitchAttack); });
	mesh_.add_event(MotionThunder, 103.0f, [=] {thunder(); });
	mesh_.add_event(MotionThunder, 80.0f, [=] {gsPlaySE(Se_WitchAttack); });
	mesh_.add_event(MotionThunder, 105.0f, [=] {gsPlaySE(Se_WitchThunder); });
	
	
}

//更新
void Witch::update(float delta_time) {
	//プレイヤー検索
	player_ = world_->find_actor("Player");
	//状態の更新
	update_state(delta_time);
	//重力
	velocity_.y += Gravity * delta_time;
	transform_.translate(0.0f, velocity_.y, 0.0f);
	//フィールドの判定
	collide_field();
	//モーション変更
	mesh_.change_motion(motion_, motion_loop_);
	//アニメーション更新
	mesh_.update(delta_time);
	//// メッシュの変換行列を更新
	mesh_.transform(transform_.localToWorldMatrix());
}

//描画
void Witch::draw() const 
{
	mesh_.draw();
}

//当たり判定
void Witch::react(Actor& other) 
{
	//敵と衝突したか
	if (other.tag() == "EnemyTag")
	{
		collide_actor(other);
	}
	if (state_ == State::Generation)return;
	if (state_ == State::Damage)return;

	//プレイヤーと衝突したか
	if (other.tag() == "PlayerAttackTag")
	{
		hit_stop_.set_hit_stop(10.0f);
		if (HP_.cullent_health() <= 10)
		{
			gsPlaySE(Se_WitchDamage);
			change_state(State::Damage, MotionDmage, false);
		}
		//seを鳴らす
		gsPlaySE(Se_EnemyDamage);
		const GSvector3 ZanOffset{ 0.0f,0.8f,0.0f };
		const GSvector3 ZanPos = transform_.position() + ZanOffset;
		gsPlayEffect(Effect_Attack, &ZanPos);
		//修正値
		const GSvector3 Offset{ 0.0f,0.5f,0.0f };
		//エフェクを出す位置
		const GSvector3 Pos = transform_.position()+Offset;
		//エフェクト表示
		gsPlayEffect(Effect_Blood, &Pos);
		//ダメージ
		HP_.hit_damage(HitDamage);
	}
	//死亡してるか？
	if (HP_.is_end())
	{
		//当たり判定を無効
		enable_collider_ = false;
		//死亡
		is_dead_ = true;
		gsPlaySE(Se_WitchDetate);
		change_state(Witch::State::Dead, MotionDead, false);
	}
}

//状態の更新
void Witch::update_state(float delta_time) {
	switch (state_)
	{
	case Witch::State::Generation:generation(delta_time); break;
	case Witch::State::Idle:idle(delta_time); break;
	case Witch::State::Run:run(delta_time); break;
	case Witch::State::Damage:damage(delta_time); break;
	case Witch::State::Dead:dead(delta_time); break;
	case Witch::State::Attack:attack(delta_time); break;
	}
	state_timer_ += delta_time;
}

//状態の変更
void Witch::change_state(State state, int motion, bool loop) 
{
	motion_ = motion;
	motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.0f;
	hit_wall_ = false;
}

void Witch::generation(float delta_time)
{
	
	if (state_timer_ >= mesh_.motion_end_time())
	{
		gsStopEffect(Effect_AppearanceBlood);
		change_state(State::Idle, MotionIdle, true);
	}
}

//アイドル
void Witch::idle(float delta_time) 
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
			attack_selection();
			return;
		}
	}

}
//走る
void Witch::run(float delta_time) 
{
	// 前進する（ローカル座標基準）
	transform_.translate(0.0f, 0.0f, RunSpeed * delta_time);
	//攻撃範囲か、壁にぶつかったか？
	if (is_attack() || is_hit_wall()) 
	{
		attack_selection();
	}
}

void Witch::damage(float delta_time)
{
	if (state_timer_ >= mesh_.motion_end_time())
	{
		change_state(State::Idle, MotionIdle, false);
	}
}

//死亡
void Witch::dead(float delta_time)
{
	if (state_timer_ >= mesh_.motion_end_time())
	{
		//死亡
		dead_ = true;
	}
}

//攻撃中
void Witch::attack(float delta_time)
{
	if (state_timer_ >= mesh_.motion_end_time())
	{
		change_state(State::Idle, MotionIdle, false);
	}

}

//次の攻撃
void Witch::attack_selection() 
{
	// ターゲット方向の角度を求める
	float angle = target_signed_angle();
	// 向きを変える
	transform_.rotate(0.0f, angle, 0.0f);
	int Next = gsRand(0, 2);
	if (Next == 0) {
		change_state(State::Attack, MotionThunder, false);
	}
	else
	{
		change_state(State::Attack, MotionSpitFire, false);
	}
}

void Witch::thunder()
{
	const GSvector3 PlayerPos = player_->transform().position();
	std::string AttackName{ "WitchThunder" };
	world_->add_actor(std::make_unique<Thunder>(world_, PlayerPos,AttackName));
}

void Witch::spit_fire() 
{
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
	std::string AttackName{ "WitchSpitFire" };
	// 弾の生成
	world_->add_actor(std::make_unique< FireSphere>(world_, position, velocity,AttackName));
}

//走るか？
bool Witch::is_run() const
{
	return target_distance() < RunDistance;
}
//攻撃範囲か？
bool Witch::is_attack() const
{
	return target_distance() >= RunDistance;
}
