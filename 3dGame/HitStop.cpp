#include "HitStop.h"
#include<algorithm>
#include"Timer.h"
float HitStop::time_{ 0.0f };

void HitStop::update(float delta_time)
{
    time_ = std::max(time_-delta_time, 0.0f);
    if(time_>0)
    {
        ab_ = 0.005f;
    }
    else
    {
        ab_ = 1.0f;
    }
}

void HitStop::set_hit_stop(const float time)
{
    time_ = time;
}

float HitStop::b()const
{
    return ab_;
}
