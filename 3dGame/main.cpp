#include<GSgame.h>
#include "Scene/SceneManager.h"
#include"Scene//TitleScene.h"
#include "Scene/GamePlayScene.h"
#include"Scene/LoadingScene.h"
#include"Assets.h"
#include"Input.h"

#include<crtdbg.h>
#include<memory>
class MyGame :public gslib::Game {
public:
	//
	//コンストラクタ
	MyGame() : gslib::Game{ 1280, 720 }{

	}

	// 開始
	void start() override {
		scene_manager_.add("LoadingScene", new LoadingScene());
		scene_manager_.add("TitleScene", new TitleScene());
		scene_manager_.add("GamePlayScene", new GamePlayScene());
		scene_manager_.change("LoadingScene");
	}
	// 更新
	void update(float delta_time) {
		scene_manager_.update(delta_time);
		Input::update(delta_time);
	}
	// 描画
	void draw() override {
		scene_manager_.draw();
	}
	// 終了
	void end() {
		scene_manager_.end();
		
		// メッシュの削除
		gsDeleteMesh(Mesh_Player);
		gsDeleteMesh(Mesh_Dragon);
		gsDeleteMesh(Mesh_Skybox);
		// スケルトンの削除
		gsDeleteSkeleton(Mesh_Player);
		gsDeleteSkeleton(Mesh_Dragon);
		// アニメーションの削除
		gsDeleteAnimation(Mesh_Player);
		gsDeleteAnimation(Mesh_Dragon);
		// オクトリーの削除
		gsDeleteOctree(Octree_Stage);
		gsDeleteOctree(Octree_Collider);
	}

	bool is_running()override {
		//エスケープキーが押されていたら終了
		if (gsGetKeyState(GKEY_ESCAPE)) {
			return false;
		}
		return true;
	}
private:
	// シーンマネージャー
	SceneManager scene_manager_;
};

int main() {
	return MyGame().run();
}