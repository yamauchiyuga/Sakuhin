#include"CameraTPS.h"
#include"../World/IWorld.h"
#include"../Collision/Field.h"
#include"../Collision/Line.h"
#include"../Input.h"

//カメラの注視点の補完
const GSvector3 ReferencePointOffset{ 0.0f, 2.0f, 0.0f };

//コンストラクタ
CameraTPS::CameraTPS(IWorld* world, const GSvector3& position, const GSvector3& at) :
	state_{ State::PlayerLockOn } {
	world_ = world;
	// タグの設定
	tag_ = "CameraTag";
	// 名前の設定
	name_ = "Camera";
	// 視点の位置を設定
	transform_.position(position);
	// 注視点を設定（注視点の方向に向きを変える）
	transform_.lookAt(at);
	///x軸回りの回転角度の初期化
	pitch_ = (at - position).getPitch();
	///y軸回りの回転角度の初期化
	yaw_ = (at - position).getYaw();
}

//更新
void CameraTPS::update(float delta_time) {
	switch (state_)
	{
	case CameraTPS::State::PlayerLockOn:player_lock_on(delta_time); break;
	case CameraTPS::State::EnemyLockOn:enemy_lock_on(delta_time); break;
	}
}

void CameraTPS::player_lock_on(float delta_time) {
	//プレーヤー検索
	Actor* Player = world_->find_actor("Player");
	//プレーヤーが見つからなかったらそのまま返す
	if (Player == nullptr) return;

	//回転スピード
	const float RotateSpeed{ 3.0f };
	///y軸回りに回転させる
	yaw_ += Input::get_right_horizontal() * -RotateSpeed * delta_time;
	//x軸回りに回転させる
	pitch_ += Input::get_right_vertical() * -RotateSpeed * delta_time;

	//x軸の最低角度
	const float MinAngle{ -30.0f };
	//x軸の最大角度
	const float MaxAngle{ -10.0f };
	//x軸回りの回転を制限する
	pitch_ = CLAMP(pitch_, MinAngle, MaxAngle);
	//プレイヤーからの相対座標
	const GSvector3 PlayerOffset{ 0.0f, 2.0f, -7.0f };

	//注視点の位置を求める(プレーヤーの頭部の少し上あたりの座標)
	GSvector3 at = Player->transform().position() + ReferencePointOffset;
	//視点位置を求める(プレーヤーの背後の座標)
	GSvector3 position = at + GSquaternion::euler(pitch_, yaw_, 3.0f) * PlayerOffset;

	const float SmoothTime{ 13.0f };    // 補間フレーム数
	const float MaxSpeed{ 2.0f };       // 移動スピードの最大値
	position = GSvector3::smoothDamp(transform_.position(), position, velocity_,
		SmoothTime, MaxSpeed, delta_time);


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

	if (Input::is_lock_on()) {
		state_ = State::EnemyLockOn;
	}
}

void CameraTPS::enemy_lock_on(float delta_time) {
	//
	Actor* Player = world_->find_actor("Player");
	//
	Actor* Enemy = world_->find_actor("Enemy");

	if (Enemy == nullptr)return;

		//回転スピード
		const float RotateSpeed{ 2.0f };
	const float Distance{ 6.0f };
	const float CamerHeightPos{ 2.0f };

	GSvector3 enemy_to_player = Player->transform().position() - Enemy->transform().position();
	enemy_to_player.normalize();
	enemy_to_player = enemy_to_player * Distance;
	enemy_to_player.y = CamerHeightPos;

	//注視点の位置を求める(プレーヤーの頭部の少し上あたりの座標)
	GSvector3 at = Enemy->transform().position() + ReferencePointOffset;
	//視点位置を求める(プレーヤーの背後の座標)
	GSvector3 position = Player->transform().position() + enemy_to_player;

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

	if (Input::is_lock_on()) {
		yaw_ = transform_.eulerAngles().y;
		state_ = State::PlayerLockOn;
	}


}

//描画
void CameraTPS::draw()const {
	//視点の位置
	GSvector3 eye = transform_.position();
	//注視点の位置
	GSvector3 at = eye + transform_.forward();
	//
	GSvector3 up = transform_.up();
	// カメラの設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,     // 視点の位置
		at.x, at.y, at.z,      // 注視点の位置
		up.x, up.y, up.z       // 視点の上方向
	);

}