#include "HitStop.h"
#include<algorithm>
//std::unique_ptr<Timer>HitStop::timer_;
Timer HitStop::timer_(0.0f);

void HitStop::update(float delta_time)
{
   // if (!timer_)return;
    timer_.update(delta_time);
    if(timer_.is_time_out())
    {
        delay_time_ = 1.0f;
        timer_.reset();
    }
    else
    {
        delay_time_=0.0f;
    }
}

void HitStop::set_hit_stop(const float& time)
{
    timer_.set_limit_time(time);
}

float HitStop::delta_time_delay()const
{
    return delay_time_;
}
