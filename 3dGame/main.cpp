#include<GSgame.h>
#include "Scene/SceneManager.h"
#include"Scene//TitleScene.h"
#include "Scene/GamePlayScene.h"
#include"Scene/LoadingScene.h"
#include"Tween/Tween.h"
#include"Assets.h"
#include"Input.h"
#include<crtdbg.h>
#include<memory>


class MyGame :public gslib::Game 
{
public:
	//
	//コンストラクタ
	MyGame() : gslib::Game{ 1280, 720}{
	}

	// 開始
	void start() override 
	{
		scene_manager_.add("LoadingScene", std::make_shared<LoadingScene>());
		scene_manager_.add("TitleScene", std::make_shared<TitleScene>());
		scene_manager_.add("GamePlayScene", std::make_shared<GamePlayScene>());
		scene_manager_.change("LoadingScene");
	}
	// 更新
	void update(float delta_time)
	{
		scene_manager_.update(delta_time);
		Input::update(delta_time);
		Tween::update(delta_time);
	}
	// 描画
	void draw() override
	{
		scene_manager_.draw();
	}
	// 終了
	void end() 
	{
		scene_manager_.end();
		
		//画像の削除
		gsDeleteTexture(Texture_Frame);
		gsDeleteTexture(Texture_HP);
		gsDeleteTexture(Texture_ST);
		gsDeleteTexture(Texture_Fade);
		gsDeleteTexture(Texture_Titel);
		gsDeleteTexture(Texture_GameClear);
		gsDeleteTexture(Texture_GameOver);

		// メッシュの削除
		gsDeleteMesh(Mesh_Player);
		gsDeleteMesh(Mesh_Dragon);
		gsDeleteMesh(Mesh_Skeleton);
		gsDeleteMesh(Mesh_Witch);
		gsDeleteMesh(Mesh_Skybox);
		// スケルトンの削除
		gsDeleteSkeleton(Mesh_Player);
		gsDeleteSkeleton(Mesh_Dragon);
		gsDeleteSkeleton(Mesh_Skeleton);
		gsDeleteSkeleton(Mesh_Witch);
		// アニメーションの削除
		gsDeleteAnimation(Mesh_Player);
		gsDeleteAnimation(Mesh_Dragon);
		gsDeleteAnimation(Mesh_Skeleton);
		gsDeleteAnimation(Mesh_Witch);
		// オクトリーの削除
		gsDeleteOctree(Octree_Stage);
		gsDeleteOctree(Octree_Collider);
		//SEの削除
		gsDeleteSE(Se_PlayerAttack);
		gsDeleteSE(Se_PlayerBlock);
		gsDeleteSE(Se_PlayerDamage);
		gsDeleteSE(Se_PlayerRun);
		gsDeleteSE(Se_EnemyDamage);
		gsDeleteSE(Se_DragonAttack1);
		gsDeleteSE(Se_DragonAttack2);
		gsDeleteSE(Se_DragonDeath);
		gsDeleteSE(Se_DragonFire);
		gsDeleteSE(Se_DragonFoot);
		gsDeleteSE(Se_DragonLanding);
		gsDeleteSE(Se_DragonSpitFire);
		// ライトマップの削除
		gsDeleteLightmap(0);
		// リフレクションプローブの削除
		gsDeleteReflectionProbe(0);
		// 補助ライトの削除
		gsDeleteAuxLight(0);
	}

	bool is_running()override 
	{
		//エスケープキーが押されていたら終了
		if (gsGetKeyState(GKEY_ESCAPE))
		{
			return false;
		}
		return true;
	}
private:
	// シーンマネージャー
	SceneManager scene_manager_;
};

int main()
{
	return MyGame().run();
}