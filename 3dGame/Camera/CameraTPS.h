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
	void input_update();
	void state_update(float delta_time);
	void player_update();

	void player_lock_on(float delta_time);
	bool enemy_lock_on(float delta_time);
	bool lock_on_enemy_to_player(float delta_taime);

	bool decide_targe();
	void field_react();
	void set_parameter();
	void set_target(const std::shared_ptr<Actor>& target);

	void set_reference_point(const GSvector3& at);
	void operation_processor(float delta_time);
	void linear_interpolation(const float time, const float max_speed, float delta_time);
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

	std::string x, y, z;
};

#endif // !CAMERA_TPS_H_
