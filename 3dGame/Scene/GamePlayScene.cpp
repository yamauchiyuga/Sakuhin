#include "GamePlayScene.h"
#include"../Collision/Field.h"
#include "../Collision/Field.h"
#include "../Camera/CameraTPS.h"
#include "../Actor/Light.h"
#include"../Actor/Player/Player.h"
#include"../Assets.h"
#include"../Actor/Enemy/Skeleton.h"
#include"../Actor/Enemy/Dragon.h"
#include<imgui/imgui.h>
#include <GSstandard_shader.h>
#include<GSeffect.h>

// 標準シェーダーの設定
#define GS_ENABLE_AUX_LIGHT                 // 補助ライトを有効にする
#define GS_ENABLE_BAKED_LIGHTMAP_SHADOW     // ベイクしたライトマップに影を落とす
#define GS_ENABLE_SOFT_SHADOW               // ソフトシャドウ（影の輪郭をぼかす）
#define GS_ENABLE_RIM_LIGHT                 // リムライトを有効にする

	// ポストエフェクトのパラメータ
static GScolor color_{ 1.0f, 1.0f, 1.0f, 1.0f };
static float   saturation_{ 1.0f };
static float   luminance_{ 1.4f };
static float   exposure_{ 0.8f };

void GamePlayScene::start() {
	// 終了フラグを初期化
	is_end_ = false;
	// エフェクトの初期化
	gsInitEffect();
	// 視錐台カリングを有効にする
	gsCreateRenderTarget(0, 1280, 720, GS_TRUE, GS_TRUE, GS_TRUE);
	//視錐台カリングを有効にする
	gsEnable(GS_FRUSTUM_CULLING);
	world_ = std::make_shared<World>();
	gsSetVolumeBGM(0.5f);
	gsPlayBGM(Sound_Wind);
	// シャドウマップの作成（２枚のカスケードシャドウマップ）
	static const GSuint shadow_map_size[] = { 2024, 1024 };
	gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
	// シャドウマップの減衰パラメータ
	gsSetShadowMapAttenuation(0.6f);
	// シャドウマップの距離を設定
	gsSetShadowMapDistance(60.0f);

	// エフェクトの読み込み（松明の炎）
	gsLoadEffect(Effect_TorchFlame, "Assets/Effect/Fire/Fire.efk");
	gsLoadEffect(Effect_Blood, "Assets/Effect/Blood.efk");
	gsLoadEffect(Effect_HitSpark, "Assets/Effect/HitSpark1.efk");
	gsLoadEffect(Effect_Explosion, "Assets/Effect/Explosion.efk");
	gsLoadEffect(Effect_FireBall, "Assets/Effect/Ball.efk");
	gsLoadEffect(Effect_Smoke, "Assets/Effect/Smoke.efk");



	// プレーヤーの追加
	world_->add_actor(std::make_shared<Player>(world_, GSvector3{ -5,4.0,10 }));
	world_->add_actor(std::make_shared<Dragon>(world_, GSvector3{ -5,4.0,50 }));
	// フィールドクラスの追加
	world_->add_field(std::make_shared<Field>(Octree_Stage, Octree_Collider));
	// カメラクラスの追加
	world_->add_camera(std::make_shared<CameraTPS>(
		world_, GSvector3{ 0.0f, 3.2f, -4.8f }, GSvector3{ 0.0f, 1.0, 0.0f }));
	// ライトクラスの追加
	world_->add_light(std::make_shared<Light>(world_));
	//ステージデータ読み込み
	enemy_generator_ = std::make_shared<EnemyGenerator>(world_, "Assets/StageData/StatgeData.csv", "Assets/StageData/FasePos.csv");

	// 炎のエフェクトを再生する(0～8が松明用ライト）
	for (GSint i = 0; i < 17; ++i) {
		// 補助ライトの位置を取得
		GSvector3 position;
		gsGetAuxLightPosition(Effect_TorchFlame, i, &position);
		// 補助ライトの位置に炎のエフェクトを出現させる
		gsPlayEffect(Effect_TorchFlame, &position);
	}


}

// 更新
void GamePlayScene::update(float delta_time) {
	// ワールドの更新
	world_->update(delta_time);
	//ジェネレータの更新
	//enemy_generator_->update(delta_time);
}

// 描画
void GamePlayScene::draw() const {
	// レンダーターゲットを有効にする
	gsBeginRenderTarget(0);
	// レンダーターゲットのクリア
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	// ワールドの描画
	world_->draw();
	//GUI描画
	world_->draw_gui();
	//fog描画
	fog_.draw();
	//レンダーターゲットを無効にする
	gsEndRenderTarget();

	//シェーダーを有効にする
	GScolor col = color_ * luminance_;
	col.a = 1.0f;
	gsBeginShader(0);
	// 彩度の設定
	gsSetShaderParam1f("u_Saturation", saturation_);
	// テクスチャの設定
	gsSetShaderParamTexture("u_RenderTexture", 0);
	// 色調整
	gsSetShaderParam4f("u_Color", &col);
	// 露出
	gsSetShaderParam1f("u_Exposure", exposure_);
	// レンダーターゲット用のテクスチャをバインド
	gsBindRenderTargetTextureEx(0, 0, 0);
	// レンダーターゲットを描画
	gsDrawRenderTargetEx(0);
	// レンダーターゲット用のテクスチャの解除
	gsUnbindRenderTargetTextureEx(0, 0, 0);
	// シェーダーを無効にする
	gsEndShader();
}

// 終了しているか？
bool GamePlayScene::is_end() const {
	return world_->is_game_over();         // 終了フラグを返す
}

// 次のシーンを返す
std::string GamePlayScene::next() const {
	return "TitleScene";    // タイトルシーンに戻る
}

// 終了
void GamePlayScene::end() {
	// ワールドを消去
	world_->clear();
	// 再生中の全エフェクトを停止（削除）する
	gsStopAllEffects();
	//BGM 削除
	gsDeleteBGM(Sound_Wind);
	//SE削除
	gsDeleteSE(Se_GameStart);
}
