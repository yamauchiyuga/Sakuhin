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
	//�R���X�g���N�^
	MyGame() : gslib::Game{ 1280, 720 }{

	}

	// �J�n
	void start() override {
		scene_manager_.add("LoadingScene", new LoadingScene());
		scene_manager_.add("TitleScene", new TitleScene());
		scene_manager_.add("GamePlayScene", new GamePlayScene());
		scene_manager_.change("LoadingScene");
	}
	// �X�V
	void update(float delta_time) {
		scene_manager_.update(delta_time);
		Input::update(delta_time);
	}
	// �`��
	void draw() override {
		scene_manager_.draw();
	}
	// �I��
	void end() {
		scene_manager_.end();
		
		// ���b�V���̍폜
		gsDeleteMesh(Mesh_Player);
		gsDeleteMesh(Mesh_Dragon);
		gsDeleteMesh(Mesh_Skybox);
		// �X�P���g���̍폜
		gsDeleteSkeleton(Mesh_Player);
		gsDeleteSkeleton(Mesh_Dragon);
		// �A�j���[�V�����̍폜
		gsDeleteAnimation(Mesh_Player);
		gsDeleteAnimation(Mesh_Dragon);
		// �I�N�g���[�̍폜
		gsDeleteOctree(Octree_Stage);
		gsDeleteOctree(Octree_Collider);
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
	return MyGame().run();
}