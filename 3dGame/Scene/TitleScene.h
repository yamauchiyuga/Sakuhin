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
    // �J�n
    virtual void start() override;
    // �X�V
    virtual void update(float delta_time) override;
    // �`��
    virtual void draw() const override;
    // �I�����Ă��邩�H
    virtual bool is_end() const override;
    // ���̃V�[����Ԃ�
    virtual std::string next() const override;
    // �I��
    virtual void end() override;
private:
   std::shared_ptr<World> world_;
   Fog fog;
    //�t�F�[�h�摜�̕`��
    void draw_fade() const;
    // �I���t���O
    bool is_end_{false};
    //�t�F�[�h�摜�̃A���t�@�l
    float alpha_{0.0f};
    //�t�F�[�h����
    bool is_fade_{false};
};

#endif

