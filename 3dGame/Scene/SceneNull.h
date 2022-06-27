#ifndef SCENE_NULL_H_
#define SCENE_NULL_H_

#include "IScene.h"


class SceneNull :public IScene 
{
public:
	//�J�n
	virtual void start()override {}
	//�X�V
	virtual void update(float delta_time)override {};
	//�`��
	virtual void draw()const override{}
	//�I�����Ă��邩
	virtual bool is_end()const override {
		return false;
	}
	//���̃V�[����Ԃ�
	virtual std::string next()const override
	{
		return "SceneNull";
	}
	//�I��
	virtual void end()override{}
};
#endif // !SCENE_NULL_H_

