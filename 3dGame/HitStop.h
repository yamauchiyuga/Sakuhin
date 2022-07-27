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
	float b()const;
private:
	static std::unique_ptr<Timer> timer_;
	float ab_{1.0f};
};
#endif // !HIT_STOP_H_