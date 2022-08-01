#include "Enemy.h"
#include"../../World/IWorld.h"
#include"../../Collision/Field.h"
#include"../../Collision/Line.h"
#include"../AttackCollider.h"


float Enemy::target_signed_angle()const {
	//�^�[�Q�b�g�̕����x�N�g�������߂�
	GSvector3 to_target = player_->transform().position() - transform_.position();
	//�O���������x�N�g�����擾
	GSvector3 forward = transform_.forward();
	//�x�N�g��y�����𖳌��ɂ���
	forward.y = 0.0f;
	to_target.y = 0.0f;
	//// �O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x�������߂�
	return GSvector3::signedAngle(forward, to_target);
}

//�^�[�Q�b�g�����̊p�x�����߂�i�����Ȃ��j
float Enemy::target_angle()const {
	return std::abs(target_signed_angle());
}

//�^�[�Q�b�g�Ƃ̋��������߂�
float Enemy::target_distance() const {
	return (player_->transform().position() - transform_.position()).magnitude();
}

bool Enemy::dead()const {
	return is_dead_;
}

void Enemy::collide_actor(Actor& other) {
	//y���W���������W�����߂�
	GSvector3 position = collider().center_;
	GSvector3 target = other.collider().center_;
	//����Ƃ̋���
	float distance = GSvector3::distance(position, target);
	//�Փ˔��苅�̔��a���m�����������������߂�
	float length = collider_.radius_ + other.collider().radius_;
	//�Փ˔��苅�̏d�Ȃ��Ă��钷�������߂�
	float overlap = length - distance;
	//�d�Ȃ��Ă��镔���̔����̋������������ړ��ʂ����߂�
	GSvector3 v = (position - target).getNormalized() * overlap;
	transform_.translate(v, GStransform::Space::World);
	//�t�B�[���h�Ƃ̏Փ˔���
	collide_field();
}

//�t�B�[���h�Ƃ̏Փ˔���
void Enemy::collide_field() {
	//�����̃I�t�Z�b�g
	const float FootOffset{ 0.2f };
	GSvector3 center; // �Փˌ�̋��̂̒��S���W
	if (world_->field()->collide(collider(), &center)) {
		// y���W�͕ύX���Ȃ�
		center.y = transform_.position().y;
		// �␳��̍��W�ɕύX����
		transform_.position(center);
		hit_wall_ = true;
	}

	// �n�ʂƂ̏Փ˔���i�����Ƃ̌�������j
	GSvector3 position = transform_.position();
	Line line;
	line.start = position + collider_.center_;
	line.end = position + GSvector3{ 0.0f, -FootOffset, 0.0f };
	GSvector3 intersect;  // �n�ʂƂ̌�_
	if (world_->field()->collide(line, &intersect)) {
		// ��_�̈ʒu����y���W�̂ݕ␳����
		position.y = intersect.y;
		// ���W��ύX����
		transform_.position(position);
		// �d�͂�����������
		velocity_.y = 0.0f;
	}
}

//�U������̐���
void Enemy::generate_attack_collider(const float radius, const float distance, const float height, const float width, const float delay, const float life_span, const std::string& attack_name) {

	// �Փ˔�����o����������W�����߂�i�O���̈ʒu�j
	GSvector3 position = transform_.position() + (transform_.forward() * distance + transform_.right() * width);
	// �����̕␳�i��������̍����j
	position.y += height;
	// �Փ˔���p�̋����쐬
	BoundingSphere collider{ radius, position };
	// �Փ˔�����o��������
	world_->add_actor(std::make_shared<AttackCollider>( world_, collider,
		"EnemyAttackTag", attack_name, tag_, life_span, delay ));
}

// �ǂɂԂ�������
bool Enemy::is_hit_wall()const {
	return hit_wall_;
}



