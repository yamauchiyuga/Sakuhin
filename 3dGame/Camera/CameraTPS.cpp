#include"CameraTPS.h"
#include"../World/IWorld.h"
#include"../Collision/Field.h"
#include"../Collision/Line.h"
#include"../Actor/Enemy/Enemy.h"
#include"../Assets.h"
#include"../Input.h"
#include<vector>
#include<algorithm>
#include<utility>


enum  State
{
	PlayerLockOn,
	EnemyLockOn,
	LookAtPlayerFromEnemy
};

//プレイヤーからの相対座標
const GSvector3 PlayerOffset{ 0.0f, 2.0f, -6.5f };

//コンストラクタ
CameraTPS::CameraTPS(std::shared_ptr<IWorld> world, const GSvector3& position, const GSvector3& at) :
	state_{ State::PlayerLockOn },
	player_{ nullptr },
	target_{ nullptr }
{
	world_ = world;
	// タグの設定
	tag_ = "CameraTag";
	// 名前の設定
	name_ = "Camera";
	// 視点の位置を設定
	transform_.position(position);

	// 注視点を設定（注視点の方向に向きを変える）
	transform_.lookAt(at);
	//x軸回りの回転角度の初期化
	pitch_ = 0;
	//y軸回りの回転角度の初期化
	yaw_ = 0;
	//
	timer_ = 0;
}

void CameraTPS::update(float delta_time)
{
	player_update();

	if (is_player_dead()) return;
	input_update();

	state_update(delta_time);
	field_react();
	set_parameter();
}
void CameraTPS::draw()const
{
	// 透視射影の変換行列を設定する
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		45.0f,
		1280.0f / 720.0f,
		0.3f,
		100.0f
	);


	// 視点の位置
	GSvector3 eye = transform_.position();
	// 注視点の位置
	GSvector3 at = eye + transform_.forward();
	// 上方向
	GSvector3 up = transform_.up();

	// カメラの設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,     // 視点の位置
		at.x, at.y, at.z,      // 注視点の位置
		up.x, up.y, up.z       // 視点の上方向
	);

	if (state_ == State::EnemyLockOn)
	{
		//ロックオンのテクスチャ描画
		const GSvector3 Offset{ 0.0f,1.0f,0.0f };
		GSvector3 Pos = target_->transform().position() + Offset;
		GSrect billboard_size{ -1.0f, 1.0f, 1.0f, -1.0f };  // ビルボードの大きさ
		gsDrawSprite3D(Texture_Lock, &Pos, &billboard_size, NULL, NULL, NULL, 0.0f);

	}
}
void CameraTPS::input_update()
{
	input_horizontal_amount_ = Input::get_right_horizontal();
	input_vertical_amount_ = Input::get_right_vertical();

	if (!Input::is_lock_on()) return;
	if (state_ == State::LookAtPlayerFromEnemy) return;
	if (state_ == State::EnemyLockOn) state_++;
	else if (decide_targe()) state_++;
}
void CameraTPS::state_update(float delta_time)
{
	switch (state_)
	{
	case State::PlayerLockOn:player_lock_on(delta_time); break;
	case State::EnemyLockOn:
		if (!enemy_lock_on(delta_time)) state_ = State::PlayerLockOn; break;
	case State::LookAtPlayerFromEnemy:
		if (lock_on_enemy_to_player(delta_time)) state_ = State::PlayerLockOn;
		break;
	default:break;
	}
}
void CameraTPS::player_update()
{
	std::shared_ptr<Actor> temp{ world_->find_actor("Player") };
	player_ = std::move(temp);
}
void CameraTPS::player_lock_on(float delta_time)
{
	operation_processor(delta_time);
	//注視点の座標を求める
	set_reference_point(player_->transform().position());
	//カメラの座標を求める
	position_ = at_ + GSquaternion::euler(pitch_, yaw_, 0.0f) * PlayerOffset;

	static const float SmoothTime{ 30.0f }, MaxSpeed{ 0.5f };
	GSvector3::smoothDamp(transform_.position(), position_, velocity_, SmoothTime, MaxSpeed, delta_time);
	linear_interpolation(SmoothTime, MaxSpeed, delta_time);
}
bool CameraTPS::enemy_lock_on(float delta_time)
{
	if (target_->is_dead()) return false;

	static const float Distance{ 6.5f };
	static const float CamerHeightPos{ 3.5f };

	GSvector3 EnemyToPlayer = player_->transform().position() - target_->transform().position();
	EnemyToPlayer = EnemyToPlayer.normalized() * Distance;
	EnemyToPlayer.y = CamerHeightPos;

	//注視点の位置を求める
	set_reference_point(target_->transform().position());
	//視点位置を求める(プレーヤーの背後の座標)
	position_ = player_->transform().position() + EnemyToPlayer;

	static const float SmoothTime{ 30.0f }, MaxSpeed{ 0.5f };
	linear_interpolation(SmoothTime, MaxSpeed, delta_time);

	return true;
}
bool CameraTPS::lock_on_enemy_to_player(float delta_time) {
	//注視点の座標を求める
	set_reference_point(player_->transform().position());
	//カメラの座標を求める
	position_ = at_ + GSquaternion::euler(pitch_, yaw_, 0.0f) * PlayerOffset;

	static const float SmoothTime{ 12.0f }, MaxSpeed{ 1.0f };
	linear_interpolation(SmoothTime, MaxSpeed, delta_time);

	timer_ += delta_time;

	if (timer_ >= SmoothTime) {
		timer_ = 0;
		return true;
	}
	return false;
}
bool CameraTPS::decide_targe()
{
	target_.reset();
	std::vector<std::shared_ptr<Actor>> enemies = world_->find_actor_with_tag("EnemyTag");
	if (enemies.empty()) return false;

	std::vector<std::pair<float, std::shared_ptr<Actor>>> min_distance;

	for (const auto& e : enemies)
	{
		if (e->name() == "Dragon") set_target(e);
		else
		{
			GSvector3 EPos = e->transform().position();
			GSvector3 PPos = player_->transform().position();
			min_distance.push_back(std::make_pair(EPos.distance(PPos), e));
		}
	}

	std::sort(min_distance.begin(), min_distance.end());
	if (!target_) set_target(min_distance.front().second);

	return true;
}
void CameraTPS::field_react()
{
	static const GSvector3 ReferencePointOffset{ 0.0, 2.0f, 0.0f };
	Line line{ player_->transform().position() + ReferencePointOffset, position_ };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect))
	{
		position_ = intersect;
	}
}
void CameraTPS::set_parameter()
{
	//視点の位置の設定
	transform_.position(position_);
	//注視点を設定(注視点の方向に向きを変える)
	transform_.lookAt(at_);
}
void CameraTPS::set_target(const std::shared_ptr<Actor>& target)
{
	std::shared_ptr<Actor> temp{ target };
	target_ = std::move(temp);
}
void CameraTPS::set_reference_point(const GSvector3& at)
{
	//カメラの注視点の補完
	static const GSvector3 ReferencePointOffset{ 0.0f, 2.0f, 0.0f };
	at_ = at + ReferencePointOffset;
}
void CameraTPS::operation_processor(float delta_time)
{
	//回転スピード
	static const float RotateSpeed{ 3.5f };
	///y軸回りに回転させる
	yaw_ += input_horizontal_amount_ * -RotateSpeed * delta_time;
	//x軸回りに回転させる
	pitch_ += input_vertical_amount_ * -RotateSpeed * delta_time;
	//x軸の角度範囲
	static const GSvector2 AngleRange{ -15.0f, 25.0f };
	//x軸回りの回転を制限する
	pitch_ = CLAMP(pitch_, AngleRange.x, AngleRange.y);
}
void CameraTPS::linear_interpolation(const float time, const float max_speed, float delta_time)
{
	position_ = GSvector3::smoothDamp(transform_.position(), position_, velocity_, time, max_speed, delta_time);
}
bool CameraTPS::is_player_dead() const
{
	return !(bool)player_;
}