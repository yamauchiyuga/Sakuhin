#ifndef TWEEN_UNIT_H_
#define TWEEN_UNIT_H_

#include <string>
#include <functional>
#include <gslib.h>
#include <GSeasing.h>

// �C�[�W���O���
enum class EaseType {
    Linear,
    EaseInBack,
    EaseInCirc,
    EaseInCubic,
    EaseInExpo,
    EaseInQuad,
    EaseInQuart,
    EaseInQuint,
    EaseInSine,
    EaseOutBack,
    EaseOutBounce,
    EaseOutCirc,
    EaseOutCubic,
    EaseOutElastic,
    EaseOutExpo,
    EaseOutQuad,
    EaseOutQuart,
    EaseOutQuint,
    EaseOutSine,
    EaseInOutBack,
    EaseInOutCirc,
    EaseInOutCubic,
    EaseInOutExpo,
    EaseInOutQuad,
    EaseInOutQuart,
    EaseInOutQuint,
    EaseInOutSine,
    Punch,
};


//Tween�̓���1���Ǘ�����N���X�̊��N���X
class TweenUnit {
public:
	//�R���X�g���N�^
	TweenUnit(float duration);
	//���z�f�X�g���N�^
	virtual ~TweenUnit() = default;
	//�X�V
	void update(float delta_time);
	//�I���������ǂ���
	bool is_finished() const;
    //�C�[�W���O��ʂ�ݒ肷��
    TweenUnit& ease(EaseType ease_type);
    //Back, Elastic, Punch�Ńr�����r�����ƂȂ鐨����ݒ肷��B�l���傫���قǁA�r�����r��������
    TweenUnit& overshoot(float overshoot);
    //�J�n�x�����Ԃ�ݒ肷��
    TweenUnit& delay(float delay);
    //�������̃R�[���o�b�N��ݒ肷��
    TweenUnit& on_complete(std::function<void()> complete_func);
    //���O���擾����
    const std::string& name() const;
    //���O��ݒ肷��
    TweenUnit& name(const std::string& name);

	//�R�s�[�֎~
	TweenUnit(const TweenUnit& other) = delete;
	TweenUnit& operator = (const TweenUnit& other) = delete;

protected:
	//�h���N���X���Ƃ̍X�V
	virtual void on_update(float progress){}

private:
	float duration_{ 0.0f }; //�A�j���[�V��������
	float elapsed_{ 0.0f }; //�o�ߎ��ԁi�t���[���j
	bool is_finished_{ false }; //�I��������7
    EaseType ease_type_{ EaseType::Linear }; //�C�[�W���O���
    float overshoot_{ 1.70158f }; //Elastic, Back, Punch�Ŏg������
    float delay_{ 0.0f }; //�J�n�x������
    std::function<void()> complete_func_{}; //�������̃R�[���o�b�N
    std::string name_{}; //���O�i�L�����Z�������Ɏg���j

    //�l�ɃC�[�W���O��K�p����
    static float apply_ease(float value, EaseType ease_type, float overshoot);
};

#endif