#ifndef INPUT_H_
#define INPUT_H_

#include <gslib.h>

class Input
{
public:

	//更新
	static void update(float delta_time);
	//Aボタンを押したか？
	static bool is_a_push();
	//攻撃ボタンを押したか？
	static bool is_attack();
	//回避ボタンを押したか？
	static bool is_dodge();
	//ガードボタンを押したか？
	static bool is_guard();
	//ロックボタンを押したか？
	static bool is_lock_on();
	//決定ボタンを押したか？
	static bool is_determination();
	//左スティックの縦の入力量
	static float get_left_vertical();
	//左スティックの横の入力量
	static float get_left_horizontal();
	//右スティックの縦の入力量
	static float get_right_vertical();
	//右スティックの横の入力量
	static float get_right_horizontal();
private:
	//左スティックの入力値
	static GSvector2 input_left_;
	//右スティックの入力値
	static GSvector2 input_right_;
};

#endif
