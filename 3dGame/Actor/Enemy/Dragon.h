#ifndef DRAGON_H_
#define DRAGON_H_

#include"Enemy.h"
#include"../AnimatedMesh.h"
#include"../../UI/HP.h"

class Dragon : public Enemy {
public:
	enum class State {
		Idle,
		Run,
		Turn,
		FlyStart,
		FlyMove,
		FlyIdel,
		FlyAttack,
		FlyEnd,
		Dead,
		Attack
	};
	//
	Dragon(IWorld* world, const GSvector3& position);
	//
	virtual void update(float delta_time)override;
	//
	virtual void draw()const override;
	//
	virtual void draw_gui()const override;
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
	void turn(float delta_time);
	//
	void attack(float delta_time);
	//
	void fly_start(float delta_time);
	//
	void fly_idle(float delta_time);
	//
	void fly_move(float delta_time);
	//
	void fly_attack(float delta_time);
	//
	void fly_end(float delta_time);
	//
	void dead(float delta_time);

	//
	void attack_selection();
	//
	void bite();
	//
	void tail_attack();
	//
	void spit_fire();

	//
	bool is_trun()const;
	//
	bool is_run()const;
	//
	bool is_attack()const;
private:
	//アニメーションメッシュ
	AnimatedMesh	mesh_;
	//モーション番号
	GSuint motion_;
	//
	State state_;
	//
	HP HP_;
};

#endif // !DRAGON_H_














