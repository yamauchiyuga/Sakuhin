#include"CameraTPS.h"
#include"../World/IWorld.h"
#include"../Collision/Field.h"
#include"../Collision/Line.h"
#include"../Actor/Enemy/Enemy.h"
#include"../Input.h"
#include<vector>
#include<algorithm>
#include<utility>
#include<string>

enum  State
{
	PlayerLockOn,
	EnemyLockOn,
	LookAtPlayerFromEnemy
};

//カメラの注視点の補完
const GSvector3 ReferencePointOffset{ 0.0f, 2.0f, 0.0f };
//プレイヤーからの相対座標
const GSvector3 PlayerOffset{ 0.0f, 2.0f, -6.5f };

//コンストラクタ
CameraTPS::CameraTPS(std::shared_ptr<IWorld> world, const GSvector3& position, const GSvector3& at) :
	state_{ State::PlayerLockOn }
{
	world_ = world;
	// タグの設定
	tag_ = "CameraTag";
	// 名前の設定
	name_ = "Camera";
	// 視点の位置を設定
	transform_.position(position);
	//
	target_ = GSvector3::zero();
	// 注視点を設定（注視点の方向に向きを変える）
	transform_.lookAt(at);
	//x軸回りの回転角度の初期化
	pitch_ = 0;
	//y軸回りの回転角度の初期化
	yaw_ = 0;
	//
	timer_=0;
}

//更新
void CameraTPS::update(float delta_time) 
{
	if (Input::is_lock_on()) 
	{
		state_update(delta_time);
	}
	OutputDebugString(std::to_string(state_).c_str());
	switch (state_)
	{
	case State::PlayerLockOn:player_lock_on(delta_time); break;
	case State::EnemyLockOn:enemy_lock_on(delta_time); break;
	case State::LookAtPlayerFromEnemy:look_at_player_from_enemy(delta_time); break;
	}
}

void CameraTPS::state_update(float delta_time)
{
	
	if (state_ == State::LookAtPlayerFromEnemy) return;
	if (state_ == State::EnemyLockOn) 
	{
		state_++;
	}
	else if(decide_targe())
	{
		state_++;
	}
	return;
}

void CameraTPS::player_lock_on(float delta_time) 
{
	std::shared_ptr<Actor> Player = world_->find_actor("Player");
	//プレーヤーが見つからなかったらそのまま返す
	if (!Player) return;
	//回転スピード
	const float RotateSpeed{ 3.5f };
	///y軸回りに回転させる
	yaw_ += Input::get_right_horizontal() * -RotateSpeed * delta_time;
	//x軸回りに回転させる
	pitch_ += Input::get_right_vertical() * -RotateSpeed * delta_time;

	//x軸の最低角度
	const float MinAngle{ -15.0f };
	//x軸の最大角度
	const float MaxAngle{ 25.0f };
	//x軸回りの回転を制限する
	pitch_ = CLAMP(pitch_, MinAngle, MaxAngle);


	//注視点の座標を求める
	GSvector3 at = Player->transform().position() + ReferencePointOffset;
	//カメラの座標を求める
	GSvector3 position = at + GSquaternion::euler(pitch_, yaw_, 0.0f) * PlayerOffset;

	const float SmoothTime{ 13.0f };    // 補間フレーム数
	const float MaxSpeed{ 1.0f };       // 移動スピードの最大値
	position = GSvector3::smoothDamp(transform_.position(), position, velocity_, SmoothTime, MaxSpeed, delta_time);

	//フィールドとの衝突判定
	Line line{ at,position };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) 
	{
		position = intersect;
	}

	//視点の位置の設定
	transform_.position(position);
	//注視点を設定(注視点の方向に向きを変える)
	transform_.lookAt(at);
}

void CameraTPS::enemy_lock_on(float delta_time) 
{
	GSvector3 at = GSvector3::zero();
	GSvector3 Position = GSvector3::zero();
	std::shared_ptr<Actor> Player = world_->find_actor("Player");

	const float Distance{ 6.5f };
	const float CamerHeightPos{ 3.5f };

	GSvector3 enemy_to_player = Player->transform().position() - target_;
	enemy_to_player.normalize();
	enemy_to_player = enemy_to_player * Distance;
	enemy_to_player.y = CamerHeightPos;

	//注視点の位置を求める
	at = target_ + ReferencePointOffset;
	//視点位置を求める(プレーヤーの背後の座標)
	Position = Player->transform().position() + enemy_to_player;

	const float smoothtime{ 30.0f };
	const float maxspeed{ 1.0f };

	Position = GSvector3::smoothDamp(transform_.position(), Position, velocity_, smoothtime, maxspeed, delta_time);

	//フィールドとの衝突判定
	Line line{ Player->transform().position() + ReferencePointOffset,Position };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect))
	{
		Position = intersect;
	}
	//視点の位置の設定
	transform_.position(Position);
	//注視点を設定(注視点の方向に向きを変える)
	transform_.lookAt(at);
}

void CameraTPS::look_at_player_from_enemy(float delta_time) {
	GSvector3 at = GSvector3::zero();
	GSvector3 Position = GSvector3::zero();
	std::shared_ptr<Actor> Player = world_->find_actor("Player");
	//注視点の座標を求める
	at = Player->transform().position() + ReferencePointOffset;
	//カメラの座標を求める
	Position = at + GSquaternion::euler(pitch_, yaw_, 0.0f) * PlayerOffset;
	const float smoothtime{ 12.0f };
	const float maxspeed{ 1.0f };
	Position = GSvector3::smoothDamp(transform_.position(), Position, velocity_, smoothtime, maxspeed, delta_time);
	timer_ +=delta_time;
	//視点の位置の設定
	transform_.position(Position);
	//注視点を設定(注視点の方向に向きを変える)
	transform_.lookAt(at);
	if (timer_ > smoothtime) {
		state_ = State::PlayerLockOn;
		timer_ = 0;
	}
}



bool CameraTPS::decide_targe() 
{
	std::shared_ptr<Actor> Player = world_->find_actor("Player");
	std::vector<std::shared_ptr<Actor>> enemy = world_->find_actor_with_tag("EnemyTag");
	std::vector<std::pair<float, std::shared_ptr<Actor>>> min_distance;
	bool is_dragon = false;
	if (enemy.size() == 0) 
	{
		return false;
	}
	for (const auto& e : enemy) 
	{

		if (e->name() == "Dragon") 
		{
			target_ = e->transform().position();
			is_dragon = true;
		}
		else if (e->name() == "Skeketon" || e->name() == "Witch") 
		{
			GSvector3 e_pos = e->transform().position();
			GSvector3 p_pos = Player->transform().position();
			min_distance.push_back(std::make_pair(e_pos.distance(p_pos), e));
		}
	}
	std::sort(min_distance.begin(), min_distance.end());
	if (!is_dragon) 
	{
		target_ = min_distance[0].second->transform().position();
	}
	return true;
}


//描画
void CameraTPS::draw()const 
{
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

