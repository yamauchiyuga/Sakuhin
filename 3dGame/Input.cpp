#include"Input.h"

//���͒l��������
GSvector2 Input::input_left_{ 0.0f,0.0f };
GSvector2 Input::input_right_{ 0.0f,0.0f };

//�X�V
void Input::update(float delta_time)
{
	//���͒l�̍X�V
	gsXBoxPadGetLeftAxis(0, &input_left_);
	gsXBoxPadGetRightAxis(0, &input_right_);
}
//�U���{�^����������Ă��邩�H
bool Input::is_attack()
{
	return gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_X);
}
//����{�^�������������H
bool Input::is_dodge()
{
	return gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A);
}
//�K�[�h�{�^�������������H
bool Input::is_guard()
{
	return gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_LEFT_SHOULDER);
}
//���b�N�{�^�������������H
bool Input::is_lock_on()
{
	return gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_RIGHT_SHOULDER);
}
//����{�^�������������H
bool Input::is_determination()
{
	return  gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A);
}
//���X�e�B�b�N�̓��͒l���󂯎��
GSvector2 Input::get_input_left()
{
	return input_left_;
}
//�E�X�e�B�b�N�̓��͒l���󂯎��
GSvector2 Input::get_input_right()
{
	return input_right_;
}
