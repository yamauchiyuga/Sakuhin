#ifndef PLAYER_H_
#define PLATER_H_

#include"../Actor.h"
#include"../AnimatedMesh.h"
#include"../../UI/HP.h"
#include"../../UI/ST.h"

//プレーヤークラス
class Player :public Actor {

public:
	//プレーヤー状態
	enum class State {
		Move,			//移動中
		Attack,			//攻撃中
		Dodge,			//回避中
		GuardStart,		//ガード始まり
		Guarding,		//ガード中
		GuardEnd,		//ガード終わり
		Damage,			//ダメージ中
		End				//死亡中
	};

	//コンストラクタ
	Player(IWorld* world, const GSvector3& position = GSvector3{ 0.0f, 0.0f, 0.0f });
	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;
	//
	virtual void draw_gui()const override;
	//衝突リアクション
	virtual void react(Actor& other)override;

private:
	//状態の更新
	void update_state(float delta_time);
	//状態の変更
	void change_state(State state, GSuint motion, bool loop = true);
	//移動処理
	void move(float delta_time);

	//攻撃中
	void attack(float delta_time);
	//
	void dodge(float delta_time);
	//
	void guard_start(float delta_time);
	//
	void guarding(float delta_time);
	//
	void guard_end(float delta_time);

	//ダメージ中
	void damage(float delta_time);
	//
	void end(float delta_time);
	//
	void turn();
	//
	bool can_attackable()const;
	//
	bool can_guard()const;
	//
	void knock_back(Actor& other, float power);
	//
	void collide_actor(Actor& other);
	//
	void collide_field();
	//
	void generate_attac_collider();

private:
	//アニメーションメッシュ
	AnimatedMesh	mesh_;
	//モーション番号
	GSuint motion_;
	//モーションのループ設定
	bool motion_loop_;
	//状態
	State state_;
	//状態タイマ
	float state_timer_;
	//
	int combo_;
	//
	HP HP_;
	//
	ST ST_;
};
#endif