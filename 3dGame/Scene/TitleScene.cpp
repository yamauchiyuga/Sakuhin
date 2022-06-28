#include "TitleScene.h"
#include"../Input.h"
#include"../Assets.h"
#include"../Tween/Tween.h"

// 開始
void TitleScene::start() 
{
    // 終了フラグの初期化
    is_end_ = false;
    //フェードフラグを初期化
    is_fade_ = true;
    //フェードのアルファ値を初期化
    alpha_ = 1.0f;
    gsPlayBGM(Sound_Titel);
    gsSetVolumeBGM(0.2f);
    //40フレームかけてフェードイン
    Tween::value(1.0f, 0.0f, 60.0f, [=](GSfloat val) {alpha_ = val; })
        .on_complete([=] {is_fade_ = false; });

}

// 更新
void TitleScene::update(float delta_time) 
{
    if (is_fade_)return;
    // エンターキーを押したらシーン終了
    if (Input::is_a_push()) {
        gsPlaySE(Se_GameStart);
        is_end_ = true;     // シーン終了
    }
}

// 描画
void TitleScene::draw() const 
{
    GSvector2 TitlePos{ 0.0f,0.0f };
    gsDrawSprite2D(Texture_Titel, &TitlePos, NULL, NULL, NULL, NULL,NULL);
   
    static const GSvector2 Pos{ 100.0f,-50.0f };
    gsDrawSprite2D(Texture_TitelName, &Pos, NULL, NULL, NULL, NULL, 0.0f);
    draw_fade();
}

// 終了しているか？
bool TitleScene::is_end() const {
    return is_end_;         // 終了フラグを返す
}

// 次のシーン名を返す
std::string TitleScene::next() const {
    return "GamePlayScene"; // 次のシーン名を返す
}

// 終了
void TitleScene::end() {
    //Tweenの削除
    Tween::clear();
    //BGMの削除
    gsDeleteBGM(Sound_Titel);
}

//フェード処理
void TitleScene::draw_fade()const {
    GScolor Color{ 1.0f, 1.0f, 1.0f, alpha_ };
    gsDrawSprite2D(Texture_Fade, NULL, NULL, NULL, &Color, NULL, 0.0f);
}

