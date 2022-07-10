#ifndef CAMERA_TPS_H_
#define CAMERA_TPS_H_

#include "../Actor/Actor.h"
#include<memory>
#include<string>
class CameraTPS :public Actor
{
public:
	//コンストラクタ
	CameraTPS(std::shared_ptr<IWorld> world, const GSvector3& position, const GSvector3& at);
	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;

private:
	//入力更新
	void input_update();
	//状態の更新
	void state_update(float delta_time);
	//プレイヤー
	void player_update();
	//プレイヤーをロックオン
	void player_lock_on(float delta_time);
	//敵をロックオン
	bool enemy_lock_on(float delta_time);
	//敵からプレイヤーへ視点を戻す
	bool lock_on_enemy_to_player(float delta_taime);
	//ターゲットが死んでるか
	bool decide_targe();
	//フィールドの衝突判定
	void field_react();
	//視点の設定
	void set_parameter();
	//ターゲットを取得
	void set_target(const std::shared_ptr<Actor>& target);
	//注視点の補完
	void set_reference_point(const GSvector3& at);
	//視点の回転の処理
	void operation_processor(float delta_time);
	//視点移動の補完
	void linear_interpolation(const float time, const float max_speed, float delta_time);
	//プレイヤーが死んでるか
	bool is_player_dead() const;
private:
	
	std::shared_ptr<Actor> player_;
	std::shared_ptr<Actor> target_;

	GSvector3 at_;
	GSvector3 position_;

	int state_;

	float timer_{ 0.0f };
	float yaw_{ 0.0f };
	float pitch_{ 0.0f };
	float input_horizontal_amount_{ 0.0f };
	float input_vertical_amount_{ 0.0f };	

};

#endif // !CAMERA_TPS_H_
