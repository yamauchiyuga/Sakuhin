#ifndef VALUE_UNIT_H_
#define VALUE_UNIT_H_

#include "TweenUnit.h"

// �l(float�^)�𓮂������߂�TweenUnit
class ValueUnit : public TweenUnit {
public:
    // �R���X�g���N�^
    ValueUnit(float from, float to, float duration, std::function<void(float)> update_func);
    // �X�V���̏���
    virtual void on_update(float progress) override;

private:
    float from_;                                // �J�n�l
    float to_;                                  // �I���l
    std::function<void(float)> update_func_;    // �X�V���̏���
};

#endif

