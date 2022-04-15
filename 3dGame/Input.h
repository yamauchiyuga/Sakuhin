#ifndef INPUT_H_
#define INPUT_H_

#include <gslib.h>

class Input
{
public:

	//更新
	void update(float delta_time);
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
	//左スティックの入力値を受け取る
	static GSvector2 get_input_left();
	//右スティックの入力値を受け取る
	static GSvector2 get_input_right();
private:
	//左スティックの入力値
	static GSvector2 input_left_;
	//右スティックの入力値
	static GSvector2 input_right_;
};

#endif
