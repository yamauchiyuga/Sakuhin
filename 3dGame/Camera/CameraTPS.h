#ifndef CAMERA_TPS_H_
#define CAMERA_TPS_H_

#include "../Actor/Actor.h"

class CameraTPS :public Actor {
public:
	//�R���X�g���N�^
	CameraTPS(IWorld* world, const GSvector3& position, const GSvector3& at);
	//�X�V
	virtual void update(float delta_time)override;
	//�`��
	virtual void draw()const override;
};

#endif // !CAMERA_TPS_H_
