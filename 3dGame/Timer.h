#ifndef TIMER_H_
#define TIMER_H_

#include<algorithm>
class Timer
{
public:
	//
	Timer(const float limitTime) :
		time_(0.0f), limitTime_(limitTime) {
	}
	//
	void update(float delta_time)
	{
		time_ = std::min(time_ + delta_time, limitTime_);
	}

	void set_limit_time(const float& time)
	{
		limitTime_ = time;
	}

	void reset()
	{
		time_ = 0;
		limitTime_ = 0.0f;
	}
	//
	bool is_time_out()const
	{
		return time_ >= limitTime_;
	}
private:
	//
	float  time_;
	//
	float limitTime_;

};

#endif // !TIMER_H_



