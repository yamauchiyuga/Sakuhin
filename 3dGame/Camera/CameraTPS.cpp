#include"CameraTPS.h"
#include"../World/IWorld.h"
#include"../Collision/Field.h"
#include"../Collision/Line.h"

//�v���[���[����̑��΍��W
const GSvector3 PlayerOffset{ 0.0f,2.0f,-4.0f };
//�J�����̒����_�̕⊮
const GSvector3 ReferencePointOffset{ 0.0f, 1.0f, 0.0f };

//�R���X�g���N�^
CameraTPS::CameraTPS(IWorld* world, const GSvector3& position, const GSvector3& at) {
	world_ = world;
	// �^�O�̐ݒ�
	tag_ = "CameraTag";
	// ���O�̐ݒ�
	name_ = "Camera";
	// ���_�̈ʒu��ݒ�
	transform_.position(position);
	// �����_��ݒ�i�����_�̕����Ɍ�����ς���j
	transform_.lookAt(at);
}

//�X�V
void CameraTPS::update(float delta_timer) {
	//�v���[���[����
	Actor* Player = world_->find_actor("Player");
	//�v���[���[��������Ȃ������炻�̂܂ܕԂ�
	if (Player == nullptr) return;
	//���_�ʒu�����߂�(�v���[���[�̔w��̍��W)
	GSvector3 position = PlayerOffset * Player->transform().localToWorldMatrix();
	//�����_�̈ʒu�����߂�(�v���[���[�̓����̏����゠����̍��W)
	GSvector3 at = Player->transform().position() + ReferencePointOffset;
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
}

//�`��
void CameraTPS::draw()const {
	//���_�̈ʒu
	GSvector3 eye = transform_.position();
	//�����_�̈ʒu
	GSvector3 at = eye + transform_.forward();
	// �J�����̐ݒ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,     // ���_�̈ʒu
		at.x, at.y, at.z,      // �����_�̈ʒu
		0.0f, 1.0f, 0.0f       // ���_�̏����
	);

}