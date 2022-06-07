#include "Explosion.h"
#include"../../World/IWorld.h"
#include"../../Collision/Field.h"
#include"../../Collision/Line.h"


const float delay_timer{ 20.0f };

Explosion::Explosion(IWorld* world, const GSvector3& position, const float delay_timer) {
	// ���[���h��ݒ�
	world_ = world;
	// �^�O��
	tag_ = "EnemyAttackTag";
	// �A�N�^�[��
	name_ = "EnemyAttack";
	// �Փ˔���p�̋��̂�ݒ�
	collider_ = BoundingSphere{ 1.0f };
	// ���W�̏�����
	transform_.position(position);
	// ����
	lifespan_timer_ = 10.0f;
	// �Փ˔���𖳌��ɂ���
	enable_collider_ = false;
	//
	delay_timer_ = delay_timer;
}

void Explosion::update(float delta_time) {
	if (delay_timer_ <= 0.0f) {
		// �Փ˔����L���ɂ���
		enable_collider_ = true;
		// �������s�����玀�S
		if (lifespan_timer_ <= 0.0f) {
			die();
		}
		// �����̍X�V
		lifespan_timer_ -= delta_time;
	}
	// �x�����Ԃ̍X�V
	delay_timer_ -= delta_time;
}

void Explosion::draw() const {
	// �f�o�b�O�\��
	collider().draw();
}

void Explosion::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		// �Փ˂����玀�S
		die();
	}
}
