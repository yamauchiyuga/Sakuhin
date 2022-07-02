#ifndef HP_H_
#define HP_H_

#include<GSvector2.h>
#include<GSgraphics.h>

class HP
{
public:
	//�R���X�g���N�^
	HP() = default;
	//
	HP(const float max_health);
	//�v���C���[�p
	void draw_player()const;
	//�{�X�p
	void draw_boss()const;
	//�_���[�W����
	void hit_damage(int power);
	//���݂�HP��Ԃ�
	float cullent_health()const;
	//���S�������H
	bool is_end()const;

private:
	//���݂�HP
	float cullent_health_;
	//�ő�HP 
	float max_health_;

};


#endif 
