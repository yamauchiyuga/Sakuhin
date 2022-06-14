#include "LoadingScene.h"
#include <GSstandard_shader.h>
#include "../Assets.h"
#include <GSmusic.h>
#include <gslib.h>
#include <GSgame.h>
#include <cmath>

void LoadingScene::start()
{

    is_end_ = false;
	// ロード用スレッドの実行
	gslib::Game::run_thread(load_assets, this);
}

void LoadingScene::update(float delta_time)
{

}

void LoadingScene::draw() const
{
	gsFontParameter(0, 50, "ＭＳ ゴシック");
	gsTextPos(80, 215);
	gsDrawText("Loadingシーン（仮）");
	gsFontParameter(0, 16, "ＭＳ ゴシック");
}

bool LoadingScene::is_end() const
{
    return is_end_;
}

std::string LoadingScene::next() const
{
    return "TitleScene";
}

void LoadingScene::end(){
}

void LoadingScene::load_assets(void* self) {

	LoadingScene* load = (LoadingScene*)self;
	gsInitDefaultShader();
	// メッシュ用のシェーダー
	gsLoadShader(Shader_StandardMesh, "Assets/shader/StandardMeshBump.vert", "Assets/shader/StandardMeshBump.frag");
	// スキニングメッシュ用のシェーダー
	gsLoadShader(Shader_StandardSkinnedMesh, "Assets/shader/StandardSkinnedMeshBump.vert", "Assets/shader/StandardMeshBump.frag");
	// オクツリー用のシェーダー
	gsLoadShader(Shader_StandardOctree, "Assets/shader/StandardOctreeLightmap.vert", "Assets/shader/StandardOctreeLightmap.frag");
	// シャドウマップ用のメッシュシェーダー
	gsLoadShader(Shader_ShadowMapMesh, "Assets/shader/ShadowMap.vert", "Assets/shader/ShadowMap.frag");
	// シャドウマップ用のスキニングメッシュシェーダー
	gsLoadShader(Shader_ShadowMapSkinnedMesh, "Assets/shader/ShadowMapSkinned.vert", "Assets/shader/ShadowMap.frag");


	//プレイヤーの読み込み
	gsLoadMesh(Mesh_Player, "Assets/model/Player/Player.mshb");
	gsLoadSkeleton(Mesh_Player, "Assets/model/Player/Player.sklb");
	gsLoadAnimation(Mesh_Player, "Assets/model/Player/Player.anmb");

	//ドラゴン読み込み
	gsLoadMesh(Mesh_Dragon, "Assets/model/Enemy/Dragon/Dragon.mshb");
	gsLoadSkeleton(Mesh_Dragon, "Assets/model/Enemy/Dragon/Dragon.sklb");
	gsLoadAnimation(Mesh_Dragon, "Assets/model/Enemy/Dragon/Dragon.anmb");
	//
	gsLoadMesh(Mesh_Skeleton, "Assets/model/Enemy/Skeleton.mshb");
	gsLoadSkeleton(Mesh_Skeleton, "Assets/model/Enemy/Skeleton/Skeleton.sklb");
	gsLoadAnimation(Mesh_Skeleton, "Assets/model/Enemy/Skeleton/Skeleton.anmb");
	//
	gsLoadMesh(Mesh_Witch, "Assets/model/Enemy/Witch/Witch.mshb");
	gsLoadSkeleton(Mesh_Witch, "Assets/model/Enemy/Witch/Witch.sklb");
	gsLoadAnimation(Mesh_Witch, "Assets/model/Enemy/Witch/Witch.anmb");

	gsLoadTexture(Texture_Frame, "Assets/Texture/GaugeFrame.png");

	// スカイボックスメッシュの読み込み
	gsLoadMesh(Mesh_Skybox, "Assets/Skybox/skydome.msh");
	// 描画用オクツリーの読み込み
	gsLoadOctree(Octree_Stage, "Assets/TtestFiled/Field.oct");
	// 衝突判定用オクツリーの読み込み
	gsLoadOctree(Octree_Collider, "Assets/TtestFiled/Field_collider.oct");

	load->is_end_ = true;

}