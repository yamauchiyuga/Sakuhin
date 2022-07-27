#include "HitStop.h"
#include<algorithm>
std::unique_ptr<Timer>HitStop::timer_;

void HitStop::update(float delta_time)
{
    if (!timer_)return;
    timer_->update(delta_time);
    if(timer_->is_time_out())
    {
        ab_ = 1.0f;
        timer_->reset();
    }
    else
    {
        ab_=0.05f;
    }
}

void HitStop::set_hit_stop(const float time)
{
    timer_.reset(new Timer(time));
}

float HitStop::b()const
{
    return ab_;
}
