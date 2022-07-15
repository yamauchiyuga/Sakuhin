#include "ST.h"
#include"../Assets.h"
#include"UIparameter.h"
#include<GSvector2.h>
#include<GSgraphics.h>
#include<GSmath.h>

//クールタイム
float Count{ 0.0f };
//クールタイムの時間
const int Latency{ 80 };

ST::ST(float max_stamina) :
	max_stamina_{ max_stamina },
	cullent_stamina_{ max_stamina }
{
};
//更新
void ST::update(float delta_time)
{
	//クールタイムより大きかったらスタミナ回復 
	if (Count > Latency) cullent_stamina_ = MIN(cullent_stamina_ + delta_time, max_stamina_);
	Count += delta_time;
}
//スタミナ消費の処理
bool ST::consumption_stamina(const int consumption_stamina) {
	//現在のスタミナが消費より小さいか？
	if (cullent_stamina_ < consumption_stamina) return false;
	cullent_stamina_ = MAX(cullent_stamina_ - consumption_stamina, 0.0f);
	//タイマーの初期化
	Count = 0;
	return true;
}
//現在のスタミナを返す
int ST::get_stamina()const {
	return cullent_stamina_;
}
//描画
void ST::draw()const {

	UIparameter Player;
	Player.gage_position = { 80.0f, 85.0f };
	Player.gage_frame_position = { 79.0f, 84.0f };
	Player.gage = { 0, 0, (cullent_stamina_ / max_stamina_) * 250.0f, 13.0f };
	Player.back_gage = { 0,0,250.0f, 13.0f };
	Player.gage_color = { 1,1,0,1 };
	Player.gage_frame_color = { 1,1,1,1 };
	Player.back_gage_color = { 0,0,0,1 };

	//フレームの描画
	gsDrawSprite2D(Texture_Frame, &Player.gage_frame_position, NULL, NULL, &Player.gage_frame_color, NULL, 0.0f);
	//背景の描画
	gsDrawSprite2D(Texture_HP, &Player.gage_position, &Player.back_gage, NULL, &Player.back_gage_color, NULL, 0.0f);
	//HPゲージの描画
	gsDrawSprite2D(Texture_HP, &Player.gage_position, &Player.gage, NULL, &Player.gage_color, NULL, 0.0f);
}
