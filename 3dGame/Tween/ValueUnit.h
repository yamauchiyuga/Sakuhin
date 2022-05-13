#ifndef VALUE_UNIT_H_
#define VALUE_UNIT_H_

#include "TweenUnit.h"

class ValueUnit:public TweenUnit{
public:
	//
	ValueUnit(float from, float to, float duration, std::function<void(float)>update_func);
	//
	virtual void on_update(float progress)override;

private:
	float from_;                                // �J�n�l
	float to_;                                  // �I���l
	std::function<void(float)> update_func_;    // �X�V���̏���
};
#endif // !1Value_Unit_


