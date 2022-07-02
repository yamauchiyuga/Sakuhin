#ifndef WITCH_H_
#define WITCH_H_


#include "Enemy.h"
#include"../AnimatedMesh.h"
#include<memory>
class Witch : public Enemy{
public :
	enum class State{
		Idle,
		Run,
		Dead,
		Attack
	};

	//�R���X�g���N�^
	Witch(std::shared_ptr<IWorld> world, const GSvector3& position);
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
	//�A�C�h��
	void idle(float delta_time);
	//����
	void run(float delta_time);
	//���S
	void dead(float delta_time);
	//�U��
	void attack(float delta_time);
	//���̍U��
	void attack_selection();
	//�΋�
	void spit_fire();
	//���邩�H
	bool is_run()const;
	//�U���͈͂��H
	bool is_attack()const;
	
	//�A�j���[�V�������b�V��
	AnimatedMesh	mesh_;
	//���[�V�����ԍ�
	GSuint motion_;
	//���
	State state_;
	//�ǂɂԂ��������H
	bool hit_wall;
};



#endif // !



