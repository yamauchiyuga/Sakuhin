#ifndef HP_H_
#define HP_H_

#include<GSvector2.h>
#include<GSgraphics.h>

class HP
{
public:
	HP() = default;
	//
	HP(const float max_health);
	//
	void draw(const GSvector2 HP_pos, const GSvector2 frame_pos, const float gauge_length, const GScolor color)const;
	//
	void draw_player()const;
	//
	void draw_boss()const;
	//
	void hit_damage(int power);
	//
	float cullent_health()const;
	//
	bool is_end()const;

private:
	//åªç›ÇÃHP
	float cullent_health_;
	//
	float max_health_;

};


#endif 
