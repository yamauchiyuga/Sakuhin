#include "HP.h"
#include"../Assets.h"


HP::HP(const float max_health):
	max_health_{ max_health },
	cullent_health_{ max_health }
{
}


//
void HP::hit_damage(int power) {
	cullent_health_ = MAX(cullent_health_ - power, 0);
}

void HP::draw_player() const{
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

void HP::draw_boss() const{
	//HPQ[WÌÊu
	const GSvector2 Position{ 250.0f, 650.0f };
	//Q[WÌt[ÌÊu
	const GSvector2 FramePosition{ 248.0f, 649.0f };
	//HPQ[W{ÌÌé`
	const GSrect Rect{ 0, 0, (cullent_health_ / max_health_) * 750.0f, 10.0f };
	//wiæÌé`
	const GSrect BackRect{ 0,0,750.0f, 10.0f };
	//t[Ìé`
	const GSrect FrameRect{ 0,0, 754.0f, 12.0f };
	//HPQ[WÌF
	const GScolor HealthColor{ 1,0,0,1 };
	//wiÌF
	const GScolor BackColor{ 0,0,0,1 };
	//t[ÌF
	const GScolor FrameColor{ 1,1,1,1 };

	//t[Ì`æ
	gsDrawSprite2D(Texture_HP, &FramePosition, &FrameRect, NULL, &FrameColor, NULL, 0.0f);
	//wiÌ`æ
	gsDrawSprite2D(Texture_HP, &Position, &BackRect, NULL, &BackColor, NULL, 0.0f);
	//HPQ[WÌ`æ
	gsDrawSprite2D(Texture_HP, &Position, &Rect, NULL, &HealthColor, NULL, 0.0f);
}

float HP::cullent_health()const {
	return cullent_health_;
}

bool HP::is_end()const {
	if (cullent_health_ <= 0)return true;
	return false;
}