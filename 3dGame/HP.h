#ifndef HP_H_
#define HP_H_

class HP
{
public:
	//
	HP(float max_health);
	//
	//void update(float delta_time);
	//
	void draw()const;
	//
	void hit_damage(int power);
	//
	bool is_end()const;
private:
	//åªç›ÇÃHP
	float cullent_health_;
	//
	const float max_health_;
};


#endif 
