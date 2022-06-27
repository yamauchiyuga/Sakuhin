#ifndef HP_H_
#define HP_H_

#include<GSvector2.h>
#include<GSgraphics.h>

class HP
{
public:
	//コンストラクタ
	HP() = default;
	//
	HP(const float max_health);
	//プレイヤー用
	void draw_player()const;
	//ボス用
	void draw_boss()const;
	//ダメージ処理
	void hit_damage(int power);
	//現在のHPを返す
	float cullent_health()const;
	//死亡したか？
	bool is_end()const;

private:
	//現在のHP
	float cullent_health_;
	//最大HP 
	float max_health_;

};


#endif 
