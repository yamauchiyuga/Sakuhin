#ifndef CAMERA_TPS_H_
#define CAMERA_TPS_H_

#include "../Actor/Actor.h"

class CameraTPS :public Actor {
public:
	enum  class State
	{
		PlayerLockOn,
		EnemyLockOn
	};
	//�R���X�g���N�^
	CameraTPS(IWorld* world, const GSvector3& position, const GSvector3& at);
	//�X�V
	virtual void update(float delta_time)override;
	//�`��
	virtual void draw()const override;

private:
	void player_lock_on(float delta_time);
	//
	void enemy_lock_on(float delta_time);

private:
	//
	State state_;
	//
	float yaw_{ 0.0f };
	//
	float pitch_{ 0.0f };


};

#endif // !CAMERA_TPS_H_
