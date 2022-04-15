#ifndef INPUT_H_
#define INPUT_H_

#include <gslib.h>

class Input
{
public:

	//�X�V
	void update(float delta_time);
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
	//���X�e�B�b�N�̓��͒l���󂯎��
	static GSvector2 get_input_left();
	//�E�X�e�B�b�N�̓��͒l���󂯎��
	static GSvector2 get_input_right();
private:
	//���X�e�B�b�N�̓��͒l
	static GSvector2 input_left_;
	//�E�X�e�B�b�N�̓��͒l
	static GSvector2 input_right_;
};

#endif
