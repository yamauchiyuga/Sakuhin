#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include"../Actor.h"
#include<memory>
class Explosion:public Actor {
public:
	//
	Explosion(std::shared_ptr<IWorld> world, const GSvector3& position,const float delay_timer=0.0f);
	//
	virtual void update(float delta_time)override;
	//
	virtual void draw()const override;
	//
	virtual void react(Actor& other)override;
private:
	//
	float lifespan_timer_;
	//
	float delay_timer_;
};

#endif // !Explosion


