#ifndef GAMEPLAYSCENE_H_
#define GAMEPLAYSCENE_H_

#include "IScene.h"
#include "../World/World.h"
#include"../EnemyGenerator/EnemyGenerator.h"
#include"../Fog/Fog.h"
#include<gslib.h>
#include<memory>

class GamePlayScene :public IScene {
public:
	//�X�^�[�g
	virtual void start()override;
	//�X�V
	virtual void update(float delta_time)override;
	//�`��
	virtual void draw()const override;
	//�I�����H
	virtual bool is_end()const override;
	//���̃V�[��
	virtual std::string next() const override;
	//�I��
	virtual void end()override;

private:
	//���[���h
	std::shared_ptr<World> world_;
	//�t�H�O
	Fog fog_;
	//�G�l�~�[����
	std::shared_ptr<EnemyGenerator> enemy_generator_;
	//�I�����ǂ����̃t���O
	bool is_end_{ false };
};


#endif
