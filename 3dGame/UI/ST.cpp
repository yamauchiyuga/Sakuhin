#include "ST.h"
#include"../Asset.h"

#include<GSvector2.h>
#include<GSgraphics.h>
#include<GSmath.h>

//クールタイム
float count{ 0.0f };
//クールタイムの時間
const int latency{ 80 };

ST::ST(float max_stamina) :
	max_stamina_{ max_stamina },
	cullent_stamina_{ max_stamina },
	consumption_stamina_{ 0 }{};

void ST::update(float delta_time) {
	if (count > latency) cullent_stamina_ = MIN(cullent_stamina_ + delta_time, max_stamina_);
	count += delta_time;
}

bool ST::consumption_stamina(int consumption_stamina) {
	//
	bool a = cullent_stamina_ >= consumption_stamina;
	//
	if (a)
	{
		cullent_stamina_ -= consumption_stamina;
		count = 0;
	}
	//
	return a;
}

int ST::get_stamina()const {
	return cullent_stamina_;
}

void ST::draw()const {
	//STゲージの位置
	const GSvector2 position{ 80.0f, 85.0f };
	//ゲージのフレームの位置
	const GSvector2 framePosition{ 79.0f, 84.0f };
	//STゲージ本体の矩形
	const GSrect rect{ 0, 0, (cullent_stamina_ / max_stamina_) * 250.0f, 13.0f };
	//背景の矩形
	const GSrect back_rect{ 0,0,250.0f, 13.0f };
	//STゲージの色
	const GScolor barColor{ 1,1,1,1 };
	//背景の色
	const GScolor backColor{ 0,0,0,1 };
	//フレームの描画
	gsDrawSprite2D(Texture_Frame, &framePosition, NULL, NULL, NULL, NULL, 0.0f);
	//背景の描画
	gsDrawSprite2D(Texture_HP, &position, &back_rect, NULL, &backColor, NULL, 0.0f);
	//STゲージの描画
	gsDrawSprite2D(Texture_HP, &position, &rect, NULL, &barColor, NULL, 0.0f);
}
