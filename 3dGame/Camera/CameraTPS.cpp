#include"CameraTPS.h"
#include"../World/IWorld.h"
#include"../Collision/Field.h"
#include"../Collision/Line.h"

//プレーヤーからの相対座標
const GSvector3 PlayerOffset{ 0.0f,2.0f,-4.0f };
//カメラの注視点の補完
const GSvector3 ReferencePointOffset{ 0.0f, 1.0f, 0.0f };

//コンストラクタ
CameraTPS::CameraTPS(IWorld* world, const GSvector3& position, const GSvector3& at) {
	world_ = world;
	// タグの設定
	tag_ = "CameraTag";
	// 名前の設定
	name_ = "Camera";
	// 視点の位置を設定
	transform_.position(position);
	// 注視点を設定（注視点の方向に向きを変える）
	transform_.lookAt(at);
}

//更新
void CameraTPS::update(float delta_timer) {
	//プレーヤー検索
	Actor* Player = world_->find_actor("Player");
	//プレーヤーが見つからなかったらそのまま返す
	if (Player == nullptr) return;
	//視点位置を求める(プレーヤーの背後の座標)
	GSvector3 position = PlayerOffset * Player->transform().localToWorldMatrix();
	//注視点の位置を求める(プレーヤーの頭部の少し上あたりの座標)
	GSvector3 at = Player->transform().position() + ReferencePointOffset;
	//フィールドとの衝突判定
	Line line{ at,position };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		position = intersect;
	}

	//視点の位置の設定
	transform_.position(position);
	//注視点を設定(注視点の方向に向きを変える)
	transform_.lookAt(at);
}

//描画
void CameraTPS::draw()const {
	//視点の位置
	GSvector3 eye = transform_.position();
	//注視点の位置
	GSvector3 at = eye + transform_.forward();
	// カメラの設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,     // 視点の位置
		at.x, at.y, at.z,      // 注視点の位置
		0.0f, 1.0f, 0.0f       // 視点の上方向
	);

}