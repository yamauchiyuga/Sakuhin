#ifndef TITLE_PLAYER_H_
#define TITLE_PLAYER_H_

#include "../Actor.h"
#include "../AnimatedMesh.h"
#include<memory>

class TitlePlayer :public Actor
{
public:
	//
	TitlePlayer(std::shared_ptr<IWorld> world, const GSvector3& position);
	//
	virtual void update(float delta_time)override;
	//
	virtual void draw()const override;
	//�t�B�[���h����
	void collide_field();
private:
	//�A�j���[�V�����t�����b�V��
	AnimatedMesh mesh_;
	//���[�V�����ԍ�
	GSuint motion_;
	//���[�V�����̃��[�v�t���O
	bool motion_loop_;

};

#endif // !TITLE_PLAYER_H_
