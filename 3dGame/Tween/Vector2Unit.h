#ifndef VECTOR2_UNIT_H_
#define VECTOR2_UNIT_H_

#include "TweenUnit.h"

//Vector2�𓮂������߂�TweenUnit
class Vector2Unit : public TweenUnit {
public:
	//�R���X�g���N�^
	Vector2Unit(const GSvector2& from, const GSvector2& to, float duration,
		std::function<void(const GSvector2&)> update_func);
	//�X�V����
	virtual void on_update(float progress) override;

private:
	GSvector2 from_; //�J�n�l
	GSvector2 to_; //�I���l
	std::function<void(GSvector2)> update_func_; //�X�V���̏���
};

#endif