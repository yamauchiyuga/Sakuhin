#pragma once
#ifndef TITLE_DRAGON_H_
#define TITLE_DRAGON_H_

#include"Enemy.h"
#include"../AnimatedMesh.h"
#include<memory>

class TitleDragon:public Enemy
{
public:
	//
	TitleDragon(std::shared_ptr<IWorld> world, const GSvector3& position);
	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;
private:
	//アニメーションメッシュ
	AnimatedMesh	mesh_;
	//モーション番号
	GSuint motion_;
};

#endif // !TITLE_DRAGON_H_
