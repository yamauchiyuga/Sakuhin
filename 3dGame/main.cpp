#include<GSgame.h>

class MyGame :public gslib::Game {
    // �J�n
    void start() override {
        // �X�J�C�h�[���p�̃��b�V����ǂݍ���
        gsLoadMesh(0, "Assets/Skybox/skydome.msh");
        // �I�N�g���[��ǂݍ���
        gsLoadOctree(0, "Assets/Octree/stage.oct");
    }
    // �X�V
    void update(float delta_time) override {
    }
    // �`��
    void draw() override {
        // �X�J�C�h�[���̕`��
        gsDrawSkybox(0);
        // �I�N�g���[�̕`��
        gsDrawOctree(0);
    }
    // �I��
    void end() override {
        // �X�J�C�h�[���̍폜
        gsDeleteMesh(0);
        // �I�N�g���[�̍폜
        gsDeleteOctree(0);
    }

};

int main() {
	return MyGame().run();
}