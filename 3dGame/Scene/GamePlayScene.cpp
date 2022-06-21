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

void GamePlayScene::start() {
	// 終了フラグを初期化
	is_end_ = false;
	// 視錐台カリングを有効にする
	gsEnable(GS_FRUSTUM_CULLING);

	// リフレクションプローブの読み込み(0番に読み込めば自動的に適用される）
	gsLoadReflectionProbe(0, "Assets/RefProbe/ReflectionProbe.txt");
	// ライトマップの読み込み(0番に読み込めば自動的に適用される）
	gsLoadLightmap(0, "Assets/Lightmap/Lightmap.txt");
	// 補助ライトの読み込み
	gsLoadAuxLight(0, "Assets/AuxLight/AuxLight.txt");

	// シャドウマップの作成（２枚のカスケードシャドウマップ）
	static const GSuint shadow_map_size[] = { 2024, 1024 };
	gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
	// シャドウマップを適用する距離
	gsSetShadowMapDistance(80.0f);
	// シャドウマップバイアス
	gsSetShadowMapBias(0.0f);

	// プレーヤーの追加
	world_.add_actor(new Player{ &world_, GSvector3{ -7.0f, 4.0f, -100.0f } });
	// フィールドクラスの追加
	world_.add_field(new Field{ Octree_Stage, Octree_Collider, Mesh_Skybox });
	// カメラクラスの追加
	world_.add_camera(new CameraTPS{
			  &world_, GSvector3{0.0f, 3.2f, -4.8f}, GSvector3{0.0f, 1.0, 0.0f} });
	// ライトクラスの追加
	world_.add_light(new Light{ &world_ });
	//
	//enemy_generator_= std::make_shared<EnemyGenerator>(&world_,"Assets/StageData/StatgeData.csv","Assets/StageData/FasePos.csv" );

	//world_.add_actor(new Dragon{ &world_,GSvector3{-7.0f,4.0f,-116.0f} });
	world_.add_actor(new Skeketon{ &world_,GSvector3{-7.0f,4.0f,-120.0f} });
	//world_.add_actor(new Skeketon{ &world_,GSvector3{-7.0f,4.0f,-110.0f} });


}

// 更新
void GamePlayScene::update(float delta_time) {
	// ワールドの更新
	world_.update(delta_time);
	//enemy_generator_->update(delta_time);
}

// 描画
void GamePlayScene::draw() const {
	// ワールドの描画
	world_.draw();
	gsDrawText(std::to_string(world_.count_actor()).c_str());
}

// 終了しているか？
bool GamePlayScene::is_end() const {
	return world_.is_game_over();         // 終了フラグを返す
}

// 次のシーンを返す
std::string GamePlayScene::next() const {
	return "TitleScene";    // タイトルシーンに戻る
}

// 終了
void GamePlayScene::end() {
	// ワールドを消去
	world_.clear();
}
