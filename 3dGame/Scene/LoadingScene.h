#ifndef LOADING_SCENE_H_
#define LOADING_SCENE_H_

#include "IScene.h"
class LoadingScene : public IScene
{
public:
    // 開始
    virtual void start() override;
    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;
    // 終了しているか？
    virtual bool is_end() const override;
    // 次のシーンを返す
    virtual std::string next() const override;
    // 終了
    virtual void end() override;
private:
    //ロード用スレッド
    static void load_assets(void* self);
    //
    std::string now_loading_;
    //ドットを出現させるタイマー
    float timer_;
    //ドットの数
    int dot_count_;
    //終了しているか
    bool is_end_;
};



#endif // !LoadingScene


