#include "Enemy.h"
#include"../../World/IWorld.h"
#include"../../Collision/Field.h"
#include"../../Collision/Line.h"
#include"../AttackCollider.h"


float Enemy::target_signed_angle()const {
	//ターゲットの方向ベクトルを求める
	GSvector3 to_target = player_->transform().position() - transform_.position();
	//前向き方向ベクトルを取得
	GSvector3 forward = transform_.forward();
	//ベクトルy成分を無効にする
	forward.y = 0.0f;
	to_target.y = 0.0f;
	//// 前向き方向のベクトルとターゲット方向のベクトルの角度差を求める
	return GSvector3::signedAngle(forward, to_target);
}

//ターゲット方向の角度を求める（符号なし）
float Enemy::target_angle()const {
	return std::abs(target_signed_angle());
}

//ターゲットとの距離を求める
float Enemy::target_distance() const {
	return (player_->transform().position() - transform_.position()).magnitude();
}

bool Enemy::dead()const {
	return is_dead_;
}

void Enemy::collide_actor(Actor& other) {
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

//フィールドとの衝突判定
void Enemy::collide_field() {
	//足元のオフセット
	const float FootOffset{ 0.2f };
	GSvector3 center; // 衝突後の球体の中心座標
	if (world_->field()->collide(collider(), &center)) {
		// y座標は変更しない
		center.y = transform_.position().y;
		// 補正後の座標に変更する
		transform_.position(center);
		hit_wall_ = true;
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

//攻撃判定の生成
void Enemy::generate_attack_collider(const float radius, const float distance, const float height, const float width, const float delay, const float life_span, const std::string& attack_name) {

	// 衝突判定を出現させる座標を求める（前方の位置）
	GSvector3 position = transform_.position() + (transform_.forward() * distance + transform_.right() * width);
	// 高さの補正（足元からの高さ）
	position.y += height;
	// 衝突判定用の球を作成
	BoundingSphere collider{ radius, position };
	// 衝突判定を出現させる
	world_->add_actor(std::make_shared<AttackCollider>( world_, collider,
		"EnemyAttackTag", attack_name, tag_, life_span, delay ));
}

// 壁にぶつかったか
bool Enemy::is_hit_wall()const {
	return hit_wall_;
}



