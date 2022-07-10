#ifndef TITLE_SCENE_H_
#define TITLE_SCENE_H_

#include "IScene.h"
#include"../World/World.h"
#include"../Fog/Fog.h"
#include <gslib.h>
#include<memory>

class TitleScene : public IScene 
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
   std::shared_ptr<World> world_;
   Fog fog;
    //フェード画像の描画
    void draw_fade() const;
    // 終了フラグ
    bool is_end_{false};
    //フェード画像のアルファ値
    float alpha_{0.0f};
    //フェード中か
    bool is_fade_{false};
};

#endif

