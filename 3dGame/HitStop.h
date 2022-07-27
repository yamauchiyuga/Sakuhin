#ifndef HIT_STOP_H_
#define HIT_STOP_H_

class HitStop
{
public:
	HitStop()=default;
	~HitStop() = default;
	void update(float delta_time);
	void set_hit_stop(const float time);
	float b()const;
private:
	float ab_{1.0f};
	static float time_;
};
#endif // !HIT_STOP_H_