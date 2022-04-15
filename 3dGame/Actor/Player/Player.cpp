#include"Player.h"
#include"../AttackCollider.h"
#include "../../World/IWorld.h"
#include "../../Collision/Field.h"
#include "../../Collision/Line.h"
#include "../../Asset.h"

enum
{
	MotionIdle = 0,                 //    
	MotionWalk = 1,                   //
	MotionAttack = 17,                //
	MotionDamage = 14,                //
	MotionGetup = 16                  //
};
//
const float WalkSpeed{ 0.050f };
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
Player::Player(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Player,Mesh_Player, Mesh_Player, MotionIdle },
	motion_{ MotionIdle },
	motion_loop_{ true },
	state_{ State::Move },
	state_timer_{ 0.0f }{
	// ワールドを設定
	world_ = world;
	//
	name_ = "Player";
	//
	tag_ = "playerTag";
	//
	collider_ = BoundingSphere{ PlayerHeight,GSvector3{0.0f,PlayerRadius,0.0f} };
	// 座標の初期化
	transform_.position(position);
	// メッシュの変換行列を初期化
	mesh_.transform(transform_.localToWorldMatrix());

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

//
void Player::react(Actor& other) {

	if (other.tag() == "EnemyTag") {
		// ダメージ効果音を再生
		gsPlaySE(Se_PlayerDamage);
		change_state(State::Damage, MotionDamage, false);
		enable_collider_ = false;
	}
}

void Player::update_state(float delta_time) {
	//
	switch (state_) {
	case Player::State::Move: move(delta_time);    break;
	case Player::State::Attack:attack(delta_time); break;
	case Player::State::Damage:damage(delta_time); break;
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

	//
	if (gsGetKeyState(GKEY_UP)) {
		//
		transform_.translate(GSvector3{ 0.0f,0.0f,WalkSpeed * delta_time });
		//
		change_state(State::Move, MotionWalk);
	}
	else
	{
		change_state(State::Move, MotionIdle);
	}


	if (gsGetKeyTrigger(GKEY_SPACE))
	{
		// 攻撃効果音を再生
		gsPlaySE(Se_PlayerAttack);
		//
		generate_attac_collider();
		//
		change_state(State::Attack, MotionAttack);
	}

	if (gsGetKeyState(GKEY_LEFT)) {
		//
		transform_.rotate(0.0f, RotateSpeed, 0.0f);
	}
	if (gsGetKeyState(GKEY_RIGHT)) {
		//
		transform_.rotate(0.0f, -RotateSpeed, 0.0f);
	}


}

void Player::attack(float delta_time) {
	//
	if (state_timer_ >= mesh_.motion_end_time()) {
		move(delta_time);
	}

}

void Player::damage(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::Move, MotionIdle);
		enable_collider_ = true;
	}
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

	//
	GSvector3 pos = transform_.position();
	//
	pos.x = CLAMP(pos.x, Field::MinX, Field::MaxX);
	//
	pos.z = CLAMP(pos.z, Field::MinZ, Field::MaxZ);
	//
	transform_.position(pos);

}

void Player::generate_attac_collider() {
	// 攻撃判定を出現させる場所の距離
	const float AttackColliderDistance{ 1.5f };
	// 攻撃判定の半径
	const float AttackColliderRadius{ 0.8f };
	// 攻撃判定を出す場所の高さ
	const float AttackColliderHeight{ 1.0f };

	// 攻撃判定が有効になるまでの遅延時間
	const float AttackCollideDelay{ 10.0f };
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