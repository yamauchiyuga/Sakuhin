#include"Input.h"

//入力値を初期化
GSvector2 Input::input_left_{ 0.0f,0.0f };
GSvector2 Input::input_right_{ 0.0f,0.0f };

//更新
void Input::update(float delta_time)
{
	//入力値の更新
	gsXBoxPadGetLeftAxis(0, &input_left_);
	gsXBoxPadGetRightAxis(0, &input_right_);
}
//攻撃ボタンを押されているか？
bool Input::is_attack()
{
	return gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_X);
}
//回避ボタンを押したか？
bool Input::is_dodge()
{
	return gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A);
}
//ガードボタンを押したか？
bool Input::is_guard()
{
	return gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_LEFT_SHOULDER);
}
//ロックボタンを押したか？
bool Input::is_lock_on()
{
	return gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_RIGHT_SHOULDER);
}
//決定ボタンを押したか？
bool Input::is_determination()
{
	return  gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A);
}
//左スティックの入力値を受け取る
GSvector2 Input::get_input_left()
{
	return input_left_;
}
//右スティックの入力値を受け取る
GSvector2 Input::get_input_right()
{
	return input_right_;
}
