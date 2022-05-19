#include<GSgame.h>
#include "Scene/SceneManager.h"
#include"Scene//TitleScene.h"
#include "Scene/GamePlayScene.h"
#include"Input.h"
#include<crtdbg.h>
#include<memory>
class MyGmae :public gslib::Game {
public:
	//
	MyGmae() :gslib::Game{ 1280,720 }{
	}

	// 開始
	void start() override {
		scene_manager_.add("TitleScene", new TitleScene());
		scene_manager_.add("GamePlayScene", new GamePlayScene());
		scene_manager_.change("TitleScene");
	}
	// 更新
	void update(float delta_time) {
		scene_manager_.update(delta_time);
		Input::update();
	}
	// 描画
	void draw() override {
		scene_manager_.draw();
	}
	// 終了
	void end() {
		scene_manager_.end();
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
	return MyGmae().run();
}