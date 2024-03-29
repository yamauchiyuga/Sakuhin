#include "GamePlayScene.h"
#include"../Collision/Field.h"
#include "../Collision/Field.h"
#include "../Camera/CameraTPS.h"
#include "../Actor/Light.h"
#include"../Actor/Player/Player.h"
#include"../Assets.h"
#include"../Actor/Enemy/Skeleton.h"
#include"../Actor/Enemy/Dragon.h"
#include <GSstandard_shader.h>
#include<GSeffect.h>


void GamePlayScene::start() {
	// 終了フラグを初期化
	is_end_ = false;
	gsSetActiveAuxLightMax(8);
	// 視錐台カリングを有効にする
	gsCreateRenderTarget(0, 1280, 720, GS_TRUE, GS_TRUE, GS_TRUE);
	//視錐台カリングを有効にする
	gsEnable(GS_FRUSTUM_CULLING);
	world_ = std::make_shared<World>();
	gsSetVolumeBGM(0.5f);
	gsPlayBGM(Sound_Wind);

	// リフレクションプローブの読み込み(0番に読み込めば自動的に適用される）
	gsLoadReflectionProbe(0, "Assets/RefProbe/ReflectionProbe.txt");
	// ライトマップの読み込み(0番に読み込めば自動的に適用される）
	gsLoadLightmap(0, "Assets/Lightmap/Lightmap.txt");
	// 補助ライトの読み込み
	gsLoadAuxLight(0, "Assets/AuxLight/AuxLight_Torch_.txt");

	// シャドウマップの作成（２枚のカスケードシャドウマップ）
	static const GSuint shadow_map_size[] = { 2024, 1024 };
	gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
	// シャドウマップの減衰パラメータ
	gsSetShadowMapAttenuation(0.6f);
	// シャドウマップの距離を設定
	gsSetShadowMapDistance(60.0f);

	// プレーヤーの追加
	world_->add_actor(std::make_shared<Player>(world_, GSvector3{ -5.0f,1.967f,4.0f }));
	// フィールドクラスの追加
	world_->add_field(std::make_shared<Field>(Octree_Stage, Octree_Collider));
	// カメラクラスの追加
	world_->add_camera(std::make_shared<CameraTPS>(
		world_, GSvector3{ 0.0f, 3.2f, 9.0f }, GSvector3{ 0.0f, 1.0, 0.0f }));
	// ライトクラスの追加
	world_->add_light(std::make_shared<Light>(world_));
	//ステージデータ読み込み
	enemy_generator_ = std::make_shared<EnemyGenerator>(world_, "Assets/StageData/StatgeData.csv", "Assets/StageData/FasePos.csv");
	// 炎のエフェクトを再生する(0〜8が松明用ライト）
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
	enemy_generator_->update(delta_time);
}

// 描画
void GamePlayScene::draw() const {
	 //レンダーターゲットを有効にする
	gsBeginRenderTarget(0);
	 //レンダーターゲットのクリア
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	 //ワールドの描画
	world_->draw();
	//GUI描画
	world_->draw_gui();
	//fog描画
	fog_.draw();
	//レンダーターゲットを無効にする
	gsEndRenderTarget();

	// ポストエフェクトのパラメータ
	static GScolor color_{ 1.0f, 1.0f, 1.0f, 1.0f };
	static float   saturation_{ 1.0f };
	static float   luminance_{ 1.0f };
	static float   exposure_{ 1.0f };

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
	//BGM 止める
	gsStopBGM();
}
