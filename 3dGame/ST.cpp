#include "ST.h"
#include"Asset.h"

#include<GSvector2.h>
#include<GSgraphics.h>
#include<GSmath.h>

ST::ST(float max_stamina) :
	max_stamina_{max_stamina},
	cullent_stamina_{max_stamina},
	dodge_stamina_{0}{};

void ST::update(float delta_time) {
	cullent_stamina_ = MIN(cullent_stamina_ + delta_time, max_stamina_);
}

bool ST::dodge_stamina(int dodge_stamina) {
	//
	bool a = cullent_stamina_ >= dodge_stamina;
	//
	if(a) cullent_stamina_ -= dodge_stamina;
	//
	return a;
}

int ST::get_stamina()const {
	return cullent_stamina_;
}

void ST::draw()const {
	//STQ[WΜΚu
	const GSvector2 position{ 80.0f, 85.0f };
	//Q[WΜt[ΜΚu
	const GSvector2 framePosition{ 79.0f, 84.0f };
	//STQ[W{ΜΜι`
	const GSrect rect{ 0, 0, (cullent_stamina_ /max_stamina_ ) * 250.0f, 13.0f };
	//wiΜι`
	const GSrect back_rect{ 0,0,250.0f, 13.0f };
	//STQ[WΜF
	const GScolor barColor{ 1,1,1,1 };
	//wiΜF
	const GScolor backColor{ 0,0,0,1 };
	//t[Μ`ζ
	gsDrawSprite2D(Texture_Frame, &framePosition, NULL, NULL, NULL, NULL, 0.0f);
	//wiΜ`ζ
	gsDrawSprite2D(Texture_HP, &position, &back_rect, NULL, &backColor, NULL, 0.0f);
	//STQ[WΜ`ζ
	gsDrawSprite2D(Texture_HP, &position, &rect, NULL, &barColor, NULL, 0.0f);
}
