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

	// �J�n
	void start() override {
		scene_manager_.add("TitleScene", new TitleScene());
		scene_manager_.add("GamePlayScene", new GamePlayScene());
		scene_manager_.change("TitleScene");
	}
	// �X�V
	void update(float delta_time) {
		scene_manager_.update(delta_time);
		Input::update();
	}
	// �`��
	void draw() override {
		scene_manager_.draw();
	}
	// �I��
	void end() {
		scene_manager_.end();
	}

	bool is_running()override {
		//�G�X�P�[�v�L�[��������Ă�����I��
		if (gsGetKeyState(GKEY_ESCAPE)) {
			return false;
		}
		return true;
	}
private:
	// �V�[���}�l�[�W���[
	SceneManager scene_manager_;
};

int main() {
	return MyGmae().run();
}