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
	//HPQ[WÌÊu
	const GSvector2 position{ 80.0f, 70.0f };
	//Q[WÌt[ÌÊu
	const GSvector2 framePosition{ 79.0f, 69.0f };
	//HPQ[W{ÌÌé`
	const GSrect rect{ 0, 0, (cullent_health_ / max_health_) * 250.0f, 13.0f };
	//wiÌé`
	const GSrect back_rect{ 0,0,250.0f, 13.0f };
	//HPQ[WÌF
	const GScolor barColor{ 0,1,0,1 };
	//wiÌF
	const GScolor backColor{ 0,0,0,1 };
	//t[Ì`æ
	gsDrawSprite2D(Texture_Frame, &framePosition, NULL, NULL, NULL, NULL, 0.0f);
	//wiÌ`æ
	gsDrawSprite2D(Texture_HP, &position, &back_rect, NULL, &backColor, NULL, 0.0f);
	//HPQ[WÌ`æ
	gsDrawSprite2D(Texture_HP, &position, &rect, NULL, &barColor, NULL, 0.0f);

}

bool HP::is_end()const {
	if (cullent_health_ <= 0)return true;
	return false;
}