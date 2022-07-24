#ifndef FIRE_SPHERE_H_
#define FIRE_SPHERE_H_

#include<memory>
#include<string>
#include"../Actor.h"

class FireSphere :public Actor {
public:
	//
	FireSphere(std::shared_ptr<IWorld>world, const GSvector3& position, const GSvector3& velocity,const std::string& attack_name);
	//
	virtual void update(float delta_time)override;
	//
	virtual void draw()const override;
	//
	virtual void react(Actor& other)override;
private:
	//
	int effect_;
};

#endif // !Fire_Sphere_H_

