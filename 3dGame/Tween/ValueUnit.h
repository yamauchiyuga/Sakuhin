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
	float from_;                                // 開始値
	float to_;                                  // 終了値
	std::function<void(float)> update_func_;    // 更新時の処理
};
#endif // !1Value_Unit_


