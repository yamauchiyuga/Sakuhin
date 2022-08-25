#include "Tween.h"
#include "ValueUnit.h"
#include "Vector3Unit.h"
#include "Vector2Unit.h"

std::list<std::shared_ptr<TweenUnit>> Tween::units_;

void Tween::add(std::shared_ptr<TweenUnit> unit) {
	units_.push_back(unit);
}

void Tween::update(float delta_time) {
	//全てのTweenUnitの更新処理を呼び出す
	for (auto& unit : units_) {
		unit->update(delta_time);
	}

	//終了したTweenUnitは削除する
	for (auto itr = units_.begin(); itr != units_.end();) {
		if ((*itr)->is_finished()) {
			//リストの要素を削除する
			itr = units_.erase(itr);
		}
		else {
			++itr;
		}
	}
}

void Tween::clear() {
	units_.clear();
}

TweenUnit& Tween::value(float from, float to, float duration, std::function<void(float)> update_func) {
	std::shared_ptr<TweenUnit> unit = std::make_shared<ValueUnit>(from, to, duration, update_func);
	add(unit); //リストに加えて管理対象とする
	return *units_.back(); //TweenUnitの参照を返却する
}

TweenUnit& Tween::vector3(const GSvector3& from, const GSvector3& to, float duration,
	std::function<void(const GSvector3&)> update_func) {
	std::shared_ptr<TweenUnit> unit = std::make_shared< Vector3Unit>(from, to, duration, update_func);
	add(unit);
	return *units_.back();
}

TweenUnit& Tween::vector2(const GSvector2& from, const GSvector2& to, float duration,
	std::function<void(const GSvector2&)> update_func) {
	std::shared_ptr<TweenUnit> unit = std::make_shared<Vector2Unit>(from, to, duration, update_func);
	add(unit);
	return  *units_.back();
}

TweenUnit& Tween::delay_call(float delay_time, std::function<void()> callback) {
	std::shared_ptr<TweenUnit> unit = std::make_shared<TweenUnit>(delay_time);
	unit->on_complete(callback);
	add(unit);
	return *units_.back();
}

void Tween::cancel(const std::string& name) {
	// 全ての項目を走査して、名前が一致したものを削除する
	units_.remove_if([&](const auto& unit) { return unit->name() == name; });
}