#ifndef TITLE_PLAYER_H_
#define TITLE_PLAYER_H_

#include "../Actor.h"
#include "../AnimatedMesh.h"
#include<memory>

class TitlePlayer :public Actor
{
public:
	//
	TitlePlayer(std::shared_ptr<IWorld> world, const GSvector3& position);
	//
	virtual void update(float delta_time)override;
	//
	virtual void draw()const override;
	//フィールド判定
	void collide_field();
private:
	//アニメーション付きメッシュ
	AnimatedMesh mesh_;
	//モーション番号
	GSuint motion_;
	//モーションのループフラグ
	bool motion_loop_;

};

#endif // !TITLE_PLAYER_H_
