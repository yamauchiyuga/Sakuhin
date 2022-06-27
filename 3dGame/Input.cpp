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
//Aボタンを押したか？
bool Input::is_a_push() {
	return gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A);
}


//攻撃ボタンを押したか？
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
//左スティックの縦の入力量
float Input::get_left_vertical()
{
	return input_left_.y;
}
//左スティックの横の入力量
float Input::get_left_horizontal() 
{
	return input_left_.x;
}
//右スティックの縦の入力量
float Input::get_right_vertical() {
	return input_right_.y;
}
//右スティックの横の入力量
float Input::get_right_horizontal() {
	return input_right_.x;
}
