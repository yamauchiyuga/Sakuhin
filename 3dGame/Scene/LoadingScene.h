#ifndef LOADING_SCENE_H_
#define LOADING_SCENE_H_

#include "IScene.h"
class LoadingScene : public IScene
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
    //���[�h�p�X���b�h
    static void load_assets(void* self);
    //
    std::string now_loading_;
    //
    float timer_;
    //
    int dot_count_;
    //
    bool is_end_;
};



#endif // !LoadingScene


