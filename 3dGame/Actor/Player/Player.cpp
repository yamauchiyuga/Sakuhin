#include"Player.h"
#include"../AttackCollider.h"
#include "../../World/IWorld.h"
#include "../../Collision/Field.h"
#include "../../Collision/Line.h"
#include "../../Asset.h"
#include"../../Input.h"

enum {
	MotionAttack,
	MotionAttack2,
	MotionAttack3,
	MotionDamage,
	MotionEnd,
	MotionDodge,
	MotionIdle,
	MotionRun
};
//
const GSvector2 HP_pos{ 80.0f, 70.0f };
const GSvector2 frame_pos{ 78.0f, 68.0f };
const float gauge_length{ 250.0f };
const GScolor color{ 0,1,0,1 };
//
const float RotateSpeed{ 1.0f };
//
const float PlayerHeight{ 0.5f };
//
const float PlayerRadius{ 0.85f };
//
const float FootOffset{ 0.1f };
//
const float Gravity{ -0.016f };
//
const int DodgeStamina{ 20 };
//
const float DodgeDistance{ 1.0f };
//
const int AttackStamina{ 10 };
//
const int MaxCombo{ 2 };
//
const float MaxHP{ 100.0f };
//
const float MaxST{ 100.0f };
//
const float Input{ 0.2f };

//
Player::Player(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Player,Mesh_Player, Mesh_Player, MotionIdle ,true },
	motion_{ MotionIdle },
	motion_loop_{ true },
	state_{ State::Move },
	state_timer_{ 0.0f },
	combo_{ 0 },
	HP_{ MaxHP ,HP_pos ,frame_pos,gauge_length,color },
	ST_{ MaxST }{
	// ワールドを設定
	world_ = world;
	//
	name_ = "Player";
	//
	tag_ = "PlayerTag";
	//
	collider_ = BoundingSphere{ PlayerHeight,GSvector3{0.0f,PlayerRadius,0.0f} };
	// 座標の初期化
	transform_.position(position);
	// メッシュの変換行列を初期化
	mesh_.transform(transform_.localToWorldMatrix());

	mesh_.add_animation_event(MotionAttack, 15.0f, [=] {generate_attac_collider(); });
	mesh_.add_animation_event(MotionAttack2, 15.0f, [=] {generate_attac_collider(); });
	mesh_.add_animation_event(MotionAttack3, 22.0f, [=] {generate_attac_collider(); });

}

//
void Player::update(float delta_time) {
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

	ST_.update(delta_time);
}

//
void Player::draw()const {
	//
	mesh_.draw();

	//
	if (enable_collider_) {
		//
		collider().draw();
	}
}

void Player::draw_gui()const {
	//
	HP_.draw();
	//
	ST_.draw();
}

//
void Player::react(Actor& other) {

	if (other.tag() == "EnemyTag") {
		//
		collide_actor(other);
	}
	//回避中なら何もしない
	if (state_ == State::Dodge)return;
	//
	if (state_ == State::Damage)return;
	//死亡中なら何もしない
	if (state_ == State::End)return;

	if (other.tag() == "EnemyAttackTag") {

		enable_collider_ = false;
		change_state(State::Damage, MotionDamage, false);
		//
		HP_.hit_damage(10);

		if (HP_.is_end()) {
			change_state(State::End, MotionEnd, false);
		}
	}
}

void Player::update_state(float delta_time) {
	//
	switch (state_) {
	case Player::State::Move: move(delta_time);    break;
	case Player::State::Attack:attack(delta_time); break;
	case Player::State::Dodge: dodge(delta_time); break;
	case Player::State::Damage:damage(delta_time); break;
	case Player::State::End: end(delta_time); break;
	}
	//
	state_timer_ += delta_time;
}

void Player::change_state(State state, GSuint motion, bool loop) {
	motion_ = motion;
	motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.0f;
}

void Player::move(float delta_time) {


	if (Input::is_dodge() && ST_.get_stamina() > DodgeStamina) {
		velocity_ = transform_.forward() * DodgeDistance;
		ST_.consumption_stamina(DodgeStamina);
		change_state(State::Dodge, MotionDodge, false);
		return;
	}

	if (Input::is_attack() && ST_.get_stamina() > AttackStamina) {
		ST_.consumption_stamina(AttackStamina);
		change_state(State::Attack, MotionAttack, false);
		return;
	}

	int motion = MotionIdle;

	const float WalkSpeed{ 0.15f };
	//
	GSvector3 forwad = world_->camera()->transform().forward();
	forwad.y = 0.0f;
	//
	GSvector3 right = world_->camera()->transform().right();
	right.y = 0.0f;
	//
	GSvector3 velocity{ 0.0f,0.0f,0.0f };

	if (Input::get_left_vertical() > Input)velocity += forwad;
	if (Input::get_left_vertical() < -Input)velocity += -forwad;
	if (Input::get_left_horizontal() > Input)velocity += right;
	if (Input::get_left_horizontal() < -Input)velocity += -right;
	velocity = velocity.normalize() * WalkSpeed * delta_time;

	const float Rotate{ 15.0f };

	if (velocity_.length() != 0.0f) {
		//
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



void Player::attack(float delta_time) {

	if (Input::is_dodge() && ST_.get_stamina() > DodgeStamina) {
		turn();
		velocity_ = transform_.forward() * DodgeDistance;
		ST_.consumption_stamina(DodgeStamina);
		change_state(State::Dodge, MotionDodge, false);
		return;
	}

	if (Input::is_attack() && can_attackable()) {
		turn();
		ST_.consumption_stamina(AttackStamina);
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
	//
	if (state_timer_ >= mesh_.motion_end_time()) {
		combo_ = 0;
		move(delta_time);
	}

}

void Player::dodge(float delta_time) {
	transform_.translate(velocity_, GStransform::Space::World);
	//
	const float decrement_value{ 0.2f };
	velocity_ -= GSvector3{ velocity_.x,0.0f,velocity_.z }*decrement_value * delta_time;
	//
	if (state_timer_ >= mesh_.motion_end_time()) {
		combo_ = 0;
		move(delta_time);
	}
}

void Player::damage(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::Move, MotionIdle, false);
		enable_collider_ = true;
	}
}


void Player::end(float delta_time) {

}

void Player::turn()
{
	GSvector3 forwad = world_->camera()->transform().forward();
	forwad.y = 0.0f;
	//
	GSvector3 right = world_->camera()->transform().right();
	right.y = 0.0f;
	//
	GSvector3 velocity{ 0.0f,0.0f,0.0f };

	if (Input::get_left_vertical() > Input)velocity += forwad;
	if (Input::get_left_vertical() < -Input)velocity += -forwad;
	if (Input::get_left_horizontal() > Input)velocity += right;
	if (Input::get_left_horizontal() < -Input)velocity += -right;
	velocity = velocity.normalize();

	if (velocity_.length() != 0.0f) {
		GSquaternion rotation = GSquaternion::lookRotation(velocity);
		transform_.rotation(rotation);
	}
}


//
bool Player::can_attackable() const
{
	if (ST_.get_stamina() < AttackStamina)return false;
	if (combo_ >= MaxCombo) return false;
	const float NextAttackTime{ 50.0f };
	if (state_timer_ <= mesh_.motion_end_time() - NextAttackTime)return false;
	return true;
}


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
	line.start = position + collider_.center;
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

void Player::collide_actor(Actor& other) {
	// ｙ座標を除く座標を求める
	GSvector3 position = transform_.position();
	position.y = 0.0f;
	GSvector3 target = other.transform().position();
	target.y = 0.0f;
	// 相手との距離
	float distance = GSvector3::distance(position, target);
	// 衝突判定球の半径同士を加えた長さを求める
	float length = collider_.radius + other.collider().radius;
	// 衝突判定球の重なっている長さを求める
	float overlap = length - distance;
	// 重なっている部分の半分の距離だけ離れる
	GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	transform_.translate(v, GStransform::Space::World);
	// フィールドとの衝突判定
	collide_field();
}

void Player::generate_attac_collider() {
	// 攻撃判定を出現させる場所の距離
	const float AttackColliderDistance{ 1.8f };
	// 攻撃判定の半径
	const float AttackColliderRadius{ 0.8f };
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
	world_->add_actor(new AttackCollider{ world_, collider,
		"PlayerAttackTag", "PlayerAttack", tag_, AttackCollideLifeSpan, AttackCollideDelay });
}