#include "HP.h"
#include"../Assets.h"

//�R���X�g���N�^
HP::HP(const float max_health):
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
void HP::draw_player() const{
	//HP�Q�[�W�̈ʒu
	const GSvector2 position{ 80.0f, 70.0f };
	//�Q�[�W�̃t���[���̈ʒu
	const GSvector2 framePosition{ 79.0f, 69.0f };
	//HP�Q�[�W�{�̂̋�`
	const GSrect rect{ 0, 0, (cullent_health_ / max_health_) * 250.0f, 13.0f };
	//�w�i�̋�`
	const GSrect back_rect{ 0,0,250.0f, 13.0f };
	//HP�Q�[�W�̐F
	const GScolor barColor{ 0,1,0,1 };
	//�w�i�̐F
	const GScolor backColor{ 0,0,0,1 };
	//�t���[���̕`��
	gsDrawSprite2D(Texture_Frame, &framePosition, NULL, NULL, NULL, NULL, 0.0f);
	//�w�i�̕`��
	gsDrawSprite2D(Texture_HP, &position, &back_rect, NULL, &backColor, NULL, 0.0f);
	//HP�Q�[�W�̕`��
	gsDrawSprite2D(Texture_HP, &position, &rect, NULL, &barColor, NULL, 0.0f);
}

//�{�X�p
void HP::draw_boss() const{
	//HP�Q�[�W�̈ʒu
	const GSvector2 Position{ 250.0f, 650.0f };
	//�Q�[�W�̃t���[���̈ʒu
	const GSvector2 FramePosition{ 248.0f, 649.0f };
	//HP�Q�[�W�{�̂̋�`
	const GSrect Rect{ 0, 0, (cullent_health_ / max_health_) * 750.0f, 10.0f };
	//�w�i�摜�̋�`
	const GSrect BackRect{ 0,0,750.0f, 10.0f };
	//�t���[���̋�`
	const GSrect FrameRect{ 0,0, 754.0f, 12.0f };
	//HP�Q�[�W�̐F
	const GScolor HealthColor{ 1,0,0,1 };
	//�w�i�̐F
	const GScolor BackColor{ 0,0,0,1 };
	//�t���[���̐F
	const GScolor FrameColor{ 1,1,1,1 };

	//�t���[���̕`��
	gsDrawSprite2D(Texture_HP, &FramePosition, &FrameRect, NULL, &FrameColor, NULL, 0.0f);
	//�w�i�̕`��
	gsDrawSprite2D(Texture_HP, &Position, &BackRect, NULL, &BackColor, NULL, 0.0f);
	//HP�Q�[�W�̕`��
	gsDrawSprite2D(Texture_HP, &Position, &Rect, NULL, &HealthColor, NULL, 0.0f);
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