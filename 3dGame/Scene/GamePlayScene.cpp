#include "GamePlayScene.h"
#include"../Collision/Field.h"
#include "../Collision/Field.h"
#include "../Camera/CameraTPS.h"
#include "../Actor/Light.h"
#include"../Actor/Player/Player.h"
#include"../Asset.h"
#include"../Actor/Enemy/Dragon.h"

void GamePlayScene::start() {

	

	// 終了フラグを初期化
	is_end_ = false;
	// 視錐台カリングを有効にする
	gsEnable(GS_FRUSTUM_CULLING);
	//
	gsEnable(GS_DEFAULT_SHADER_MODE);
	// メッシュ用のシェーダー
	gsLoadShader(Shader_StandardMesh,
		"Assets/shader/StandardMeshBump.vert",
		"Assets/shader/StandardMeshBump.frag");
	// スキニングメッシュ用のシェーダー
	gsLoadShader(Shader_StandardSkinnedMesh,
		"Assets/shader/StandardSkinnedMeshBump.vert",
		"Assets/shader/StandardMeshBump.frag");
	// オクツリー用のシェーダー
	gsLoadShader(Shader_StandardOctree,
		"Assets/shader/StandardOctreeLightmap.vert",
		"Assets/shader/StandardOctreeLightmap.frag");
	// 通常描画用のシェーダーをバインドする
	gsBindDefaultMeshShader(Shader_StandardMesh);
	gsBindDefaultSkinMeshShader(Shader_StandardSkinnedMesh);
	gsBindDefaultOctreeShader(Shader_StandardOctree);

	// シャドウマップの作成
	static const GSuint shadow_map_size[] = { 2048, 1024 };
	gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
	// シャドウマップを適用する距離(視点からの距離）
	gsSetShadowMapDistance(80.0f);
	// シャドウマップバイアス(シャドウアクネが発生した場合は調整が必要）
	gsSetShadowMapBias(0.0f);
	// リフレクションプローブの読み込み
	gsLoadReflectionProbe(0, "Assets/RefProbe/ReflectionProbe.txt");




	//プレイヤーの読み込み
	gsLoadMesh(Mesh_Player, "Assets/model/Player/Player.mshb");
	gsLoadSkeleton(Mesh_Player, "Assets/model/Player/Player.sklb");
	gsLoadAnimation(Mesh_Player, "Assets/model/Player/Player.anmb");

	gsLoadMesh(Mesh_Dragon, "Assets/model/Enemy/Dragon.mshb");
	gsLoadSkeleton(Mesh_Dragon, "Assets/model/Enemy/Dragon.sklb");
	gsLoadAnimation(Mesh_Dragon, "Assets/model/Enemy/Dragon.anmb");

	gsLoadTexture(Texture_HP, "Assets/Texture/HPGauge.png");
	gsLoadTexture(Texture_Frame, "Assets/Texture/GaugeFrame.png");

	// スカイボックスメッシュの読み込み
	gsLoadMesh(Mesh_Skybox, "Assets/Skybox/skydome.msh");
	// 描画用オクツリーの読み込み
	gsLoadOctree(Octree_Stage, "Assets/TtestFiled/Field.oct");
	// 衝突判定用オクツリーの読み込み
	gsLoadOctree(Octree_Collider, "Assets/TtestFiled/Field_collider.oct");

	// フィールドクラスの追加
	world_.add_field(new Field{ Octree_Stage, Octree_Collider, Mesh_Skybox });
	// カメラクラスの追加
	world_.add_camera(new CameraTPS{
			  &world_, GSvector3{0.0f, 3.2f, -4.8f}, GSvector3{0.0f, 1.0, 0.0f} });
	// ライトクラスの追加
	world_.add_light(new Light{ &world_ });

	// プレーヤーの追加
	world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.125f, 0.0f } });
	//
	world_.add_actor(new Dragon{ &world_,GSvector3{50.0f,0.125,0.0f} });
}

// 更新
void GamePlayScene::update(float delta_time) {
	// ワールドの更新
	world_.update(delta_time);
}

// 描画
void GamePlayScene::draw() const {
	// ワールドの描画
	world_.draw();
}

// 終了しているか？
bool GamePlayScene::is_end() const {
	return is_end_;         // 終了フラグを返す
}

// 次のシーンを返す
std::string GamePlayScene::next() const {
	return "TitleScene";    // タイトルシーンに戻る
}

// 終了
void GamePlayScene::end() {
	// ワールドを消去
	world_.clear();
	// メッシュの削除
	gsDeleteMesh(Mesh_Player);
	gsDeleteSkeleton(Mesh_Player);
	gsDeleteAnimation(Mesh_Player);
	gsDeleteMesh(Mesh_Skybox);
	// オクトリーの削除
	gsDeleteOctree(Octree_Stage);
	gsDeleteOctree(Octree_Collider);
}
