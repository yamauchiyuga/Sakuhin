#ifndef UI_PARAMETER_
#define UI_PARAMETER_
#include<GStype.h>

struct  UIparameter {
	//�Q�[�W�̈ʒu
	GSvector2 gage_position;
	//�Q�[�W�̃t���[���̈ʒu
	GSvector2 gage_frame_position;
	//�Q�[�W�{�̂̋�`
	GSrect gage;
	//�w�i�̋�`
	GSrect back_gage;
	//�Q�[�W�̐F
	GScolor gage_color;
	//�Q�[�W�̃t���[���̐F
	GScolor gage_frame_color;
	//�w�i�̐F
	GScolor back_gage_color;
};

#endif // !UI_PARAMETER_
