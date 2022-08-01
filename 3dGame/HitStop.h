#ifndef HIT_STOP_H_
#define HIT_STOP_H_
#include<memory>
#include"Timer.h"
class HitStop
{
public:
	HitStop()=default;
	~HitStop() = default;
	void update(float delta_time);
	void set_hit_stop(const float time);
	float delta_time_delay()const;
private:
	static std::unique_ptr<Timer> timer_;
	float delay_time_{1.0f};
};
#endif // !HIT_STOP_H_