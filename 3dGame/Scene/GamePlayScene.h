#ifndef GAMEPLAYSCENE_H_
#define GAMEPLAYSCENE_H_

#include "IScene.h"
#include "../World/World.h"
#include<gslib.h>

class GamePlayScene :public IScene {
public:
	//
	virtual void start()override;
	//
	virtual void update(float delta_time)override;
	//
	virtual void draw()const override;
	//
	virtual bool is_end()const override;
	//
	virtual std::string next() const override;
	//
	virtual void end()override;
private:
	//
	World world_;
	//
	bool is_end_{ false };
};


#endif
