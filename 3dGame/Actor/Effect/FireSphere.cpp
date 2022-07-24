#include"FireSphere.h"
#include"../../World/IWorld.h"
#include"../../Collision/Field.h"
#include"../../Collision/Line.h"
#include"../../Assets.h"
#include<GSeffect.h>

FireSphere::FireSphere(std::shared_ptr<IWorld> world, const GSvector3& position, const GSvector3& velocity,const std::string& attack_name) {
	// ワールドを設定
	world_ = world;
	// タグ名
	tag_ = "EnemyAttackTag";
	// アクター名
	name_ = attack_name;
	// 移動量の初期化
	velocity_ = velocity;
	// 衝突判定用の球体を設定
	collider_ = BoundingSphere{ 1.0f };
	// 座標の初期化
	transform_.position(position);
	 effect_=gsPlayEffect(Effect_FireBall, &position);
	gsPlaySE(Se_DragonSpitFire);
}

void FireSphere::update(float delta_time) {
	// フィールドとの衝突判定
	Line line;
	line.start = transform_.position();
	line.end = transform_.position() + velocity_;
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		// 交点の座標に補正
		transform_.position(intersect);
		die();
		// フィールドに衝突したら死亡
		GSvector3 position{ transform_.position() };
		gsStopEffect(effect_);
		effect_=gsPlayEffect(Effect_Explosion, &position);
		return;
	}

	// 移動する（ワールド座標系基準）
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
}

void FireSphere::draw() const {
	//エフェクトに自身のワールド変換行列を設定
	GSmatrix4 world = transform_.localToWorldMatrix();
	gsSetEffectMatrix(effect_, &world);
}

void FireSphere::react(Actor& other) {

	if (other.tag() == "PlayerTag") {
		// 衝突したら死亡
		die();
		GSvector3 position{ transform_.position() };
		gsPlaySE(Se_DragonExplosion);
		gsStopEffect(effect_);
		effect_= gsPlayEffect(Effect_Explosion, &position);
	}

}
