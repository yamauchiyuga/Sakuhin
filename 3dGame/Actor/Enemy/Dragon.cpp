#include "Dragon.h"
#include"../Effect/FireSphere.h"
#include"../../World/IWorld.h"
#include"../AttackCollider.h"
#include"../../Assets.h"
#include<GSeffect.h>

//モーション番号
enum
{
	MotionBite,
	MotionDead,
	MotionFly,
	MotionFlySpitFireball,
	MotionDamage,
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

//最大体力
const float MaxHP{ 400 };
//重力
const float Gravity{ -0.003f };
//攻撃範囲
const float AttackAngle{ 30.0f };
//走り出す距離
const float RunDistance{ 7.0f };
//走るスピード
const float RunSpeed{ 0.11f };
//飛行スピード
const float FlySpeed{ 0.12f };
//回転量
const float TurnAngle{ 2.0f };
//回転する角度
const float TurnAroundAngle{ 25.0f };
//ダメージ量
const int HitDamage{ 20 };

Dragon::Dragon(std::shared_ptr<IWorld>world, const GSvector3& position) :
	mesh_{ Mesh_Dragon,Mesh_Dragon, Mesh_Dragon, MotionIdle },
	state_{ State::Idle },
	motion_{ MotionIdle }
{
	//ワールド設定
	world_ = world;
	//タグ
	tag_ = "EnemyTag";
	//名前
	name_ = "Dragon";
	player_ = nullptr;
	//判定用球
	collider_ = BoundingSphere{ 2.0f, GSvector3{0.0f, 2.0f, 0.0f} };
	HP_= MaxHP;
	//座標の初期化
	transform_.position(position);
	// メッシュの変換行列を初期化
	mesh_.transform(transform_.localToWorldMatrix());
	//イベント登録
	mesh_.add_event(MotionBite, 36.0f, [=] {bite(); });
	mesh_.add_event(MotionTailAttack, 45.0f, [=] {tail_attack(); });
	mesh_.add_event(MotionSpitFireball, 23.0f, [=] {spit_fire(); });
	mesh_.add_event(MotionSpitFireball, 23.0f, [=] {gsPlaySE(Se_DragonSpitFire); });
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
	mesh_.add_event(MotionDead, 10, [] {gsPlaySE(Se_DragonDetate); });
}

//更新
void Dragon::update(float delta_time)
{
	//検索
	player_ = world_->find_actor("Player");
	//状態の変更
	update_state(delta_time);
	//フィールドの判定
	collide_field();
	//状態の変更
	mesh_.change_motion(motion_, motion_loop_);
	//アニメーション更新
	mesh_.update(delta_time);
	// メッシュの変換行列を更新
	mesh_.transform(transform_.localToWorldMatrix());
	//重力を掛けるか？
	if (is_gravity())
	{
		//重力加算
		velocity_.y += Gravity * delta_time;
		transform_.translate(0.0f, velocity_.y, 0.0f);
	}
}

//描画
void Dragon::draw() const
{
	mesh_.draw();
}

//GUI描画
void Dragon::draw_gui() const
{
	HP_.draw_boss();
}

//
void Dragon::react(Actor& other)
{
	//プレイヤーの攻撃に当たったか？
	if (other.tag() == "PlayerAttackTag")
	{
		if (HP_.cullent_health() ==200)
		{
			gsPlaySE(Se_DragonDamage);
			change_state(State::Damage, MotionDamage, false);
		}
		hit_stop_.set_hit_stop(15.0f);
		//SEを鳴らす
		gsPlaySE(Se_EnemyDamage);
		const GSvector3 ZanOffset{ 0.0f,0.8f,0.0f };
		const GSvector3 ZanPos = transform_.position() + ZanOffset;
		gsPlayEffect(Effect_Attack1, &ZanPos);
		//ダメージ
		HP_.hit_damage(HitDamage);
		//エフェクの描画
		GSvector3 Offset{ 0.0f,0.5f,0.0f };
		GSvector3 Pos = transform_.position() + Offset;
		gsPlayEffect(Effect_Blood, &Pos);
	}
	//死亡
	if (HP_.is_end())
	{
		//当たり判定を無効
		enable_collider_ = false;
		//死亡する
		is_dead_ = true;
		change_state(Dragon::State::Dead, MotionDead, false);
		//クリア画面を呼ぶ
		world_->game_cler();
	}
}

//状態の更新
void Dragon::update_state(float delta_time)
{
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
	case Dragon::State::Damage:damage(delta_time); break;
	case Dragon::State::Dead:dead(delta_time); break;
	}
	state_timer_ += delta_time;
}

//状態の変更
void Dragon::change_state(State state, int motion, bool loop) {
	motion_ = motion;
	motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.0f;
}

//アイドル
void Dragon::idle(float delta_time)
{
	//振り向くか？
	if (is_trun())
	{
		//プレイヤーとの角度が0以上か？
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
		//攻撃をランダムに選択
		int next = gsRand(0, 2);
		//飛行スタート
		if (next == 0)
		{
			GSvector3 position{ transform_.position() };
			gsPlayEffect(Effect_Smoke, &position);
			change_state(State::FlyStart, MotionGoInAir, false);
		}
		else
		{
			attack_selection();
		}
		return;
	}
}


//走る
void Dragon::run(float delta_time)
{
	// ターゲット方向の角度を求める
	float angle = CLAMP(target_signed_angle(), -TurnAngle, TurnAngle);
	transform_.rotate(0.0f, angle * delta_time, 0.0f);
	transform_.translate(0.0f, 0.0f, RunSpeed * delta_time);
	if (is_attack())
	{
		attack_selection();
	}
}

//振る向き
void Dragon::turn(float delta_time)
{
	if (state_timer_ >= mesh_.motion_end_time())
	{
		//行動をランダムに選択
		int next = gsRand(0, 3);
		if (is_run())
		{
			//飛行スタート
			if (next == 0) {
				// エフェクを出す
				GSvector3 position{ transform_.position() };
				gsPlayEffect(Effect_Smoke, &position);
				change_state(State::FlyStart, MotionGoInAir, false);
			}
			//走る
			else if (next == 1)
			{
				change_state(State::Run, MotionRun, true);
			}
			//火球
			else
			{
				change_state(State::Attack, MotionSpitFireball, false);
			}
			return;
		}

		if (is_attack())
		{
			attack_selection();
		}
	}

	// 振り向きモーションをしながらターゲット方向を向く
	float angle = (target_signed_angle() >= 0.0f) ? TurnAngle : -TurnAngle;
	transform_.rotate(0.0f, angle * delta_time, 0.0f);
}

//攻撃
void Dragon::attack(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		if (is_trun())
		{
			GSint motion = (target_signed_angle() >= 0.0f) ? MotionTurnLeft : MotionTurnRight;
			// 振り向き状態に遷移
			change_state(State::Turn, motion);
		}
		change_state(State::Idle, MotionIdle, false);
	}
}

//飛行スタート
void Dragon::fly_start(float delta_time)
{
	//少し留める
	const int FlyStart{ 50 };
	if (state_timer_ >= FlyStart)
	{
		const float Height{ 6.0f };
		const float LerpTime{ 0.08f };
		//上昇位置の決定
		GSvector3 fly_pos{ transform_.position().x,Height,transform_.position().z };
		transform_.position(GSvector3::lerp(transform_.position(), fly_pos, LerpTime * delta_time));
	}

	if (state_timer_ >= mesh_.motion_end_time())
	{
		change_state(State::FlyIdel, MotionFly, true);
	}
}

//飛行中
void Dragon::fly_idle(float delta_time)
{
	if (state_timer_ >= mesh_.motion_end_time())
	{
		//行動をランダムに選択
		int next = gsRand(0, 2);
		//空中攻撃
		if (next == 0)
		{
			change_state(State::FlyAttack, MotionFlySpitFireball, false);
		}
		//移動
		else
		{
			change_state(State::FlyMove, MotionFly, true);
		}
	}
}

//飛行移動
void Dragon::fly_move(float delta_time)
{
	//ターゲット方向の角度を求める
	float angle = CLAMP(target_signed_angle(), -TurnAngle, TurnAngle);
	transform_.rotate(0.0f, angle * delta_time, 0.0f);
	transform_.translate(0.0f, 0.0f, FlySpeed * delta_time);

	if (is_attack())
	{
		//行動をランダムに選択
		int next = gsRand(0, 1);
		//空中攻撃
		if (next == 0)
		{
			change_state(State::FlyAttack, MotionFlySpitFireball, false);
		}
		//着地
		else
		{
			//エフェクを出す
			GSvector3 position{ transform_.position() };
			gsPlayEffect(Effect_Smoke, &position);
			change_state(State::FlyEnd, MotionLanding, false);
		}
	}

	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::FlyMove, MotionFly, true);
	}
}

//空中攻撃
void Dragon::fly_attack(float delta_time) {
	//ターゲット方向の角度を求める
	float angle = (target_signed_angle() >= 0.0f) ? TurnAngle : -TurnAngle;
	transform_.rotate(0.0f, angle * delta_time, 0.0f);
	transform_.translate(0.0f, 0.0f, -FlySpeed * delta_time);
	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::FlyIdel, MotionFly, true);
	}
}

//着地
void Dragon::fly_end(float delta_time) {
	//重力加算
	velocity_.y += Gravity * delta_time;
	transform_.translate(0.0f, velocity_.y, 0.0f);
	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::Idle, MotionIdle, false);
	}
}

//
void Dragon::damage(float delta_time)
{
	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::Idle, MotionIdle, false);
	}
}

//死亡
void Dragon::dead(float delta_time) {
	//何もしない
}

//攻撃選択
void Dragon::attack_selection() {
	GSuint motion = NULL;
	//攻撃をランダムに選択
	int attack = gsRand(0, 2);
	//噛みつき
	if (attack == 0)
	{
		motion = MotionBite;
	}
	//尻尾
	else if (attack == 1)
	{
		motion = MotionTailAttack;
	}
	//火球
	else if (attack == 2)
	{
		motion = MotionSpitFireball;
	}
	change_state(State::Attack, motion, false);
}

//噛みつき攻撃
void Dragon::bite() {
	// 攻撃判定の半径
	const float AttackColliderRadius{ 1.5f };
	// 攻撃判定を出現させる場所の距離
	const float AttackColliderDistance{ 4.0f };
	// 攻撃判定を出す場所の高さ
	const float AttackColliderHeight{ 1.0f };
	//
	float const AttackColliderWidth{ 0.0f };

	// 攻撃判定が有効になるまでの遅延時間
	const float AttackCollideDelay{ 9.0f };
	// 攻撃判定の寿命
	const float AttackCollideLifeSpan{ 10.0f };
	//
	std::string AttackName{ "DragonBite" };
	generate_attac_collider(AttackColliderRadius, AttackColliderDistance, AttackColliderHeight, AttackColliderWidth, AttackCollideDelay, AttackCollideLifeSpan, AttackName);
	gsPlaySE(Se_DragonAttack2);
}

//尻尾攻撃
void Dragon::tail_attack() {
	// 攻撃判定の半径
	const float AttackColliderRadius{ 3.0f };
	// 攻撃判定を出現させる場所の距離
	const float AttackColliderDistance{ 5.0f };
	// 攻撃判定を出す場所の高さ
	const float AttackColliderHeight{ 1.5f };
	// 攻撃判定を出す場所の幅
	const float AttackColliderWidth{ 0.0f };
	// 攻撃判定が有効になるまでの遅延時間
	const float AttackCollideDelay{ 9.0f };
	// 攻撃判定の寿命
	const float AttackCollideLifeSpan{ 10.0f };
	//
	std::string AttackName{ "DragonTail" };
	//生成
	generate_attac_collider(AttackColliderRadius, AttackColliderDistance, AttackColliderHeight, AttackColliderWidth, AttackCollideDelay, AttackCollideLifeSpan, AttackName);
	gsPlaySE(Se_DragonAttack1);
}

//火球
void Dragon::spit_fire()
{
	// 弾を生成する場所の距離
	const float GenerateDistance{ 3.0f };
	// 生成する位置の高さの補正値
	const float GenerateHeight{ 1.0f };
	// 弾の移動スピード
	const float Speed{ 0.6f };
	// 生成位置の計算
	GSvector3 position = transform_.position() + transform_.forward() * GenerateDistance;
	// 生成位置の高さを補正する
	position.y += GenerateHeight;
	// 移動量の計算
	GSvector3 velocity = GSvector3::zero();
	velocity = transform_.forward() * Speed;
	//
	std::string AttackName{ "DragonSpitFire" };
	if (state_ == State::FlyAttack)
	{
		AttackName = "DragonFlySpitFire";
		velocity = (player_->transform().position() - position).normalized() * Speed;
	}
	// 弾の生成
	world_->add_actor(std::make_unique<FireSphere>(world_, position, velocity, AttackName));
}

//振り返るか
bool Dragon::is_trun() const
{
	return target_angle() >= TurnAroundAngle;
}

//走るか？
bool Dragon::is_run() const
{
	return   target_distance() > RunDistance;
}

//攻撃範囲か？
bool Dragon::is_attack()const
{
	return target_distance() <= RunDistance && target_angle() <= AttackAngle;
}

//重力を有効にするか？
bool Dragon::is_gravity() const
{
	//飛んでるときは無効
	if (state_ == State::FlyStart)return false;
	if (state_ == State::FlyIdel)return false;
	if (state_ == State::FlyMove)return false;
	if (state_ == State::FlyAttack)return false;
	if (state_ == State::FlyEnd)return false;
	return true;
}
