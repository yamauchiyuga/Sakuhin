#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include"../Actor.h"

class Explosion:public Actor {
public:
	//
	Explosion(IWorld* world, const GSvector3& position,const float delay_timer);
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


