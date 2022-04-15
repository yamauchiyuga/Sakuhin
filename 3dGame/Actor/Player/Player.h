#ifndef PLAYER_H_
#define PLATER_H_

#include"../Actor.h"
#include"../AnimatedMesh.h"

//�v���[���[�N���X
class Player :public Actor {

public:
	//�v���[���[���
	enum class State {
		Move,			//�ړ���
		Attack,			//�U����
		Damage			//�_���[�W��
	};

	//�R���X�g���N�^
	Player(IWorld* world, const GSvector3& position = GSvector3{ 0.0f, 0.0f, 0.0f });
	//�X�V
	virtual void update(float delta_time)override;
	//�`��
	virtual void draw()const override;
	//�Փ˃��A�N�V����
	virtual void react(Actor& other)override;

private:
	//��Ԃ̍X�V
	void update_state(float delta_time);
	//��Ԃ̕ύX
	void change_state(State state, GSuint motion, bool loop = true);
	//�ړ�����
	void move(float delta_time);
	//�U����
	void attack(float delta_time);
	//�_���[�W��
	void damage(float delta_time);

	//
	void collide_field();
	//
	void generate_attac_collider();

	//�A�j���[�V�������b�V��
	AnimatedMesh	mesh_;
	//���[�V�����ԍ�
	GSuint motion_;
	//���[�V�����̃��[�v�ݒ�
	bool motion_loop_;
	//���
	State state_;
	//��ԃ^�C�}
	float state_timer_;
};
#endif