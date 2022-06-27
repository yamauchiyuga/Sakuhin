#ifndef LIGHT_H_
#define LIGHT_H_

#include "Actor.h"
#include<memory>
// ライトクラス
class Light : public Actor
{
public:
	// コンストラクタ
	Light(std::shared_ptr<IWorld> world);
	//
	virtual void update(float delta_time)override;
	// 描画
	virtual void draw() const override;
private:
	// ライトのパラメータ
	float light_ambient_[4];
	float light_diffuse_[4];
	float light_specular_[4];
	float light_constant_attenuation_;
	float light_linear_attenuation_;
	float light_quadtatic_attenuation_;
	// リムライトのパラメータ
	GScolor light_rim_color_;
	float   light_rim_exponent_;
	//カメラ取得
	std::shared_ptr<Actor> Camera_;
};

#endif