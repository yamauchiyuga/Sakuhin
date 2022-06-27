#ifndef CAMERA_TPS_H_
#define CAMERA_TPS_H_

#include "../Actor/Actor.h"
#include<memory>

class CameraTPS :public Actor 
{
public:

	//コンストラクタ
	CameraTPS(std::shared_ptr<IWorld> world, const GSvector3& position, const GSvector3& at);
	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;

private:
	//
	void state_update(float delta_time);
	//
	void player_lock_on(float delta_time);
	//
	void enemy_lock_on(float delta_time);
	//
	void look_at_player_from_enemy(float delta_taime);
	//
	bool decide_targe();
private:
	//
	int state_;
	//
	GSvector3 target_;
	//
	float timer_;
	//
	float yaw_{ 0.0f };
	//
	float pitch_{ 0.0f };


};

#endif // !CAMERA_TPS_H_
