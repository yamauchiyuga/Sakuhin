#include<GSgame.h>
#include "Scene/SceneManager.h"
#include "Scene/GamePlayScene.h"
#include<crtdbg.h>
#include<memory>
class MyGmae :public gslib::Game {
public:
    //
    MyGmae() :gslib::Game{ 1024,768 }{
    }

    // 開始
    void start() override {

        scene_manager_.add("GamePlayScene", new GamePlayScene());
        scene_manager_.change("GamePlayScene");
    }
    // 更新
    void update(float delta_time) {
        scene_manager_.update(delta_time);
    }
    // 描画
    void draw() override {
        scene_manager_.draw();
    }
    // 終了
    void end() {
        scene_manager_.end();
    }

private:
    // シーンマネージャー
    SceneManager scene_manager_;


};

int main() {
    return MyGmae().run();
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}