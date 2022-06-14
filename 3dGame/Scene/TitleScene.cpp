#include "TitleScene.h"
#include"../Input.h"
// 開始
void TitleScene::start() {
    // 終了フラグの初期化
    is_end_ = false;
}

// 更新
void TitleScene::update(float delta_time) {
    // エンターキーを押したらシーン終了
    if (Input::is_a_push()) {
        is_end_ = true;     // シーン終了
    }
}

// 描画
void TitleScene::draw() const {
    gsFontParameter(0, 50, "ＭＳ ゴシック");
    gsTextPos(80, 215);
    gsDrawText("タイトルシーン（仮）");
    gsFontParameter(0, 16, "ＭＳ ゴシック");
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
}

