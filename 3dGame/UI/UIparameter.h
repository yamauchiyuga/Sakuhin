#ifndef UI_PARAMETER_
#define UI_PARAMETER_
#include<GStype.h>

struct  UIparameter {
	//ゲージの位置
	GSvector2 gage_position;
	//ゲージのフレームの位置
	GSvector2 gage_frame_position;
	//ゲージ本体の矩形
	GSrect gage;
	//背景の矩形
	GSrect back_gage;
	//ゲージの色
	GScolor gage_color;
	//ゲージのフレームの色
	GScolor gage_frame_color;
	//背景の色
	GScolor back_gage_color;
};

#endif // !UI_PARAMETER_
