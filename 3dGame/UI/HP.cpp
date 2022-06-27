#include "HP.h"
#include"../Assets.h"

//コンストラクタ
HP::HP(const float max_health):
	max_health_{ max_health },
	cullent_health_{ max_health }
{
}

//ダメージ処理
void HP::hit_damage(int power) {
	//0未満にならないにしている
	cullent_health_ = MAX(cullent_health_ - power, 0);
}
//プレイヤー用
void HP::draw_player() const{
	//HPゲージの位置
	const GSvector2 position{ 80.0f, 70.0f };
	//ゲージのフレームの位置
	const GSvector2 framePosition{ 79.0f, 69.0f };
	//HPゲージ本体の矩形
	const GSrect rect{ 0, 0, (cullent_health_ / max_health_) * 250.0f, 13.0f };
	//背景の矩形
	const GSrect back_rect{ 0,0,250.0f, 13.0f };
	//HPゲージの色
	const GScolor barColor{ 0,1,0,1 };
	//背景の色
	const GScolor backColor{ 0,0,0,1 };
	//フレームの描画
	gsDrawSprite2D(Texture_Frame, &framePosition, NULL, NULL, NULL, NULL, 0.0f);
	//背景の描画
	gsDrawSprite2D(Texture_HP, &position, &back_rect, NULL, &backColor, NULL, 0.0f);
	//HPゲージの描画
	gsDrawSprite2D(Texture_HP, &position, &rect, NULL, &barColor, NULL, 0.0f);
}

//ボス用
void HP::draw_boss() const{
	//HPゲージの位置
	const GSvector2 Position{ 250.0f, 650.0f };
	//ゲージのフレームの位置
	const GSvector2 FramePosition{ 248.0f, 649.0f };
	//HPゲージ本体の矩形
	const GSrect Rect{ 0, 0, (cullent_health_ / max_health_) * 750.0f, 10.0f };
	//背景画像の矩形
	const GSrect BackRect{ 0,0,750.0f, 10.0f };
	//フレームの矩形
	const GSrect FrameRect{ 0,0, 754.0f, 12.0f };
	//HPゲージの色
	const GScolor HealthColor{ 1,0,0,1 };
	//背景の色
	const GScolor BackColor{ 0,0,0,1 };
	//フレームの色
	const GScolor FrameColor{ 1,1,1,1 };

	//フレームの描画
	gsDrawSprite2D(Texture_HP, &FramePosition, &FrameRect, NULL, &FrameColor, NULL, 0.0f);
	//背景の描画
	gsDrawSprite2D(Texture_HP, &Position, &BackRect, NULL, &BackColor, NULL, 0.0f);
	//HPゲージの描画
	gsDrawSprite2D(Texture_HP, &Position, &Rect, NULL, &HealthColor, NULL, 0.0f);
}
//現在のHP を返す
float HP::cullent_health()const {
	return cullent_health_;
}
//死亡してるか返す
bool HP::is_end()const {
	if (cullent_health_ <= 0)return true;
	return false;
}