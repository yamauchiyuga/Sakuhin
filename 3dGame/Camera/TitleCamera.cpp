#include"TitleCamera.h"

TitleCamera::TitleCamera(std::shared_ptr<IWorld> world, const GSvector3& position, const GSvector3& at)
{
	world_ = world;
	tag_ = "CameraTag";
	name_ = "Camera";
	transform_.position(position);
	transform_.lookAt(at);
}
 
void TitleCamera::draw()const 
{
	//�o�b�t�@�̃N���A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//�����ˉe��̕ϊ��s���ݒ肷��
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		45.0f,
		1280.0f / 720.0f,
		0.3f,
		500.0f
	);
	//���_�̈ʒu
	GSvector3 eye = transform_.position();
	//�����_�̈ʒu
	GSvector3 at = eye + transform_.forward();
	//���_�̏����
	GSvector3 up = transform_.up();
	//�J�����̐ݒ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,     // ���_�̈ʒu
		at.x, at.y, at.z,      // �����_�̈ʒu
		up.x, up.y, up.z      // ���_�̏����
	);
}