#ifndef ATTACK_COLLIDER_H_
#define ATTACK_COLLIDER_H_

#include"Actor.h"

//�U������N���X
class AttackCollider :public Actor {
public:
	//�R���X�g���N�^
	AttackCollider(IWorld* world, const BoundingSphere& collider,
		const std::string& tag, const std::string& name,
		const std::string& ownwr_tag = "",
		float lifespan = 1.0f, float delay = 0.0f);

	//�X�V
	virtual void update(float delta_time)override;
	//�`��
	virtual void draw()const override;
	//�Փ˃��A�N�V����
	virtual void react(Actor& ohter)override;

private:
	//�U��������o���������A�N�^�[�̃^�O
	std::string owner_tag_;
	//����
	float lifespan_timer_;
	//�Փ˔��肪�s�����ɂȂ�܂ł̒x������
	float delay_timer_;
};

#endif // !ATTACK_COLLIDER_H_

