#ifndef TWEEN_H_
#define TWEEN_H_

#include <list>
#include<memory>
#include "TweenUnit.h"

//Tween���[�V�������Ǘ�����static�N���X
//1��1�̓�����TweenUnit�N���X���s��
//���̃N���X�ł́A������Tween���[�V�����̊Ǘ����s��
class Tween {
public:
	//TweenUnit��ǉ����ĊǗ����ɒu��
	static void add(std::shared_ptr<TweenUnit> unit);
	//�Ǘ�����TweenUnit�̍X�V�������Ăяo��
	static void update(float delta_time);
	//�Ǘ�����TweenUnit��S�č폜����
	static void clear();
	//float�^�̒l��Tween����
	static TweenUnit& value(float from, float to, float duration,
		std::function<void(float)> update_func);
	//Vector3�^�̒l��Tween����
	static TweenUnit& vector3(const GSvector3& from, const GSvector3& to, float duration,
		std::function<void(const GSvector3&)> update_func);
	//Vector2�^�̒l��Tween����
	static TweenUnit& vector2(const GSvector2& from, const GSvector2& to, float duration,
		std::function<void(const GSvector2&)> update_func);
	//�w�肳�ꂽ���Ԍ�ɏ��������s����
	static TweenUnit& delay_call(float delay_time, std::function<void()> callback);
	//�w�肳�ꂽ���O��Tween���L�����Z���i�폜�j����
	static void cancel(const std::string& name);

private:
	//�C���X�^���X�����֎~
	Tween() = delete;
	//������TweenUnit���Ǘ����邽�߂̃��X�g
	static std::list<std::shared_ptr<TweenUnit>> units_;
};

#endif