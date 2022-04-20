#include "GamePlayScene.h"
#include"../Collision/Field.h"
#include "../Collision/Field.h"
#include "../Camera/CameraTPS.h"
#include "../Actor/Light.h"
#include"../Actor/Player/Player.h"
#include"../Asset.h"


void GamePlayScene::start() {
    // 終了フラグを初期化
    is_end_ = false;

    //
    gsLoadMesh(Mesh_Player, "Assets/model/Kendo/kendo.msh");
    gsLoadSkeleton(Mesh_Player, "Assets/model/Kendo/kendo.skl");
    gsLoadAnimation(Mesh_Player, "Assets/model/Kendo/kendo.anm");

    // スカイボックスメッシュの読み込み
    gsLoadMesh(Mesh_Skybox, "Assets/Skybox/skydome.msh");
    // 描画用オクツリーの読み込み
    gsLoadOctree(Octree_Stage, "Assets/Octree/stage.oct");
    // 衝突判定用オクツリーの読み込み
    gsLoadOctree(Octree_Collider, "Assets/Octree/stage_collider.oct");

    // フィールドクラスの追加
    world_.add_field(new Field{ Octree_Stage, Octree_Collider, Mesh_Skybox });
    // カメラクラスの追加
    world_.add_camera(new CameraTPS{
              &world_, GSvector3{0.0f, 3.2f, -4.8f}, GSvector3{0.0f, 1.92, 0.0f} });
    // ライトクラスの追加
    world_.add_light(new Light{ &world_ });

    // プレーヤーの追加
    world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.125f, 0.0f } });

}

// 更新
void GamePlayScene::update(float delta_time) {
    // エンターキーでシーン終了としておく
    if (gsGetKeyTrigger(GKEY_RETURN)) {
        is_end_ = true;
    }
    // ワールドの更新
    world_.update(delta_time);
}

// 描画
void GamePlayScene::draw() const {
    // ワールドの描画
    world_.draw();
}

// 終了しているか？
bool GamePlayScene::is_end() const {
    return is_end_;         // 終了フラグを返す
}

// 次のシーンを返す
std::string GamePlayScene::next() const {
    return "TitleScene";    // タイトルシーンに戻る
}

// 終了
void GamePlayScene::end() {
    // ワールドを消去
    world_.clear();
    // メッシュの削除
    gsDeleteMesh(Mesh_Player);
    gsDeleteSkeleton(Mesh_Player);
    gsDeleteAnimation(Mesh_Player);
    gsDeleteMesh(Mesh_Skybox);
    // オクトリーの削除
    gsDeleteOctree(Octree_Stage);
    gsDeleteOctree(Octree_Collider);
}
