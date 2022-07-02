#ifndef INPUT_H_
#define INPUT_H_

#include <gslib.h>

class Input
{
public:

	//�X�V
	static void update(float delta_time);
	//A�{�^�������������H
	static bool is_a_push();
	//�U���{�^�������������H
	static bool is_attack();
	//����{�^�������������H
	static bool is_dodge();
	//�K�[�h�{�^�������������H
	static bool is_guard();
	//���b�N�{�^�������������H
	static bool is_lock_on();
	//����{�^�������������H
	static bool is_determination();
	//���X�e�B�b�N�̏c�̓��͗�
	static float get_left_vertical();
	//���X�e�B�b�N�̉��̓��͗�
	static float get_left_horizontal();
	//�E�X�e�B�b�N�̏c�̓��͗�
	static float get_right_vertical();
	//�E�X�e�B�b�N�̉��̓��͗�
	static float get_right_horizontal();
private:
	//���X�e�B�b�N�̓��͒l
	static GSvector2 input_left_;
	//�E�X�e�B�b�N�̓��͒l
	static GSvector2 input_right_;
};

#endif
