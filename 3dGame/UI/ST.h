#ifndef ST_H_
#define ST_H_

class ST
{
public:
	//コンストラクタ
	ST(float max_stamina);
	//更新
	void update(float delta_time);
	//描画
	void draw()const;
	//スタミナ消費の処理
	bool consumption_stamina(const int  consumption_stamina);
	//現在のスタミナを返す
	int get_stamina()const;
private:
	//現在のスタミナ
	float cullent_stamina_;
	//最大スタミナ
	const float max_stamina_;

};

#endif // !ST_H_
