#ifndef SKELETON_H_
#define SKELETON_H

#include"Enemy.h"
#include"../AnimatedMesh.h"
#include<memory>

class Skeleton :public Enemy 
{
public:
	//���
	enum  class State
	{
		Generation,
		Idle,
		Run,
		Turn,
		Dead,
		ShieldBlock,
		Damage,
		Attack
	};
	//�R���X�g���N�^
	Skeleton(std::shared_ptr<IWorld> world, const GSvector3& position);
	//�X�V
	virtual void update(float delta_time)override;
	//�`��
	virtual void draw()const override;
	//�Փ˔���
	virtual void react(Actor& other)override;
private:
	//��Ԃ̍X�V
	void update_state(float delta_time);
	//��Ԃ̕ύX
	void change_state(State state, int motion, bool loop = true);
	//����
	void generation(float delta_time);
	//�A�C�h��
	void idle(float delta_time);
	//����
	void run(float delta_time);
	//�U�������
	void turn(float delta_time);
	//
	void Shield_Block(float delta_time);
	//���S
	void dead(float delta_time);
	//�U����
	void attack(float delta_time);
	//
	void damage(float delta_time);
	//�U��
	void slash();
	//���邩�H
	bool is_run()const;
	//�U��������H
	bool is_trun()const;
	//�U���͈͂��H
	bool is_attack()const;

	//�A�j���[�V�������b�V��
	AnimatedMesh	mesh_;
	//���[�V�����ԍ�
	GSuint motion_;
	//���
	State state_;
};

#endif // !SKELETON_H_

