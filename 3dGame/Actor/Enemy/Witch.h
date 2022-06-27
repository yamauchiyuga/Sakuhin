#ifndef WITCH_H_
#define WITCH_H_


#include "Enemy.h"
#include"../AnimatedMesh.h"
#include<memory>
class Witch : public Enemy{
public :
	enum class State{
		Idle,
		Run,
		Dead,
		Attack
	};

	//
	Witch(std::shared_ptr<IWorld> world, const GSvector3& position);
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
	void idle(float delta_time);
	//
	void run(float delta_time);
	//
	void dead(float delta_time);
	//
	void attack(float delta_time);
	//
	void attack_selection();
	//
//	void explosion();
	//
	void spit_fire();
	//
	bool is_run()const;
	//
	bool is_attack()const;

	bool hit_wall;
	//アニメーションメッシュ
	AnimatedMesh	mesh_;
	//モーション番号
	GSuint motion_;
	//
	State state_;
	//
};



#endif // !



