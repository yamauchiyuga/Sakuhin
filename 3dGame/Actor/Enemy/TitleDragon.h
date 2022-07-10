#pragma once
#ifndef TITLE_DRAGON_H_
#define TITLE_DRAGON_H_

#include"Enemy.h"
#include"../AnimatedMesh.h"
#include<memory>

class TitleDragon:public Enemy
{
public:
	//
	TitleDragon(std::shared_ptr<IWorld> world, const GSvector3& position);
	//�X�V
	virtual void update(float delta_time)override;
	//�`��
	virtual void draw()const override;
private:
	//�A�j���[�V�������b�V��
	AnimatedMesh	mesh_;
	//���[�V�����ԍ�
	GSuint motion_;
};

#endif // !TITLE_DRAGON_H_
