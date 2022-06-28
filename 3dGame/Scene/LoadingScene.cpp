#include "LoadingScene.h"
#include <GSstandard_shader.h>
#include "../Assets.h"
#include <GSmusic.h>
#include <gslib.h>
#include <GSgame.h>
#include <cmath>
#include<GSeffect.h>

void LoadingScene::start()
{
	now_loading_ = "Now Loading";
	timer_ = 0.0f;
	dot_count_ = 0;
	is_end_ = false;
	// ロード用スレッドの実行
	gslib::Game::run_thread(load_assets, this);
}

void LoadingScene::update(float delta_time)
{
	//この秒数間で生成
	static const float dot_generation_time = 10.0f;
	if (timer_ >= dot_generation_time) {
		//ドットが指定数より多くならないよう初期化
		now_loading_ = "Now Loading";
		dot_count_ = (dot_count_ + 1) % 4;
		for (int i = 0; i < dot_count_ ; ++i) {
			now_loading_ += ".";
		}
		timer_ = 0.0f;
	}
	timer_ += delta_time;
}

void LoadingScene::draw() const
{
	const static GSvector2 TextPos{ 900,650 };
	gsFontParameter(0, 50, "ＭＳ ゴシック");
	gsTextPos(TextPos.x, TextPos.y);
	gsDrawText(now_loading_.c_str());
}

bool LoadingScene::is_end() const
{
	return is_end_;
}

std::string LoadingScene::next() const
{
	return "TitleScene";
}

void LoadingScene::end() {
}

void LoadingScene::load_assets(void* self) {

	LoadingScene* load = (LoadingScene*)self;
	//シェーダーの初期化
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
	// ポストエフェクトシェーダーの読み込み
	gsLoadShader(0, "Assets/Shader/RenderTexture.vert", "Assets/Shader/PostEffect.frag");
	// リフレクションプローブの読み込み(0番に読み込めば自動的に適用される）
	gsLoadReflectionProbe(0, "Assets/RefProbe/ReflectionProbe.txt");
	// ライトマップの読み込み(0番に読み込めば自動的に適用される）
	gsLoadLightmap(0, "Assets/Lightmap01/Lightmap.txt");
	// 補助ライトの読み込み
	gsLoadAuxLight(0, "Assets/AuxLight/AuxLight_Torch_.txt");
	gsLoadAuxLight(1, "Assets/AuxLight/AuxLight.txt"); 

	//プレイヤーの読み込み
	gsLoadMesh(Mesh_Player, "Assets/model/Player/Player.mshb");
	gsLoadSkeleton(Mesh_Player, "Assets/model/Player/Player.sklb");
	gsLoadAnimation(Mesh_Player, "Assets/model/Player/Player.anmb");
	//ドラゴン読み込み
	gsLoadMesh(Mesh_Dragon, "Assets/model/Enemy/Dragon/Dragon.mshb");
	gsLoadSkeleton(Mesh_Dragon, "Assets/model/Enemy/Dragon/Dragon.sklb");
	gsLoadAnimation(Mesh_Dragon, "Assets/model/Enemy/Dragon/Dragon.anmb");
	//スケルトン読み込み
	gsLoadMesh(Mesh_Skeleton, "Assets/model/Enemy/Skeleton/Skeleton.mshb");
	gsLoadSkeleton(Mesh_Skeleton, "Assets/model/Enemy/Skeleton/Skeleton.sklb");
	gsLoadAnimation(Mesh_Skeleton, "Assets/model/Enemy/Skeleton/Skeleton.anmb");
	//ウィッチ読み込み
	gsLoadMesh(Mesh_Witch, "Assets/model/Enemy/Witch/Witch.mshb");
	gsLoadSkeleton(Mesh_Witch, "Assets/model/Enemy/Witch/Witch.sklb");
	gsLoadAnimation(Mesh_Witch, "Assets/model/Enemy/Witch/Witch.anmb");
	//テクスチャ読み込み
	gsLoadTexture(Texture_Titel, "Assets/Texture/Title.png");
	gsLoadTexture(Texture_Frame, "Assets/Texture/GaugeFrame.png");
	gsLoadTexture(Texture_Fade, "Assets/Texture/fade.png");
	gsLoadTexture(Texture_GameOver, "Assets/Texture/GameOver.png");
	gsLoadTexture(Texture_GameClear, "Assets/Texture/GameClear.png");
	gsLoadTexture(Texture_Lock, "Assets/Texture/LockOn.png");
	//SE読み込み
	gsLoadSE(Se_GameStart, "Assets/Sound/SE/Titel.wav",1, GWAVE_DEFAULT);
	gsLoadSE(Se_GameClear, "Assets/Sound/SE/clear.wav",1, GWAVE_DEFAULT);
	gsLoadSE(Se_PlayerAttack, "Assets/Sound/SE/Player/Player_Combo01_Swing.wav",1, GWAVE_DEFAULT);
	gsLoadSE(Se_PlayerBlock, "Assets/Sound/SE/Player/Player_Block.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_PlayerDamage, "Assets/Sound/SE/Player/Player_Damage.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_PlayerRun, "Assets/Sound/SE/Player/Player_W.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_EnemyDamage, "Assets/Sound/SE/Player/Damage.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_DragonAttack1, "Assets/Sound/SE/Dragon/Dragon_Attack1.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_DragonAttack2, "Assets/Sound/SE/Dragon/Dragon_Attack2.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_DragonDeath, "Assets/Sound/SE/Dragon/Dragon_Death.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_DragonFire, "Assets/Sound/SE/Dragon/Dragon_Fire.wav", 2, GWAVE_DEFAULT);
	gsLoadSE(Se_DragonFoot, "Assets/Sound/SE/Dragon/Dragon_Foot.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_DragonLanding, "Assets/Sound/SE/Dragon/Dragon_Foot.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_DragonSpitFire, "Assets/Sound/SE/Dragon/Dragon_SpitFire.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_DragonExplosion, "Assets/Sound/SE/Dragon/Dragon_Explosion.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_WitchDetate, "Assets/Sound/SE/Witch/Witch_Death.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_SkeletonDetate, "Assets/Sound/SE/Skeleton/Skeleton_Death.wav", 1, GWAVE_DEFAULT);
	//BG読み込み
	gsLoadBGM(Sound_Titel, "Assets/Sound/BGM/Result.ogg",GS_TRUE);
	gsLoadBGM(Sound_Wind, "Assets/Sound/BGM/Wind.ogg",GS_TRUE);

	// 描画用オクツリーの読み込み
	gsLoadOctree(Octree_Stage, "Assets/TtestFiled/Field01.oct");
	// 衝突判定用オクツリーの読み込み
	gsLoadOctree(Octree_Collider, "Assets/TtestFiled/Field01_collider.oct");
	//音量設定
	gsSetVolumeSE(Se_PlayerRun, 0.5f);
	gsSetVolumeSE(Se_PlayerAttack, 0.5f);
	gsSetVolumeSE(Se_PlayerBlock, 0.5f);
	gsSetVolumeSE(Se_PlayerDamage, 0.5f);
	gsSetVolumeSE(Se_EnemyDamage, 0.5f);
	gsSetVolumeSE(Se_DragonAttack1, 0.5f);
	gsSetVolumeSE(Se_DragonAttack2, 0.5f);
	gsSetVolumeSE(Se_DragonDeath, 0.5f);
	gsSetVolumeSE(Se_DragonFire, 0.5f);
	gsSetVolumeSE(Se_DragonFoot, 0.5f);
	gsSetVolumeSE(Se_DragonLanding, 0.5f);
	gsSetVolumeSE(Se_DragonSpitFire, 0.5f);
	gsSetVolumeSE(Se_DragonExplosion, 0.5f);
	gsSetVolumeSE(Se_WitchDetate, 0.5f);
	gsSetVolumeSE(Se_SkeletonDetate, 0.5f);

	load->is_end_ = true;

}