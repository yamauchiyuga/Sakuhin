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
//A�{�^�������������H
bool Input::is_a_push() {
	return gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A);
}


//�U���{�^�������������H
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
//���X�e�B�b�N�̏c�̓��͗�
float Input::get_left_vertical()
{
	return input_left_.y;
}
//���X�e�B�b�N�̉��̓��͗�
float Input::get_left_horizontal() 
{
	return input_left_.x;
}
//�E�X�e�B�b�N�̏c�̓��͗�
float Input::get_right_vertical() {
	return input_right_.y;
}
//�E�X�e�B�b�N�̉��̓��͗�
float Input::get_right_horizontal() {
	return input_right_.x;
}
