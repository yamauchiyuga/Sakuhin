#ifndef ENEMY_H_
#define ENEMY_H_
#include"../Actor.h"
#include"../../UI/HP.h"


//
class Enemy :public Actor {
public:
	Enemy() = default;
	virtual~Enemy() = default;

protected:
	// �^�[�Q�b�g�����̊p�x�����߂�i�����t���j
	float target_signed_angle() const;
	// �^�[�Q�b�g�����̊p�x�����߂�i�����Ȃ��j
	float target_angle() const;
	// �^�[�Q�b�g�̋��������߂�
	float target_distance() const;
	//�A�N�^�[�Ƃ̏Փ˃��A�N�V����
	void collide_actor(Actor& other);
	//�t�B�[���h�Ƃ̏Փ˃��A�N�V����
	void collide_field();
	//�U������𐶐�
	void generate_attac_collider(const float radius, const float distance, const float height,const float width,const float delay,const float life_span);

public:

	//
	bool dead()const;

	//�R�s�[�֎~
	Enemy(const Enemy& other) = delete;
	Enemy& operator = (const Enemy& other) = delete;

protected:
	
	//���[�V�����̃��[�v�ݒ�
	bool motion_loop_{true};
	// ��ԃ^�C�}
	float state_timer_{0.0f};
	//
	bool is_dead_{ false };
	//�v���C���[
	Actor* player_{nullptr};
	//
	HP HP_;
};

#endif // !ENEMY_H_
