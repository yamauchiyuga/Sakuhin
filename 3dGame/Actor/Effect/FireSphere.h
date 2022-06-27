#ifndef FIRE_SPHERE_H_
#define FIRE_SPHERE_H_

#include<memory>
#include"../Actor.h"

class FireSphere :public Actor {
public:
	//
	FireSphere(std::shared_ptr<IWorld>world, const GSvector3& position, const GSvector3& velocity);
	//
	virtual void update(float delta_time)override;
	//
	virtual void draw()const override;
	//
	virtual void react(Actor& other)override;
private:
	//
	float lifespan_timer_;
};

#endif // !Fire_Sphere_H_

