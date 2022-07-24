#ifndef WITCH_H_
#define WITCH_H_


#include "Enemy.h"
#include"../AnimatedMesh.h"
#include<string>
#include<memory>
class Witch : public Enemy{
public :
	enum class State{
		Generation,
		Idle,
		Run,
		Dead,
		Attack,
		Damage
	};

	//コンストラクタ
	Witch(std::shared_ptr<IWorld> world, const GSvector3& position);
	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;
	//衝突判定
	virtual void react(Actor& other)override;
private:
	//状態の更新
	void update_state(float delta_time);
	//状態の変更
	void change_state(State state, int motion, bool loop = true);
	//
	void generation(float delta_time);
	//アイドル
	void idle(float delta_time);
	//走る
	void run(float delta_time);
	//
	void damage(float delta_time);
	//死亡
	void dead(float delta_time);
	//攻撃
	void attack(float delta_time);
	//次の攻撃
	void attack_selection();
	//
	void thunder();
	//火球
	void spit_fire();
	//走るか？
	bool is_run()const;
	//攻撃範囲か？
	bool is_attack()const;
	
	//アニメーションメッシュ
	AnimatedMesh	mesh_;
	//モーション番号
	GSuint motion_;
	//状態
	State state_;
	//壁にぶつかったか？
	bool hit_wall;
};



#endif // !



