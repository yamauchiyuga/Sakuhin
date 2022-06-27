#include "ST.h"
#include"../Assets.h"

#include<GSvector2.h>
#include<GSgraphics.h>
#include<GSmath.h>

//�N�[���^�C��
float Count{ 0.0f };
//�N�[���^�C���̎���
const int Latency{ 80 };

ST::ST(float max_stamina) :
	max_stamina_{ max_stamina },
	cullent_stamina_{ max_stamina },
	consumption_stamina_{ 0 }
{
};
//�X�V
void ST::update(float delta_time) 
{
	//�N�[���^�C�����傫��������X�^�~�i�� 
	if (Count > Latency) cullent_stamina_ = MIN(cullent_stamina_ + delta_time, max_stamina_);
	Count += delta_time;
}
//�X�^�~�i����̏���
bool ST::consumption_stamina(const int consumption_stamina) {
	//���݂̃X�^�~�i�������葽�����H
	bool a = cullent_stamina_ >= consumption_stamina;
	if (a)
	{
		cullent_stamina_ -= consumption_stamina;
		Count = 0;
	}
	
	return a;
}
//���݂̃X�^�~�i��Ԃ�
int ST::get_stamina()const {
	return cullent_stamina_;
}
//�`��
void ST::draw()const {
	//ST�Q�[�W�̈ʒu
	const GSvector2 position{ 80.0f, 85.0f };
	//�Q�[�W�̃t���[���̈ʒu
	const GSvector2 framePosition{ 79.0f, 84.0f };
	//ST�Q�[�W�{�̂̋�`
	const GSrect rect{ 0, 0, (cullent_stamina_ / max_stamina_) * 250.0f, 13.0f };
	//�w�i�̋�`
	const GSrect back_rect{ 0,0,250.0f, 13.0f };
	//ST�Q�[�W�̐F
	const GScolor barColor{ 1,1,1,1 };
	//�w�i�̐F
	const GScolor backColor{ 0,0,0,1 };
	//�t���[���̕`��
	gsDrawSprite2D(Texture_Frame, &framePosition, NULL, NULL, NULL, NULL, 0.0f);
	//�w�i�̕`��
	gsDrawSprite2D(Texture_HP, &position, &back_rect, NULL, &backColor, NULL, 0.0f);
	//ST�Q�[�W�̕`��
	gsDrawSprite2D(Texture_HP, &position, &rect, NULL, &barColor, NULL, 0.0f);
}
