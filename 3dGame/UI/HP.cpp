#include "HP.h"
#include"../Assets.h"
#include"UIparameter.h"

//�R���X�g���N�^
HP::HP(const float max_health) :
	max_health_{ max_health },
	cullent_health_{ max_health }
{
}

//�_���[�W����
void HP::hit_damage(int power) {
	//0�����ɂȂ�Ȃ��ɂ��Ă���
	cullent_health_ = MAX(cullent_health_ - power, 0);
}
//�v���C���[�p
void HP::draw_player() const {
	UIparameter Player;
	Player.gage_position={ 80.0f, 70.0f };
	Player.gage_frame_position={ 79.0f, 69.0f };
	Player.gage={ 0, 0, (cullent_health_ / max_health_) * 250.0f, 13.0f };
	Player.back_gage={ 0,0,250.0f, 13.0f };
	Player.gage_color={ 0,1,0,1 };
	Player.gage_frame_color = { 1,1,1,1 };
	Player.back_gage_color={ 0,0,0,1 };

	//�t���[���̕`��
	gsDrawSprite2D(Texture_Frame, &Player.gage_frame_position, NULL, NULL, &Player.gage_frame_color, NULL, 0.0f);
	//�w�i�̕`��
	gsDrawSprite2D(Texture_HP, &Player.gage_position, &Player.back_gage, NULL, &Player.back_gage_color, NULL, 0.0f);
	//HP�Q�[�W�̕`��
	gsDrawSprite2D(Texture_HP, &Player.gage_position, &Player.gage, NULL, &Player.gage_color, NULL, 0.0f);
}

//�{�X�p
void HP::draw_boss() const {
	UIparameter Dragon;
	Dragon.gage_position = { 250.0f, 650.0f };
	Dragon.gage_frame_position = { 248.0f, 649.0f };
	Dragon.gage = { 0, 0, (cullent_health_ / max_health_) * 750.0f, 10.0f };
	Dragon.back_gage = { 0,0, 754.0f, 12.0f };
	Dragon.gage_color = { 1,0,0,1 };
	Dragon.gage_frame_color = { 1,1,1,1 };
	Dragon.back_gage_color = { 0,0,0,1 };
	//�t���[���̕`��
	gsDrawSprite2D(Texture_Frame, &Dragon.gage_frame_position, NULL, NULL, &Dragon.gage_frame_color, NULL, 0.0f);
	//�w�i�̕`��
	gsDrawSprite2D(Texture_HP, &Dragon.gage_position, &Dragon.back_gage, NULL, &Dragon.back_gage_color, NULL, 0.0f);
	//HP�Q�[�W�̕`��
	gsDrawSprite2D(Texture_HP, &Dragon.gage_position, &Dragon.gage, NULL, &Dragon.gage_color, NULL, 0.0f);
}
//���݂�HP ��Ԃ�
float HP::cullent_health()const {
	return cullent_health_;
}
//���S���Ă邩�Ԃ�
bool HP::is_end()const {
	if (cullent_health_ <= 0)return true;
	return false;
}