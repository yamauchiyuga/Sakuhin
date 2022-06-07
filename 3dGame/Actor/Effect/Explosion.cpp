#include "Explosion.h"
#include"../../World/IWorld.h"
#include"../../Collision/Field.h"
#include"../../Collision/Line.h"


const float delay_timer{ 20.0f };

Explosion::Explosion(IWorld* world, const GSvector3& position, const float delay_timer) {
	// ワールドを設定
	world_ = world;
	// タグ名
	tag_ = "EnemyAttackTag";
	// アクター名
	name_ = "EnemyAttack";
	// 衝突判定用の球体を設定
	collider_ = BoundingSphere{ 1.0f };
	// 座標の初期化
	transform_.position(position);
	// 寿命
	lifespan_timer_ = 10.0f;
	// 衝突判定を無効にする
	enable_collider_ = false;
	//
	delay_timer_ = delay_timer;
}

void Explosion::update(float delta_time) {
	if (delay_timer_ <= 0.0f) {
		// 衝突判定を有効にする
		enable_collider_ = true;
		// 寿命が尽きたら死亡
		if (lifespan_timer_ <= 0.0f) {
			die();
		}
		// 寿命の更新
		lifespan_timer_ -= delta_time;
	}
	// 遅延時間の更新
	delay_timer_ -= delta_time;
}

void Explosion::draw() const {
	// デバッグ表示
	collider().draw();
}

void Explosion::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		// 衝突したら死亡
		die();
	}
}
