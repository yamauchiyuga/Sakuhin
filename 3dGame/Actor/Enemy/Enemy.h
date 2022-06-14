#ifndef ENEMY_H_
#define ENEMY_H_
#include"../Actor.h"
#include"../../UI/HP.h"


//
class Enemy :public Actor {
public:
	Enemy() = default;
	virtual~Enemy() = default;

protected:
	// ターゲット方向の角度を求める（符号付き）
	float target_signed_angle() const;
	// ターゲット方向の角度を求める（符号なし）
	float target_angle() const;
	// ターゲットの距離を求める
	float target_distance() const;
	//アクターとの衝突リアクション
	void collide_actor(Actor& other);
	//フィールドとの衝突リアクション
	void collide_field();
	//攻撃判定を生成
	void generate_attac_collider(const float radius, const float distance, const float height,const float width,const float delay,const float life_span);

public:

	//
	bool dead()const;

	//コピー禁止
	Enemy(const Enemy& other) = delete;
	Enemy& operator = (const Enemy& other) = delete;

protected:
	
	//モーションのループ設定
	bool motion_loop_{true};
	// 状態タイマ
	float state_timer_{0.0f};
	//
	bool is_dead_{ false };
	//プレイヤー
	Actor* player_{nullptr};
	//
	HP HP_;
};

#endif // !ENEMY_H_
