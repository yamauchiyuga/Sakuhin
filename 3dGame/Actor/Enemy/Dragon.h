#ifndef DRAGON_H_
#define DRAGON_H_

#include"Enemy.h"
#include"../AnimatedMesh.h"
#include<string>
#include<memory>

class Dragon : public Enemy 
{
public:
	//���
	enum class State 
	{
		Idle,
		Run,
		Turn,
		FlyStart,
		FlyMove,
		FlyIdel,
		FlyAttack,
		FlyEnd,
		Damage,
		Dead,
		Attack
	};
	//�R���X�g���N�^
	Dragon(std::shared_ptr<IWorld> world, const GSvector3& position);
	//�X�V
	virtual void update(float delta_time)override;
	//�`��
	virtual void draw()const override;
	//GUI�`��
	virtual void draw_gui()const override;
	//�Փ˔���
	virtual void react(Actor& other)override;

private:
	//��Ԃ̍X�V
	void update_state(float delta_time);
	//��Ԃ̕ύX
	void change_state(State state, int motion, bool loop = true);
	//�A�C�h��
	void idle(float delta_time);
	//����
	void run(float delta_time);
	//�U�����
	void turn(float delta_time);
	//�U��
	void attack(float delta_time);
	//��s�n��
	void fly_start(float delta_time);
	//��s��
	void fly_idle(float delta_time);
	//��s�ړ�
	void fly_move(float delta_time);
	//��s�U��
	void fly_attack(float delta_time);
	//��s�I��
	void fly_end(float delta_time);
	//
	void damage(float delta_time);
	//���S
	void dead(float delta_time);

	//�U���̑I��
	void attack_selection();
	//���݂�
	void bite();
	//�K���U��
	void tail_attack();
	//�΋�
	void spit_fire();
	//�U��������H
	bool is_trun()const;
	//���邩�H
	bool is_run()const;
	//�U���͈͂��H
	bool is_attack()const;
	//�d�͂�L���ɂ��邩�H
	bool is_gravity()const;
private:
	//�A�j���[�V�������b�V��
	AnimatedMesh	mesh_;
	//���[�V�����ԍ�
	GSuint motion_;
	//���
	State state_;
};

#endif // !DRAGON_H_














