#ifndef DRAGON_H_
#define DRAGON_H_

#include"Enemy.h"
#include"../AnimatedMesh.h"
#include<string>
#include<memory>

class Dragon : public Enemy 
{
public:
	//状態
	enum class State 
	{
		Idle,
		Run,
		Turn,
		FlyStart,
		FlyMove,
		FlyIdel,
		FlyAttack,
		FlyEnd,
		Damage,
		Dead,
		Attack
	};
	//コンストラクタ
	Dragon(std::shared_ptr<IWorld> world, const GSvector3& position);
	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;
	//GUI描画
	virtual void draw_gui()const override;
	//衝突判定
	virtual void react(Actor& other)override;

private:
	//状態の更新
	void update_state(float delta_time);
	//状態の変更
	void change_state(State state, int motion, bool loop = true);
	//アイドル
	void idle(float delta_time);
	//走る
	void run(float delta_time);
	//振り向く
	void turn(float delta_time);
	//攻撃
	void attack(float delta_time);
	//飛行始め
	void fly_start(float delta_time);
	//飛行中
	void fly_idle(float delta_time);
	//飛行移動
	void fly_move(float delta_time);
	//飛行攻撃
	void fly_attack(float delta_time);
	//飛行終了
	void fly_end(float delta_time);
	//
	void damage(float delta_time);
	//死亡
	void dead(float delta_time);

	//攻撃の選択
	void attack_selection();
	//噛みつき
	void bite();
	//尻尾攻撃
	void tail_attack();
	//火球
	void spit_fire();
	//振り向くか？
	bool is_trun()const;
	//走るか？
	bool is_run()const;
	//攻撃範囲か？
	bool is_attack()const;
	//重力を有効にするか？
	bool is_gravity()const;
private:
	//アニメーションメッシュ
	AnimatedMesh	mesh_;
	//モーション番号
	GSuint motion_;
	//状態
	State state_;
};

#endif // !DRAGON_H_














