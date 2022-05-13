#ifndef ST_H_
#define ST_H_

class ST
{
public:
	//
	ST(float max_stamina);
	//
	void update(float delta_time);
	//
	void draw()const;
	//
	bool consumption_stamina(int  consumption_stamina);
	//
	int get_stamina()const;
private:
	//
	float cullent_stamina_;
	//
	const float max_stamina_;
	//
	int consumption_stamina_;

};

#endif // !ST_H_


