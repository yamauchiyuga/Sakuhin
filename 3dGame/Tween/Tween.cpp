#include "Tween.h"
#include "ValueUnit.h"
#include "Vector3Unit.h"
#include "Vector2Unit.h"

std::list<TweenUnit*> Tween::units_;

void Tween::add(TweenUnit* unit) {
	units_.push_back(unit);
}

void Tween::update(float delta_time) {
	//�S�Ă�TweenUnit�̍X�V�������Ăяo��
	for (TweenUnit* unit : units_) {
		unit->update(delta_time);
	}

	//�I������TweenUnit�͍폜����
	for (auto itr = units_.begin(); itr != units_.end();) {
		if ((*itr)->is_finished()) {
			//���g��TweenUnit��delete���Ă���
			delete* itr;
			//���X�g�̗v�f���폜����
			itr = units_.erase(itr);
		}
		else {
			++itr;
		}
	}
}

void Tween::clear() {
	for (TweenUnit* tween : units_) {
		delete tween;
	}
	units_.clear();
}

TweenUnit& Tween::value(float from, float to, float duration, std::function<void(float)> update_func) {
	TweenUnit* unit = new ValueUnit(from, to, duration, update_func);
	add(unit); //���X�g�ɉ����ĊǗ��ΏۂƂ���
	return *unit; //TweenUnit�̎Q�Ƃ�ԋp����
}

TweenUnit& Tween::vector3(const GSvector3& from, const GSvector3& to, float duration,
	std::function<void(const GSvector3&)> update_func) {
	TweenUnit* unit = new Vector3Unit(from, to, duration, update_func);
	add(unit);
	return *unit;
}

TweenUnit& Tween::vector2(const GSvector2& from, const GSvector2& to, float duration,
	std::function<void(const GSvector2&)> update_func) {
	TweenUnit* unit = new Vector2Unit(from, to, duration, update_func);
	add(unit);
	return *unit;
}

TweenUnit& Tween::delay_call(float delay_time, std::function<void()> callback) {
	TweenUnit* unit = new TweenUnit(delay_time);
	unit->on_complete(callback);
	add(unit);
	return *unit;
}

void Tween::cancel(const std::string& name) {
	// �S�Ă̍��ڂ𑖍����āA���O����v�������̂��폜����
	for (auto itr = units_.begin(); itr != units_.end(); ) {
		if ((*itr)->name() == name) {
			delete* itr;
			itr = units_.erase(itr);
		}
		else {
			++itr;
		}
	}
}