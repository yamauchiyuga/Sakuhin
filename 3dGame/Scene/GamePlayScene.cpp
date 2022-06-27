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

	// シャドウマップの作成（２枚のカスケードシャドウマップ）
	static const GSuint shadow_map_size[] = { 2024, 1024 };
	gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
	// シャドウマップの減衰パラメータ
	gsSetShadowMapAttenuation(0.6f);
	// シャドウマップの距離を設定
	gsSetShadowMapDistance(60.0f);

	// エフェクトの読み込み（松明の炎）
	gsLoadEffect(0, "Assets/Effect/Fire/Fire.efk");
	// プレーヤーの追加
	world_->add_actor( std::make_shared<Player>( world_, GSvector3{-5,4.0,10} ));
	// フィールドクラスの追加
	world_->add_field(std::make_shared<Field>( Octree_Stage, Octree_Collider, Mesh_Skybox ));
	// カメラクラスの追加
	world_->add_camera(std::make_shared<CameraTPS>(
			  world_, GSvector3{0.0f, 3.2f, -4.8f}, GSvector3{0.0f, 1.0, 0.0f} ));
	// ライトクラスの追加
	world_->add_light(std::make_shared<Light>( world_ ));
	//
	enemy_generator_ = std::make_shared<EnemyGenerator>(world_, "Assets/StageData/StatgeData.csv", "Assets/StageData/FasePos.csv");
	

	// 炎のエフェクトを再生する(0～8が松明用ライト）
	for (GSint i = 0; i < 17; ++i) {
		// 補助ライトの位置を取得
		GSvector3 position;
		gsGetAuxLightPosition(0, i, &position);
		// 補助ライトの位置に炎のエフェクトを出現させる
		gsPlayEffect(0, &position);
	}

}

// 更新
void GamePlayScene::update(float delta_time) {
	// ワールドの更新
	world_->update(delta_time);
	fog_.update(delta_time);
	enemy_generator_->update(delta_time);

}

// 描画
void GamePlayScene::draw() const {

	// ワールドの描画
	world_->draw();
	//
	world_->draw_gui();
	//
	fog_.draw();
	
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
}
