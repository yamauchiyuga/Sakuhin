#ifndef PLAYER_H_
#define PLATER_H_

#include"../Actor.h"
#include"../AnimatedMesh.h"
#include"../../UI/HP.h"
#include"../../UI/ST.h"
#include"../../Timer.h"
#include<memory>

//�v���[���[�N���X
class Player :public Actor {
public:
	//�v���[���[���
	enum class State {
		Move,			
		Attack,		   
		Dodge,		   
		GuardStart,  
		Guarding,	  
		Damage,	 
		End		     
	};

	//�R���X�g���N�^
	Player(std::shared_ptr<IWorld> world, const GSvector3& position);
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
	//���
	void dodge(float delta_time);
	//�K�[�h�n�܂�
	void guard_start(float delta_time);
	//�U���q�b�g
	void guarding(float delta_time);
	//�_���[�W��
	void damage(float delta_time);
	//���S
	void end(float delta_time);
	//�m�b�N�o�b�N����
	void knock_back(Actor& other, float power);
	//�U���\���H
	bool can_attackable()const;
	//�K�[�h�\���H
	bool can_guard()const;
	//���S���C�����H
	bool end_line()const;
	//�A�N�^�[����
	void collide_actor(Actor& other);
	//�t�B�[���h����
	void collide_field();
	//�U�����萶��
	void generate_attack_collider();

private:
	//�A�j���[�V�������b�V��
	AnimatedMesh	mesh_;
	//���[�V�����ԍ�
	GSuint motion_;
	//�X�e�[�^�X
	HP HP_;
	ST ST_;
	//���
	State state_;
	//��ԃ^�C�}
	float state_timer_;
	float easing_time_{0.0f};
	//�R���{��
	int combo_;
	//���[�V�����̃��[�v�ݒ�
	bool motion_loop_;
	//���S���C��
	bool det_line_;
};
#endif