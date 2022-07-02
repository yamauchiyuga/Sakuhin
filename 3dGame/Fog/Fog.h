#ifndef FOG_H_
#define FOG_H_

class Fog
{
public:
	Fog();
	//描画
	void draw()const;
private:
	// フォグのパラメータ
	float   fog_color_[4];
	float   fog_start_;
	float   fog_end_;
};

#endif // !FOG_H_

