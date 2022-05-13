#include"FireSphere.h"
#include"../../World/IWorld.h"
#include"../../Collision/Field.h"
#include"../../Collision/Line.h"

FireSphere::FireSphere(IWorld* world, const GSvector3& position, const GSvector3& velocity) {
	// ワールドを設定
	world_ = world;
	// タグ名
	tag_ = "EnemyAttackTag";
	// アクター名
	name_ = "EnemyAttack";
	// 移動量の初期化
	velocity_ = velocity;
	// 衝突判定用の球体を設定
	collider_ = BoundingSphere{ 1.0f };
	// 座標の初期化
	transform_.position(position);
	// 寿命
	lifespan_timer_ = 120.0f;

}

void FireSphere::update(float delta_time) {
	// 寿命が尽きたら死亡
	if (lifespan_timer_ <= 0.0f) {
		die();
		return;
	}
	// 寿命の更新
	lifespan_timer_ -= delta_time;
	// フィールドとの衝突判定
	Line line;
	line.start = transform_.position();
	line.end = transform_.position() + velocity_;
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		// 交点の座標に補正
		transform_.position(intersect);
		// フィールドに衝突したら死亡
		die();
		return;
	}
	// 移動する（ワールド座標系基準）
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
}

void FireSphere::draw() const {
	// デバッグ表示
	collider().draw();
}

void FireSphere::react(Actor& other) {
	if (other.tag() == "PLayerTag") {
		// 衝突したら死亡
		die();
	}
}
