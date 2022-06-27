#include "Light.h"
#include<imgui/imgui.h>
#include"../World/IWorld.h"
#include"../Camera/CameraTPS.h"
//�R���X�g���N�^
Light::Light(std::shared_ptr<IWorld> world) :
	// ���C�g�̃p�����[�^
	light_ambient_{ 0.0087f, 0.094f, 0.333f, 1.0f },
	light_diffuse_{ 0.9f, 0.5f, 0.2f, 1.0f },
	light_specular_{ 0.9f, 0.5f, 0.2f, 1.0f },
	light_constant_attenuation_{ 1.0f },
	light_linear_attenuation_{ 0.1f },
	light_quadtatic_attenuation_{ 0.1f }
{
	world_ = world;
	Camera_ = world_->camera();
	name_ = "Light";
	tag_ = "LightTag";

}

void Light::update(float delta_time) 
{
	// ���C�g�̃p�����[�^����
	ImGui::Begin("Lighting");
	ImGui::ColorEdit3("ambient", light_ambient_, ImGuiColorEditFlags_Float);
	ImGui::ColorEdit3("diffuse", light_diffuse_, ImGuiColorEditFlags_Float);
	ImGui::DragFloat("constant", &light_constant_attenuation_, 0.001f, 0.0f, 1.0f);
	ImGui::DragFloat("linear", &light_linear_attenuation_, 0.001f, 0.0f, 1.0f);
	ImGui::DragFloat("quadratic", &light_quadtatic_attenuation_, 0.001f, 0.0f, 1.0f);
	ImGui::ColorEdit3("rim color", light_rim_color_, ImGuiColorEditFlags_Float);
	ImGui::DragFloat("rim exponent", &light_rim_exponent_, 0.01f);
	ImGui::End();
}

//�`��
void Light::draw() const 
{
	// �J�����̈ʒu�����C�g�̈ʒu�ɐݒ肷��i�|�C���g���C�g�j
	float light_position[4]{ Camera_->transform().position().x,Camera_->transform().position().y, Camera_->transform().position().z,1.0f};

	// ���C�g�p�����[�^�̐ݒ�
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);  // ���C�g�̈ʒu
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient_);  // �����J���[�̐ݒ�
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_);  // �g�U���ˌ��J���[�̐ݒ�
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular_); // ���ʔ��ˌ��J���[�̐ݒ�
	// ���C�g�̌����p�����[�^�̐ݒ�
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, light_constant_attenuation_);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, light_linear_attenuation_);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, light_quadtatic_attenuation_);

	// �������C�g�̐ݒ�
	gsSetRimLightColor(&light_rim_color_);
	gsSetRimLightExponent(light_rim_exponent_);
	// �⏕���C�g���o�C���h
	gsBindAuxLight(0);

	static float position[]{ 10.0f, 10.0f, 10.0f, 0.0f };
	// �V���h�E�}�b�v�̃��C�g�̈ʒu��ݒ�
	GSvector3 shadow_map_light_position{ position[0], position[1], position[2] };
	gsSetShadowMapLightPosition(&shadow_map_light_position);

}