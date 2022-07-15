#include"Player.h"
#include"../AttackCollider.h"
#include "../../World/IWorld.h"
#include "../../Collision/Field.h"
#include "../../Collision/Line.h"
#include "../../Assets.h"
#include"../../Input.h"
#include <GSeffect.h>

//モーション番号
enum
{
	MotionAttack,
	MotionAttack2,
	MotionAttack3,
	MotionDamage,
	MotionEnd,
	MotionIdle,
	MotionGuardAccept,
	MotionGuardLoop,
	MotionGuardStart,
	MotionDodge,
	MotionRun
};
//スポーンする位置
const GSvector3 RestartPosition{ -5.0f,6.0f,10.0f };


//高さ
const float PlayerHeight{ 1.0f };
//球の半径
const float PlayerRadius{ 0.7f };
//補完
const float FootOffset{ 0.1f };
//重力
const float Gravity{ -0.016f };
//回避距離
const float DodgeDistance{ 0.7 };
//最大体力
const float MaxHP{ 100.0f };
//最大スタミナ
const float MaxST{ 100.0f };
//入力
const float Input{ 0.3f };
//リスポーンするy軸の高さ
const float EndtLinePos_Y{ -50.0f };
//回避消費スタミナ
const int DodgeStamina{ 10 };
//ガード消費スタミナ
const int GuradStamin{ 20 };
//攻撃消費スタミナ
const int AttackStamina{ 10 };
//最大コンボ数
const int MaxCombo{ 2 };
//ダメージ量
const int Damage{ 5 };


//コンストラクタ
Player::Player(std::shared_ptr<IWorld> world, const GSvector3& position) :
	mesh_{ Mesh_Player,Mesh_Player, Mesh_Player, MotionIdle ,true },
	motion_{ MotionIdle },
	motion_loop_{ true },
	det_line_{ false },
	state_{ State::Move },
	state_timer_{ 0.0f },
	combo_{ 0 },
	HP_{ MaxHP },
	ST_{ MaxST }
{
	// ワールドを設定
	world_ = world;
	//名前
	name_ = "Player";
	//タグ
	tag_ = "PlayerTag";
	//当たり判定球
	collider_ = BoundingSphere{ PlayerRadius,GSvector3{0.0f,PlayerHeight,0.0f} };
	// 座標の初期化
	transform_.position(position);
	// メッシュの変換行列を初期化
	mesh_.transform(transform_.localToWorldMatrix());

	//イベント登録
	mesh_.add_event(MotionAttack, 15.0f, [=] {generate_attac_collider(); });
	mesh_.add_event(MotionAttack2, 15.0f, [=] {generate_attac_collider(); });
	mesh_.add_event(MotionAttack3, 22.0f, [=] {generate_attac_collider(); });
	mesh_.add_event(MotionRun, 8.0f, [] {gsPlaySE(Se_PlayerRun); });
	mesh_.add_event(MotionRun, 30.0f, [] {gsPlaySE(Se_PlayerRun); });
	mesh_.add_event(MotionDodge, 5.0f, [] {gsPlaySE(Se_PlayerDrop); });
	mesh_.add_event(MotionAttack, 10.0f, [] {gsPlaySE(Se_PlayerAttack); });
	mesh_.add_event(MotionAttack2, 10.0f, [] {gsPlaySE(Se_PlayerAttack); });
	mesh_.add_event(MotionAttack3, 10.0f, [] {gsPlaySE(Se_PlayerAttack); });
}

//更新
void Player::update(float delta_time)
{
	//
	update_state(delta_time);
	//重力加算
	velocity_.y += Gravity * delta_time;
	transform_.translate(0.0f, velocity_.y, 0.0f);
	//フィールドの当たり判定
	collide_field();
	//状態の変更
	mesh_.change_motion(motion_, motion_loop_);
	//アニメーション更新
	mesh_.update(delta_time);
	// メッシュの変換行列を更新
	mesh_.transform(transform_.localToWorldMatrix());
	//スタミナの更新
	ST_.update(delta_time);
	//リスポーン
	if (end_line())
	{
		transform_.position() = GSvector3::zero();
		transform_.position(RestartPosition);
	}

}

//描画
void Player::draw()const
{
	mesh_.draw();
	if (enable_collider_) {
		collider().draw();
	}
}

//GUI描画
void Player::draw_gui()const
{
	HP_.draw_player();
	ST_.draw();
}

//衝突判定
void Player::react(Actor& other)
{
	//敵との判定
	if (other.tag() == "EnemyTag")
	{
		collide_actor(other);
		return;
	}
	//回避中なら何もしない
	if (state_ == State::Dodge)return;
	//ダメージ中なら何もしない
	if (state_ == State::Damage)return;
	//死亡中なら何もしない
	if (state_ == State::End)return;

	//敵の攻撃が当たったら
	if (other.tag() == "EnemyAttackTag")
	{
		//ガード可能か？
		if (can_guard())
		{
			// ノックバックする
			knock_back(other, 0.2f);
			//SEを鳴らす
			gsPlaySE(Se_PlayerBlock);
			//ガードのスタミナ消費
			ST_.consumption_stamina(GuradStamin);
			change_state(State::Guarding, MotionGuardAccept, false);
			return;
		}
		//エフェクを出す
		const GSvector3 Offset{ 0.0f,0.5f,0.0f };
		const GSvector3 Pos = transform_.position() + Offset;
		gsPlayEffect(Effect_Blood, &Pos);
		//SEを鳴らす
		gsPlaySE(Se_PlayerDamage);
		//ノックバック
		knock_back(other, 0.3f);
		//当たり判定を無効
		enable_collider_ = false;
		//コンボのリッセト
		combo_ = 0;
		change_state(State::Damage, MotionDamage, false);
		//ダメージ
		HP_.hit_damage(Damage);
	}
	//死亡
	if (HP_.is_end())
	{
		change_state(State::End, MotionEnd, false);
		//ゲームオーバーを呼び出す
		world_->game_over();
	}
}

//状態の更新
void Player::update_state(float delta_time) {
	//
	switch (state_) {
	case Player::State::Move: move(delta_time);    break;
	case Player::State::Attack:attack(delta_time); break;
	case Player::State::Dodge: dodge(delta_time); break;
	case Player::State::GuardStart: guard_start(delta_time); break;
	case Player::State::Guarding: guarding(delta_time); break;
	case Player::State::Damage:damage(delta_time); break;
	case Player::State::End: end(delta_time); break;
	}
	//
	state_timer_ += delta_time;
}

//状態の変更
void Player::change_state(State state, GSuint motion, bool loop) {
	motion_ = motion;
	motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.0f;
}

//移動
void Player::move(float delta_time)
{
	//回避ボタンを押したか、スタミナは足りるか
	if (Input::is_dodge() && ST_.get_stamina() > DodgeStamina) {
		//前方向に加速
		velocity_ = transform_.forward() * DodgeDistance;
		//スタミナ消費
		ST_.consumption_stamina(DodgeStamina);
		change_state(State::Dodge, MotionDodge, false);
		return;
	}

	if (Input::is_attack() && ST_.get_stamina() > AttackStamina) {
		ST_.consumption_stamina(AttackStamina);
		change_state(State::Attack, MotionAttack, false);
		return;
	}
	//ガード可能か？
	if (Input::is_guard()) {
		change_state(State::GuardStart, MotionGuardStart, false);
		return;
	}

	int motion = MotionIdle;
	//移動速度
	const float WalkSpeed{ 0.1f };
	//カメラの前方向を取得
	GSvector3 forward = world_->camera()->transform().forward();
	forward.y = 0.0f;
	//カメラの右方向を取得
	GSvector3 right = world_->camera()->transform().right();
	right.y = 0.0f;

	//移動量
	GSvector3 velocity{ 0.0f,0.0f,0.0f };

	if (Input::get_left_vertical() > Input)velocity += forward;
	if (Input::get_left_vertical() < -Input)velocity += -forward;
	if (Input::get_left_horizontal() > Input)velocity += right;
	if (Input::get_left_horizontal() < -Input)velocity += -right;
	//正規化して移動速度を掛ける
	velocity = velocity.normalize() * WalkSpeed * delta_time;

	//回転速度
	const float Rotate{ 30.0f };

	if (velocity_.length() != 0.0f)
	{
		GSquaternion rotation = GSquaternion::rotateTowards(transform_.rotation(),
			GSquaternion::lookRotation(velocity), Rotate * delta_time);
		transform_.rotation(rotation);
		// 移動中のモーションにする
		motion = MotionRun;
	}
	velocity_.x = velocity.x;
	velocity_.z = velocity.z;
	change_state(State::Move, motion);
	transform_.translate(velocity_, GStransform::Space::World);
}


//攻撃中
void Player::attack(float delta_time) {
	//回避ボタンを押しているか、スタミナは足りているか？
	if (Input::is_dodge() && ST_.get_stamina() > DodgeStamina) {
		turn();
		velocity_ = transform_.forward() * DodgeDistance;
		ST_.consumption_stamina(DodgeStamina);
		change_state(State::Dodge, MotionDodge, false);
		return;
	}
	//攻撃ボタンを押しているか、攻撃可能か？
	if (Input::is_attack() && can_attackable()) {
		turn();
		//スタミナ消費
		ST_.consumption_stamina(AttackStamina);
		//モーション番号
		int  motion = NULL;
		++combo_;
		switch (combo_) {
		case 1:
			motion = MotionAttack2;
			break;
		case 2:
			motion = MotionAttack3;
			break;
		}
		change_state(State::Attack, motion, false);
	}

	//モーション終了か？
	if (state_timer_ >= mesh_.motion_end_time()) {
		//コンボリセット
		combo_ = 0.0f;
		change_state(State::Move, MotionIdle, true);
	}

}
//回避中
void Player::dodge(float delta_time)
{
	transform_.translate(velocity_, GStransform::Space::World);
	//減速
	const float decrement_value{ 0.2f };
	velocity_ -= GSvector3{ velocity_.x,0.0f,velocity_.z }*decrement_value * delta_time;
	//モーション終了か？
	if (state_timer_ >= mesh_.motion_end_time() - 20.0f)
	{
		//コンボリッセト
		combo_ = 0;
		change_state(State::Move, MotionIdle, true);
	}
}
//ガードスタート
void Player::guard_start(float delta_time) {
	//ガードボタンが離されたら
	if (!gsXBoxPadButtonState(0, GS_XBOX_PAD_LEFT_SHOULDER)) {
		//移動中に遷移
		change_state(State::Move, MotionIdle);
	}
}

//ガード中
void Player::guarding(float delta_time) {
	//ノックバック減速
	velocity_ -= GSvector3{ velocity_.x, 0.0f, velocity_.z } *0.2f * delta_time;
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	//モーション終了か？
	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::GuardStart, MotionGuardLoop, true);
	}
}

//ダメージ中
void Player::damage(float delta_time) {
	//ノックバックする
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	//減速させる
	const float damp{ 0.4f };
	velocity_ -= GSvector3{ velocity_.x, 0.0f, velocity_.z } *damp * delta_time;
	//ダメージモーションの終了を待つ
	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::Move, MotionIdle, false);
		enable_collider_ = true;
		gsStopEffect(Effect_Blood);
	}
}


//死亡
void Player::end(float delta_time) {
	//何もしない
}

//振り向き
void Player::turn()
{
	//カメラの前方向を取得
	GSvector3 forwad = world_->camera()->transform().forward();
	forwad.y = 0.0f;
	//カメラの右方向を取得
	GSvector3 right = world_->camera()->transform().right();
	right.y = 0.0f;
	//移動量
	GSvector3 velocity{ 0.0f,0.0f,0.0f };
	if (Input::get_left_vertical() > Input)velocity += forwad;
	if (Input::get_left_vertical() < -Input)velocity += -forwad;
	if (Input::get_left_horizontal() > Input)velocity += right;
	if (Input::get_left_horizontal() < -Input)velocity += -right;
	//正規化
	velocity = velocity.normalize();
	//移動量が0じゃないなら
	if (velocity_.length() != 0.0f) {
		//回転させる
		GSquaternion rotation = GSquaternion::lookRotation(velocity);
		transform_.rotation(rotation);
	}
}


//攻撃可能か？
bool Player::can_attackable() const
{
	//現在のスタミナが消費より多いか？
	if (ST_.get_stamina() < AttackStamina)return false;
	//現在のコンボが最大コンボを超えているか？
	if (combo_ >= MaxCombo) return false;
	const float NextAttackTime{ 40.0f };
	//現在のタイマーが次の攻撃時間を超えているか？
	if (state_timer_ <= mesh_.motion_end_time() - NextAttackTime)return false;
	return true;
}
//ガード可能か？
bool Player::can_guard() const
{
	//現在のスタミナが消費より多いか？
	if (ST_.get_stamina() <= GuradStamin)return false;
	//ガード状態以外か？
	if (state_ != State::GuardStart)return false;
	//
	return true;
}
//死亡ラインを超えてるか？
bool Player::end_line()const {
	if (transform_.position().y >= EndtLinePos_Y)return false;
	return true;
}
//ノックバック
void Player::knock_back(Actor& other, float power) {
	//ターゲット方向のベクトルを求める
	GSvector3 to_target = other.transform().position() - transform().position();
	//y成分は無効にする
	to_target.y = 0.0f;
	//ターゲット方向と逆方向にノックバックする移動量を求める
	velocity_ = to_target.getNormalized() * power;
}
//フィールド当たり判定
void Player::collide_field() {
	//
	GSvector3 center; // 衝突後の球体の中心座標
	if (world_->field()->collide(collider(), &center)) {
		// y座標は変更しない
		center.y = transform_.position().y;
		// 補正後の座標に変更する
		transform_.position(center);
	}

	// 地面との衝突判定（線分との交差判定）
	GSvector3 position = transform_.position();
	Line line;
	line.start = position + collider_.center_;
	line.end = position + GSvector3{ 0.0f, -FootOffset, 0.0f };
	GSvector3 intersect;  // 地面との交点
	if (world_->field()->collide(line, &intersect)) {
		// 交点の位置からy座標のみ補正する
		position.y = intersect.y;
		// 座標を変更する
		transform_.position(position);
		// 重力を初期化する
		velocity_.y = 0.0f;
	}
}
//アクター当たり判定
void Player::collide_actor(Actor& other) {
	//y座標を除く座標を求める
	GSvector3 position = collider().center_;
	GSvector3 target = other.collider().center_;
	//相手との距離
	float distance = GSvector3::distance(position, target);
	//衝突判定球の半径同士を加えた長さを求める
	float length = collider_.radius_ + other.collider().radius_;
	//衝突判定球の重なっている長さを求める
	float overlap = length - distance;
	//重なっている部分の半分の距離だけ離れる移動量を求める
	GSvector3 v = (position - target).getNormalized() * overlap;
	transform_.translate(v, GStransform::Space::World);
	//フィールドとの衝突判定
	collide_field();
}
//攻撃判定生成
void Player::generate_attac_collider() {
	// 攻撃判定を出現させる場所の距離
	const float AttackColliderDistance{ 1.8f };
	// 攻撃判定の半径
	const float AttackColliderRadius{ 1.3f };
	// 攻撃判定を出す場所の高さ
	const float AttackColliderHeight{ 1.0f };

	// 攻撃判定が有効になるまでの遅延時間
	const float AttackCollideDelay{ 5.0f };
	// 攻撃判定の寿命
	const float AttackCollideLifeSpan{ 5.0f };

	// 衝突判定を出現させる座標を求める（前方の位置）
	GSvector3 position = transform_.position() + transform_.forward() * AttackColliderDistance;
	// 高さの補正（足元からの高さ）
	position.y += AttackColliderHeight;
	// 衝突判定用の球を作成
	BoundingSphere collider{ AttackColliderRadius, position };
	// 衝突判定を出現させる
	world_->add_actor(std::make_unique<AttackCollider>(world_, collider,
		"PlayerAttackTag", "PlayerAttack", tag_, AttackCollideLifeSpan, AttackCollideDelay));
}