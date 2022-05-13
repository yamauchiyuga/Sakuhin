#include "HP.h"
#include"../Asset.h"


HP::HP(const float max_health, const GSvector2 HP_pos, const GSvector2 frame_pos,const float gauge_length):
	HP_pos_{HP_pos},
	frame_pos_{frame_pos},
	max_health_{max_health},
	cullent_health_{max_health},
	gauge_length_{ gauge_length }{
};

//
void HP::hit_damage(int power) {
	cullent_health_ = MAX(cullent_health_ - power, 0);
}

void HP::draw()const {
	//HP�Q�[�W�{�̂̋�`
	const GSrect rect{ 0, 0, (cullent_health_ / max_health_) * gauge_length_, 13.0f };
	//�w�i�̋�`
	const GSrect back_rect{ 0,0,gauge_length_, 13.0f };
	//HP�Q�[�W�̐F
	const GScolor barColor{ 0,1,0,1 };
	//�w�i�̐F
	const GScolor backColor{ 0,0,0,1 };
	//�t���[���̕`��
	gsDrawSprite2D(Texture_Frame, &frame_pos_, NULL, NULL, NULL, NULL, 0.0f);
	//�w�i�̕`��
	gsDrawSprite2D(Texture_HP, &HP_pos_, &back_rect, NULL, &backColor, NULL, 0.0f);
	//HP�Q�[�W�̕`��
	gsDrawSprite2D(Texture_HP, &HP_pos_, &rect, NULL, &barColor, NULL, 0.0f);

}

float HP::cullent_health()const {
	return cullent_health_;
}

bool HP::is_end()const {
	if (cullent_health_ <= 0)return true;
	return false;
}