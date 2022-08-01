#ifndef PLAYER_H_
#define PLATER_H_

#include"../Actor.h"
#include"../AnimatedMesh.h"
#include"../../UI/HP.h"
#include"../../UI/ST.h"
#include"../../Timer.h"
#include<memory>

//プレーヤークラス
class Player :public Actor {
public:
	//プレーヤー状態
	enum class State {
		Move,			
		Attack,		   
		Dodge,		   
		GuardStart,  
		Guarding,	  
		Damage,	 
		End		     
	};

	//コンストラクタ
	Player(std::shared_ptr<IWorld> world, const GSvector3& position);
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
	//回避中
	void dodge(float delta_time);
	//ガード始まり
	void guard_start(float delta_time);
	//攻撃ヒット
	void guarding(float delta_time);
	//ダメージ中
	void damage(float delta_time);
	//死亡
	void end(float delta_time);
	//ノックバック処理
	void knock_back(Actor& other, float power);
	//攻撃可能か？
	bool can_attackable()const;
	//ガード可能か？
	bool can_guard()const;
	//死亡ラインか？
	bool end_line()const;
	//アクター判定
	void collide_actor(Actor& other);
	//フィールド判定
	void collide_field();
	//攻撃判定生成
	void generate_attack_collider();

private:
	//アニメーションメッシュ
	AnimatedMesh	mesh_;
	//モーション番号
	GSuint motion_;
	//ステータス
	HP HP_;
	ST ST_;
	//状態
	State state_;
	//状態タイマ
	float state_timer_;
	float easing_time_{0.0f};
	//コンボ数
	int combo_;
	//モーションのループ設定
	bool motion_loop_;
	//死亡ライン
	bool det_line_;
};
#endif