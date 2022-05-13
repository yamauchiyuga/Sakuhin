#ifndef TWEEN_UNIT_H_
#define TWEEN_UNIT_H_

#include <functional>
#include <gslib.h>

// Tween�̓���1���Ǘ�����N���X�̊��N���X
class TweenUnit {
public:
    // �R���X�g���N�^
    TweenUnit(float duration);
    // ���z�f�X�g���N�^
    virtual ~TweenUnit() = default;
    // �X�V����
    void update(float delta_time);
    // �I���������ǂ���
    bool is_finished() const;
    // �R�s�[�֎~
    TweenUnit(const TweenUnit& other) = delete;
    TweenUnit& operator = (const TweenUnit& other) = delete;

protected:
    // �h���N���X���Ƃ̍X�V����
    virtual void on_update(float progress) {}

private:
    float duration_{ 0.0f };        // �A�j���[�V�������ԁi�t���[���j
    float elapsed_{ 0.0f };         // �o�ߎ��ԁi�t���[���j
    bool is_finished_{ false };     // �I��������
};

#endif