#include<GSgame.h>

class MyGame :public gslib::Game {
    // 開始
    void start() override {
        // スカイドーム用のメッシュを読み込む
        gsLoadMesh(0, "Assets/Skybox/skydome.msh");
        // オクトリーを読み込む
        gsLoadOctree(0, "Assets/Octree/stage.oct");
    }
    // 更新
    void update(float delta_time) override {
    }
    // 描画
    void draw() override {
        // スカイドームの描画
        gsDrawSkybox(0);
        // オクトリーの描画
        gsDrawOctree(0);
    }
    // 終了
    void end() override {
        // スカイドームの削除
        gsDeleteMesh(0);
        // オクトリーの削除
        gsDeleteOctree(0);
    }

};

int main() {
	return MyGame().run();
}