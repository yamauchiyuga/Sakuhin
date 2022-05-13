#include "Light.h"

//コンストラクタ
Light::Light(IWorld* world) {
	world_ = world;
	name_ = "Light";
	tag_ = "LightTag";
}

//描画
void Light::draw() const {
	constexpr float ambient[]{ 0.1f, 0.1f, 0.1f, 1.0f };
	constexpr float diffuse[]{ 0.4f, 0.4f, 0.4f, 1.0f };
	constexpr float specular[]{ 0.2f, 0.2f, 0.2f, 1.0f };
	constexpr float position[]{ 10.0f, 10.0f, 10.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	// シャドウマップ用のライトの位置を設定
	GSvector3 shadow_map_light_position{ position[0], position[1], position[2] };
	gsSetShadowMapLightPosition(&shadow_map_light_position);

}