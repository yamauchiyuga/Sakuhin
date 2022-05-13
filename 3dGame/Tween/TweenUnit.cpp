#include "TweenUnit.h"
#include <algorithm>

TweenUnit::TweenUnit(float duration) : duration_{ duration } { }

void TweenUnit::update(float delta_time) {
	//
	elapsed_ += delta_time;
	//
	float progress = elapsed_ / duration_;
	//
	progress = std::min(progress, 1.0f);
	//
	on_update(progress);
	//
	is_finished_ = progress >= 1.0f;
}

bool TweenUnit::is_finished()const {
	return is_finished_;
}
