#include "TitleScene.h"
#include"../Input.h"
#include"../Assets.h"
#include"../Tween/Tween.h"

// �J�n
void TitleScene::start() 
{
    // �I���t���O�̏�����
    is_end_ = false;
    //�t�F�[�h�t���O��������
    is_fade_ = true;
    //�t�F�[�h�̃A���t�@�l��������
    alpha_ = 1.0f;
    gsPlayBGM(Sound_Titel);
    gsSetVolumeBGM(0.2f);
    //40�t���[�������ăt�F�[�h�C��
    Tween::value(1.0f, 0.0f, 60.0f, [=](GSfloat val) {alpha_ = val; })
        .on_complete([=] {is_fade_ = false; });

}

// �X�V
void TitleScene::update(float delta_time) 
{
    if (is_fade_)return;
    // �G���^�[�L�[����������V�[���I��
    if (Input::is_a_push()) {
        gsPlaySE(Se_GameStart);
        is_end_ = true;     // �V�[���I��
    }
}

// �`��
void TitleScene::draw() const 
{
    GSvector2 TitlePos{ 0.0f,0.0f };
    gsDrawSprite2D(Texture_Titel, &TitlePos, NULL, NULL, NULL, NULL,NULL);
   
    static const GSvector2 Pos{ 100.0f,-50.0f };
    gsDrawSprite2D(Texture_TitelName, &Pos, NULL, NULL, NULL, NULL, 0.0f);
    draw_fade();
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
    //Tween�̍폜
    Tween::clear();
    //BGM�̍폜
    gsDeleteBGM(Sound_Titel);
}

//�t�F�[�h����
void TitleScene::draw_fade()const {
    GScolor Color{ 1.0f, 1.0f, 1.0f, alpha_ };
    gsDrawSprite2D(Texture_Fade, NULL, NULL, NULL, &Color, NULL, 0.0f);
}

