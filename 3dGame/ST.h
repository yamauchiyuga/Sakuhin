#ifndef ST_H_
#define ST_H_

class ST
{
	//
	ST(float max_stamina);
	//
	void update(float delta_time);
	//
	void draw()const;
	//
	bool dodge_stamina(int dodge_stamina);
	//
	int get_stamina()const;
private:
	//
	float cullent_stamina_;
	//
	const float max_stamina_;
	//
	int dodge_stamina_;

};

#endif // !ST_H_


