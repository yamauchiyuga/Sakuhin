#include "TitlePlayer.h"
#include"../../Assets.h"
#include "../../World/IWorld.h"
#include "../../Collision/Field.h"
#include "../../Collision/Line.h"
#include"../Enemy/TitleDragon.h"

enum
{
	MotionIdle = 5
};

//補完
constexpr float FootOffset{ 0.1f };
//重力
constexpr float Gravity{ -0.016f };

TitlePlayer::TitlePlayer(std::shared_ptr<IWorld> world, const GSvector3& position) :
	mesh_{ Mesh_TitlePlayer,Mesh_TitlePlayer, Mesh_TitlePlayer, MotionIdle ,true },
	motion_{ MotionIdle },
	motion_loop_{ true }{
	// ワールドを設定
	world_ = world;
	//名前
	name_ = "TitlePlayer";
	//タグ
	tag_ = "TitlePlayerTag";
	// 座標の初期化
	transform_.position(position);
	// メッシュの変換行列を初期化
	mesh_.transform(transform_.localToWorldMatrix());
	std::shared_ptr<Actor> enemy = world_->find_actor("TitleDragon");
	transform_.lookAt(enemy->transform().position());
};


void TitlePlayer::update(float delta_time)
{
	//重力加算
	//velocity_.y += Gravity * delta_time;
	//transform_.translate(0.0f, velocity_.y, 0.0f);
	//フィールドの当たり判定
	collide_field();
	//状態の変更
	mesh_.change_motion(motion_, motion_loop_);
	//アニメーション更新
	mesh_.update(delta_time);
	// メッシュの変換行列を更新
	mesh_.transform(transform_.localToWorldMatrix());
}

void TitlePlayer::draw() const
{
	mesh_.draw();
}

//フィールド当たり判定
void TitlePlayer::collide_field() {
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