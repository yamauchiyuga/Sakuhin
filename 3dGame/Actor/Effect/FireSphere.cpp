#include"FireSphere.h"
#include"../../World/IWorld.h"
#include"../../Collision/Field.h"
#include"../../Collision/Line.h"

FireSphere::FireSphere(IWorld* world, const GSvector3& position, const GSvector3& velocity) {
	// ���[���h��ݒ�
	world_ = world;
	// �^�O��
	tag_ = "EnemyAttackTag";
	// �A�N�^�[��
	name_ = "EnemyAttack";
	// �ړ��ʂ̏�����
	velocity_ = velocity;
	// �Փ˔���p�̋��̂�ݒ�
	collider_ = BoundingSphere{ 1.0f };
	// ���W�̏�����
	transform_.position(position);
	// ����
	lifespan_timer_ = 120.0f;

}

void FireSphere::update(float delta_time) {
	// �������s�����玀�S
	if (lifespan_timer_ <= 0.0f) {
		die();
		return;
	}
	// �����̍X�V
	lifespan_timer_ -= delta_time;
	// �t�B�[���h�Ƃ̏Փ˔���
	Line line;
	line.start = transform_.position();
	line.end = transform_.position() + velocity_;
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		// ��_�̍��W�ɕ␳
		transform_.position(intersect);
		// �t�B�[���h�ɏՓ˂����玀�S
		die();
		return;
	}
	// �ړ�����i���[���h���W�n��j
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
}

void FireSphere::draw() const {
	// �f�o�b�O�\��
	collider().draw();
}

void FireSphere::react(Actor& other) {
	if (other.tag() == "PLayerTag") {
		// �Փ˂����玀�S
		die();
	}
}
