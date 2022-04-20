#include "HP.h"
#include"Asset.h"

#include<GSvector2.h>
#include<GSgraphics.h>

//
HP::HP(float max_health) :
	max_health_{ max_health },
	cullent_health_{max_health}{};

//
void HP::hit_damage(int power) {
	cullent_health_ -= power;
}

void HP::draw()const {
	//HPゲージの位置
	const GSvector2 position{ 80.0f, 70.0f };
	//ゲージのフレームの位置
	const GSvector2 framePosition{ 79.0f, 69.0f };
	//HPゲージ本体の矩形
	const GSrect rect{ 0, 0, (cullent_health_ / max_health_) * 250.0f, 13.0f };
	//背景の矩形
	const GSrect back_rect{ 0,0,250.0f, 13.0f };
	//HPゲージの色
	const GScolor barColor{ 0,1,0,1 };
	//背景の色
	const GScolor backColor{ 0,0,0,1 };
	//フレームの描画
	gsDrawSprite2D(Texture_Frame, &framePosition, NULL, NULL, NULL, NULL, 0.0f);
	//背景の描画
	gsDrawSprite2D(Texture_HP, &position, &back_rect, NULL, &backColor, NULL, 0.0f);
	//HPゲージの描画
	gsDrawSprite2D(Texture_HP, &position, &rect, NULL, &barColor, NULL, 0.0f);

}

bool HP::is_end()const {
	if (cullent_health_ <= 0)return true;
	return false;
}