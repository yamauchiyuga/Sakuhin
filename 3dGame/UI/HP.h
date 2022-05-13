#ifndef HP_H_
#define HP_H_

#include<GSvector2.h>
#include<GSgraphics.h>

class HP
{
public:
	HP() = default;
	//
	HP(const float max_health, const GSvector2 HP_pos, const GSvector2 frame_pos,const float gauge_length);
	//
	void draw()const;
	//
	void hit_damage(int power);
	//
	float cullent_health()const;
	//
	bool is_end()const;

private:
	//
	GSvector2 HP_pos_;
	//
	GSvector2 frame_pos_;
	//åªç›ÇÃHP
	float cullent_health_;
	//
	float max_health_;
	//
	float gauge_length_;



};


#endif 
