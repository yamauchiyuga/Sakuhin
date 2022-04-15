#include"AttackCollider.h"

//コンストラクタ
AttackCollider::AttackCollider(IWorld* world, const BoundingSphere& collider,
	const std::string& tag, const std::string& name,
	const std::string& ownwr_tag,
	float lifespan, float delay) :
	lifespan_timer_{ lifespan }, delay_timer_{ delay }{
	//ワールド設定
	world_ = world;
	//タグ名の設定
	tag_ = tag;
	//生存者のタグ名
	owner_tag_ = ownwr_tag;
	//名前の設定
	name_ = name;

	//衝突判定の初期化
	collider_ = BoundingSphere{ collider.radius };
	//座標の初期化
	transform_.position(collider.center);
	//衝突判定を初期化
	enable_collider_ = false;
}

//更新
void AttackCollider::update(float delta_time) {
	//遅延時間が経過したら衝突判定を有効にする
	if (delay_timer_ <= 0) {
		//衝突判定を有効
		enable_collider_ = true;

		//寿命が尽きたら死亡
		if (lifespan_timer_ <= 0) {
			die();
		}
		//寿命の更新
		lifespan_timer_ -= delta_time;
	}
	//遅延時間の更新
	delay_timer_ -= delta_time;
}

void AttackCollider::draw()const{
	//衝突判定用のデバッグ表示
	collider().draw();
}

//衝突リアクション
void AttackCollider::react(Actor& ohter) {
	// 自分と同じタグまたは、オーナーと同じタグは衝突判定しない
	if (ohter.tag() == tag() || ohter.tag() == owner_tag_)return;
	//衝突したら死亡
	die();
}