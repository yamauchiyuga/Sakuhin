#include "TitleDragon.h"
#include"../Player/Player.h"
#include"../../World/IWorld.h"
#include"../../Assets.h"

enum
{
	MotionIdle=2
};

TitleDragon::TitleDragon(std::shared_ptr<IWorld> world, const GSvector3& position):
	mesh_{ Mesh_TitleDragon,Mesh_TitleDragon, Mesh_TitleDragon, MotionIdle },
	motion_{ MotionIdle }
{
	//ワールド設定
	world_ = world;
	//タグ
	tag_ = "EnemyTag";
	//名前
	name_ = "TitleDragon";
	player_ = world_->find_actor("Player");
	//座標の初期化
	transform_.position(position);
	// メッシュの変換行列を初期化
	mesh_.transform(transform_.localToWorldMatrix());
}


void TitleDragon::update(float delta_time)
{
	//フィールドの判定
	collide_field();
	//状態の変更
	mesh_.change_motion(motion_, motion_loop_);
	//アニメーション更新
	mesh_.update(delta_time);
	// メッシュの変換行列を更新
	mesh_.transform(transform_.localToWorldMatrix());
}

void TitleDragon::draw() const
{
	mesh_.draw();
}
