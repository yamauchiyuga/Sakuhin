#ifndef LIGHT_H_
#define LIGHT_H_

#include "Actor.h"
#include<memory>
// ���C�g�N���X
class Light : public Actor
{
public:
	// �R���X�g���N�^
	Light(std::shared_ptr<IWorld> world);
	//
	virtual void update(float delta_time)override;
	// �`��
	virtual void draw() const override;
private:
	// ���C�g�̃p�����[�^
	float light_ambient_[4];
	float light_diffuse_[4];
	float light_specular_[4];
	float light_constant_attenuation_;
	float light_linear_attenuation_;
	float light_quadtatic_attenuation_;
	// �������C�g�̃p�����[�^
	GScolor light_rim_color_;
	float   light_rim_exponent_;
	//�J�����擾
	std::shared_ptr<Actor> Camera_;
};

#endif