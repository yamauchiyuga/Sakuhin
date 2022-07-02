#ifndef GAMEPLAYSCENE_H_
#define GAMEPLAYSCENE_H_

#include "IScene.h"
#include "../World/World.h"
#include"../EnemyGenerator/EnemyGenerator.h"
#include"../Fog/Fog.h"
#include<gslib.h>
#include<memory>

class GamePlayScene :public IScene {
public:
	//スタート
	virtual void start()override;
	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;
	//終了か？
	virtual bool is_end()const override;
	//次のシーン
	virtual std::string next() const override;
	//終了
	virtual void end()override;

private:
	//ワールド
	std::shared_ptr<World> world_;
	//フォグ
	Fog fog_;
	//エネミー生成
	std::shared_ptr<EnemyGenerator> enemy_generator_;
	//終了かどうかのフラグ
	bool is_end_{ false };
};


#endif
