#ifndef VECTOR3_UNIT_H_
#define VECTOR3_UNIT_H_

#include "TweenUnit.h"

// Vector3�^�𓮂������߂�TweenUnit
class Vector3Unit : public TweenUnit {
public:
    // �R���X�g���N�^
    Vector3Unit(const GSvector3& from, const GSvector3& to, float duration, std::function<void(const GSvector3&)> update_func);
    // �X�V���̏���
    virtual void on_update(float progress) override;

private:
    GSvector3 from_;                                // �J�n�l
    GSvector3 to_;                                  // �I���l
    std::function<void(GSvector3)> update_func_;    // �X�V���̏���
};

#endif
