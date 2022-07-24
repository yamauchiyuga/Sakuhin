#ifndef SKELETON_H_
#define SKELETON_H

#include"Enemy.h"
#include"../AnimatedMesh.h"
#include<memory>

class Skeleton :public Enemy 
{
public:
	//状態
	enum  class State
	{
		Generation,
		Idle,
		Run,
		Turn,
		Dead,
		ShieldBlock,
		Damage,
		Attack
	};
	//コンストラクタ
	Skeleton(std::shared_ptr<IWorld> world, const GSvector3& position);
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
	//生成
	void generation(float delta_time);
	//アイドル
	void idle(float delta_time);
	//走る
	void run(float delta_time);
	//振り向き中
	void turn(float delta_time);
	//
	void Shield_Block(float delta_time);
	//死亡
	void dead(float delta_time);
	//攻撃中
	void attack(float delta_time);
	//
	void damage(float delta_time);
	//攻撃
	void slash();
	//走るか？
	bool is_run()const;
	//振り向くか？
	bool is_trun()const;
	//攻撃範囲か？
	bool is_attack()const;

	//アニメーションメッシュ
	AnimatedMesh	mesh_;
	//モーション番号
	GSuint motion_;
	//状態
	State state_;
};

#endif // !SKELETON_H_

