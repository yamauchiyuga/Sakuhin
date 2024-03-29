#include "Light.h"
#include<imgui/imgui.h>
#include"../World/IWorld.h"
#include"../Camera/CameraTPS.h"
//コンストラクタ
Light::Light(std::shared_ptr<IWorld> world) :
	// ライトのパラメータ
	light_ambient_{ 0.0f, 0.0f, 0.0f, 0.0f },
	light_diffuse_{ 0.9f, 0.5f, 0.2f, 1.0f },
	light_specular_{ 0.9f, 0.5f, 0.2f, 1.0f },
	light_constant_attenuation_{ 1.0f },
	light_linear_attenuation_{ 0.07f },
	light_quadtatic_attenuation_{ 0.07f },
	light_rim_color_{ 255,255,255,1 },
	light_rim_exponent_{ 0.0f }
{
	world_ = world;
	Camera_ = world_->camera();
	name_ = "Light";
	tag_ = "LightTag";

}


//描画
void Light::draw() const 
{
	// カメラの位置をライトの位置に設定する（ポイントライト）
	float light_position[4]{ Camera_->transform().position().x,Camera_->transform().position().y, Camera_->transform().position().z,1.0f};

	// ライトパラメータの設定
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);  // ライトの位置
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient_);  // 環境光カラーの設定
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_);  // 拡散反射光カラーの設定
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular_); // 鏡面反射光カラーの設定
	// ライトの減衰パラメータの設定
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, light_constant_attenuation_);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, light_linear_attenuation_);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, light_quadtatic_attenuation_);

	// リムライトの設定
	gsSetRimLightColor(&light_rim_color_);
	gsSetRimLightExponent(light_rim_exponent_);
	// 補助ライトをバインド
	gsBindAuxLight(0);

	static float position[]{ 0.0f, 10.0f, 0.0f, 0.0f };
	// シャドウマップのライトの位置を設定
	GSvector3 shadow_map_light_position{ position[0], position[1], position[2] };
	gsSetShadowMapLightPosition(&shadow_map_light_position);

}