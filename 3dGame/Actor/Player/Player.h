#ifndef PLAYER_H_
#define PLATER_H_

#include"../Actor.h"
#include"../AnimatedMesh.h"
#include"../../UI/HP.h"
#include"../../UI/ST.h"

//�v���[���[�N���X
class Player :public Actor {

public:
	//�v���[���[���
	enum class State {
		Move,			//�ړ���
		Attack,			//�U����
		Dodge,			//���
		GuardStart,		//�K�[�h�n�܂�
		Guarding,		//�K�[�h��
		GuardEnd,		//�K�[�h�I���
		Damage,			//�_���[�W��
		End				//���S��
	};

	//�R���X�g���N�^
	Player(IWorld* world, const GSvector3& position = GSvector3{ 0.0f, 0.0f, 0.0f });
	//�X�V
	virtual void update(float delta_time)override;
	//�`��
	virtual void draw()const override;
	//
	virtual void draw_gui()const override;
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
	//
	void dodge(float delta_time);
	//
	void guard_start(float delta_time);
	//
	void guarding(float delta_time);
	//
	void guard_end(float delta_time);

	//�_���[�W��
	void damage(float delta_time);
	//
	void end(float delta_time);
	//
	void turn();
	//
	bool can_attackable()const;
	//
	bool can_guard()const;
	//
	void knock_back(Actor& other, float power);
	//
	void collide_actor(Actor& other);
	//
	void collide_field();
	//
	void generate_attac_collider();

private:
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
	//
	int combo_;
	//
	HP HP_;
	//
	ST ST_;
};
#endif