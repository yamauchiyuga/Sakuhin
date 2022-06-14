#include "TitleScene.h"
#include"../Input.h"
// �J�n
void TitleScene::start() {
    // �I���t���O�̏�����
    is_end_ = false;
}

// �X�V
void TitleScene::update(float delta_time) {
    // �G���^�[�L�[����������V�[���I��
    if (Input::is_a_push()) {
        is_end_ = true;     // �V�[���I��
    }
}

// �`��
void TitleScene::draw() const {
    gsFontParameter(0, 50, "�l�r �S�V�b�N");
    gsTextPos(80, 215);
    gsDrawText("�^�C�g���V�[���i���j");
    gsFontParameter(0, 16, "�l�r �S�V�b�N");
}

// �I�����Ă��邩�H
bool TitleScene::is_end() const {
    return is_end_;         // �I���t���O��Ԃ�
}

// ���̃V�[������Ԃ�
std::string TitleScene::next() const {
    return "GamePlayScene"; // ���̃V�[������Ԃ�
}

// �I��
void TitleScene::end() {
}

