#include"CameraTPS.h"
#include"../World/IWorld.h"
#include"../Collision/Field.h"
#include"../Collision/Line.h"
#include"../Input.h"

//�J�����̒����_�̕⊮
const GSvector3 ReferencePointOffset{ 0.0f, 2.0f, 0.0f };

//�R���X�g���N�^
CameraTPS::CameraTPS(IWorld* world, const GSvector3& position, const GSvector3& at) :
	state_{ State::PlayerLockOn } {
	world_ = world;
	// �^�O�̐ݒ�
	tag_ = "CameraTag";
	// ���O�̐ݒ�
	name_ = "Camera";
	// ���_�̈ʒu��ݒ�
	transform_.position(position);
	// �����_��ݒ�i�����_�̕����Ɍ�����ς���j
	transform_.lookAt(at);
	///x�����̉�]�p�x�̏�����
	pitch_ = (at - position).getPitch();
	///y�����̉�]�p�x�̏�����
	yaw_ = (at - position).getYaw();
}

//�X�V
void CameraTPS::update(float delta_time) {
	switch (state_)
	{
	case CameraTPS::State::PlayerLockOn:player_lock_on(delta_time); break;
	case CameraTPS::State::EnemyLockOn:enemy_lock_on(delta_time); break;
	}
}

void CameraTPS::player_lock_on(float delta_time) {
	//�v���[���[����
	Actor* Player = world_->find_actor("Player");
	//�v���[���[��������Ȃ������炻�̂܂ܕԂ�
	if (Player == nullptr) return;

	//��]�X�s�[�h
	const float RotateSpeed{ 3.0f };
	///y�����ɉ�]������
	yaw_ += Input::get_right_horizontal() * -RotateSpeed * delta_time;
	//x�����ɉ�]������
	pitch_ += Input::get_right_vertical() * -RotateSpeed * delta_time;

	//x���̍Œ�p�x
	const float MinAngle{ -30.0f };
	//x���̍ő�p�x
	const float MaxAngle{ -10.0f };
	//x�����̉�]�𐧌�����
	pitch_ = CLAMP(pitch_, MinAngle, MaxAngle);
	//�v���C���[����̑��΍��W
	const GSvector3 PlayerOffset{ 0.0f, 2.0f, -7.0f };

	//�����_�̈ʒu�����߂�(�v���[���[�̓����̏����゠����̍��W)
	GSvector3 at = Player->transform().position() + ReferencePointOffset;
	//���_�ʒu�����߂�(�v���[���[�̔w��̍��W)
	GSvector3 position = at + GSquaternion::euler(pitch_, yaw_, 3.0f) * PlayerOffset;

	const float SmoothTime{ 13.0f };    // ��ԃt���[����
	const float MaxSpeed{ 2.0f };       // �ړ��X�s�[�h�̍ő�l
	position = GSvector3::smoothDamp(transform_.position(), position, velocity_,
		SmoothTime, MaxSpeed, delta_time);


	//�t�B�[���h�Ƃ̏Փ˔���
	Line line{ at,position };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		position = intersect;
	}

	//���_�̈ʒu�̐ݒ�
	transform_.position(position);
	//�����_��ݒ�(�����_�̕����Ɍ�����ς���)
	transform_.lookAt(at);

	if (Input::is_lock_on()) {
		state_ = State::EnemyLockOn;
	}
}

void CameraTPS::enemy_lock_on(float delta_time) {
	//
	Actor* Player = world_->find_actor("Player");
	//
	Actor* Enemy = world_->find_actor("Enemy");

	if (Enemy == nullptr)return;

		//��]�X�s�[�h
		const float RotateSpeed{ 2.0f };
	const float Distance{ 6.0f };
	const float CamerHeightPos{ 2.0f };

	GSvector3 enemy_to_player = Player->transform().position() - Enemy->transform().position();
	enemy_to_player.normalize();
	enemy_to_player = enemy_to_player * Distance;
	enemy_to_player.y = CamerHeightPos;

	//�����_�̈ʒu�����߂�(�v���[���[�̓����̏����゠����̍��W)
	GSvector3 at = Enemy->transform().position() + ReferencePointOffset;
	//���_�ʒu�����߂�(�v���[���[�̔w��̍��W)
	GSvector3 position = Player->transform().position() + enemy_to_player;

	//�t�B�[���h�Ƃ̏Փ˔���
	Line line{ at,position };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		position = intersect;
	}

	//���_�̈ʒu�̐ݒ�
	transform_.position(position);
	//�����_��ݒ�(�����_�̕����Ɍ�����ς���)
	transform_.lookAt(at);

	if (Input::is_lock_on()) {
		yaw_ = transform_.eulerAngles().y;
		state_ = State::PlayerLockOn;
	}


}

//�`��
void CameraTPS::draw()const {
	//���_�̈ʒu
	GSvector3 eye = transform_.position();
	//�����_�̈ʒu
	GSvector3 at = eye + transform_.forward();
	//
	GSvector3 up = transform_.up();
	// �J�����̐ݒ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,     // ���_�̈ʒu
		at.x, at.y, at.z,      // �����_�̈ʒu
		up.x, up.y, up.z       // ���_�̏����
	);

}