#include "TweenUnit.h"
#include <algorithm>

TweenUnit::TweenUnit(float duration) : duration_{ duration } {}

void TweenUnit::update(float delta_time) {
    //delay���ݒ肳��Ă�����A���ꂪ�Ȃ��Ȃ�܂ŉ��������ҋ@
    if (delay_ > 0) {
        delay_ -= delta_time;
        //�ҋ@�����������͌o�ߎ��ԂƂ��Čv��
        if (delay_ < 0) {
            elapsed_ = -delay_;
        }
        return;
    }

	//�o�ߎ��Ԃ�i�߂�
	elapsed_ += delta_time;
	//�i�����i0�`1�j���v�Z
	float progress = elapsed_ / duration_;
	//�i������1�𒴂��Ȃ��悤�ɂ���
	progress = std::min(progress, 1.0f);
	//�i�����ɃC�[�W���O��K�p����
	float eased = apply_ease(progress, ease_type_, overshoot_);
	//�C�[�W���O��K�p�����i�������g���A�h���N���X���Ƃ̍X�V�������s��
	on_update(eased);
	//�I���������𔻒肷��
	is_finished_ = progress >= 1.0f;

    //�I�����Ă��āA���A�������R�[���o�b�N���ݒ肳��Ă���ꍇ�́A��������s����
    if (is_finished_ && complete_func_) {
        complete_func_();
    }
}

bool TweenUnit::is_finished() const {
	return is_finished_;
}

TweenUnit& TweenUnit::ease(EaseType ease_type) {
	ease_type_ = ease_type;
	return *this;
}

TweenUnit& TweenUnit::overshoot(float overshoot) {
    overshoot_ = overshoot;
    return *this;
}

TweenUnit& TweenUnit::delay(float delay) {
    delay_ = delay;
    return *this;
}

TweenUnit& TweenUnit::on_complete(std::function<void()> complete_func) {
    complete_func_ = complete_func;
    return *this;
}

const std::string& TweenUnit::name() const {
    return name_;
}

TweenUnit& TweenUnit::name(const std::string& name) {
    name_ = name;
    return *this;
}

float TweenUnit::apply_ease(float value, EaseType ease_type, float overshoot) {
    switch (ease_type) {
    case EaseType::Linear:         return value;
    case EaseType::EaseInBack:     return gsEasingInBack(value, overshoot);
    case EaseType::EaseInCirc:     return gsEasingInCirc(value);
    case EaseType::EaseInCubic:    return gsEasingInCubic(value);
    case EaseType::EaseInExpo:     return gsEasingInExpo(value);
    case EaseType::EaseInQuad:     return gsEasingInQuad(value);
    case EaseType::EaseInQuart:    return gsEasingInQuart(value);
    case EaseType::EaseInQuint:    return gsEasingInQuint(value);
    case EaseType::EaseInSine:     return gsEasingInSine(value);
    case EaseType::EaseOutBack:    return gsEasingOutBack(value, overshoot);
    case EaseType::EaseOutBounce:  return gsEasingOutBounce(value);
    case EaseType::EaseOutCirc:    return gsEasingOutCirc(value);
    case EaseType::EaseOutCubic:   return gsEasingOutCubic(value);
    case EaseType::EaseOutElastic: {
        if (value == 1.0f) return 1.0f;
        return 1.0f - cos(value * 10 * overshoot) * pow(1 - value, 3);
    }
    case EaseType::EaseOutExpo:    return gsEasingOutExpo(value);
    case EaseType::EaseOutQuad:    return gsEasingOutQuad(value);
    case EaseType::EaseOutQuart:   return gsEasingOutQuart(value);
    case EaseType::EaseOutQuint:   return gsEasingOutQuint(value);
    case EaseType::EaseOutSine:    return gsEasingOutSine(value);
    case EaseType::EaseInOutBack:  return gsEasingInOutBack(value, overshoot);
    case EaseType::EaseInOutCirc:  return gsEasingInOutCirc(value);
    case EaseType::EaseInOutCubic: return gsEasingInOutCubic(value);
    case EaseType::EaseInOutExpo:  return gsEasingInOutExpo(value);;
    case EaseType::EaseInOutQuad:  return gsEasingInOutQuad(value);
    case EaseType::EaseInOutQuart: return gsEasingInOutQuart(value);
    case EaseType::EaseInOutQuint: return gsEasingInOutQuint(value);
    case EaseType::EaseInOutSine:  return gsEasingInOutSine(value);
    case EaseType::Punch: {
        if (value == 0 || value == 1) return 0;
        float peek = 0.25f / overshoot;
        float a = (value - 1.0f) / (peek - 1.0f);
        a = CLAMP(a, 0.0f, 1.0f);
        float delta_time_delay = a * a * (3 - 2 * a);
        return sin(value * 6.28318530f * overshoot) * delta_time_delay;
    }
    default: return value;
    }
}