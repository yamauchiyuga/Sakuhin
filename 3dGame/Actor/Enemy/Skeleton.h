#ifndef SKELETON_H_
#define SKELETON_H

#include"Enemy.h"
#include"../AnimatedMesh.h"
#include<memory>

class Skeketon :public Enemy {
public:
	enum  class State
	{
		Generation,
		Idle,
		Run,
		Turn,
		Dead,
		Attack
	};
	//
	Skeketon(std::shared_ptr<IWorld> world, const GSvector3& position);
	//
	virtual void update(float delta_time)override;
	//
	virtual void draw()const override;
	//
	virtual void react(Actor& other)override;
private:
	//
	void update_state(float delta_time);
	//
	void change_state(State state, int motion, bool loop = true);
	//
	void generation(float delta_time);
	//
	void idle(float delta_time);
	//
	void run(float delta_time);
	//
	void turn(float delta_time);
	//
	void dead(float delta_time);
	//
	void attack(float delta_time);
	//
	void slash();
	//
	bool is_run()const;
	//
	bool is_trun()const;
	//
	bool is_attack()const;

	//アニメーションメッシュ
	AnimatedMesh	mesh_;
	//モーション番号
	GSuint motion_;
	//
	State state_;
	//
	float attack_time_;


};

#endif // !SKELETON_H_

