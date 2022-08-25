#include "ActorManager.h"
#include "Actor.h"

#include<algorithm>
// �f�X�g���N�^
ActorManager::~ActorManager()
{
	clear();
}

// �A�N�^�[�̒ǉ�
void ActorManager::add(std::shared_ptr<Actor> actor) 
{
	actors_.push_back(actor);
}

// �A�N�^�[�̍X�V
void ActorManager::update(float delta_time) 
{
	for (auto i = actors_.begin(); i != actors_.end(); ++i) 
	{
		(*i)->update(delta_time);
	}

}

// �A�N�^�[�̒x���X�V
void ActorManager::late_update(float delta_time) 
{
	for (auto i = actors_.begin(); i != actors_.end(); ++i) 
	{
		(*i)->late_update(delta_time);
	}
}

// �A�N�^�[�̕`��
void ActorManager::draw() const 
{
	for (auto i = actors_.begin(); i != actors_.end(); ++i) 
	{
		(*i)->draw();
	}
}

// �������A�N�^�[�̕`��
void ActorManager::draw_transparent() const 
{
	for (auto i = actors_.begin(); i != actors_.end(); ++i) 
	{
		(*i)->draw_transparent();
	}
}

// �A�N�^�[��GUI�`�� 
void ActorManager::draw_gui() const 
{
	for (auto i = actors_.begin(); i != actors_.end(); ++i) 
	{
		(*i)->draw_gui();
	}
}

// �A�N�^�[�̏Փ˔���
void ActorManager::collide() {
	for (auto i = actors_.begin(); i != actors_.end(); ++i) {
		for (auto j = std::next(i); j != actors_.end(); ++j) {
			(*i)->collide(**j);
		}
	}
}

// ���S���Ă���A�N�^�[�̍폜
void ActorManager::remove() 
{
	actors_.remove_if([](const auto& actor) {return actor->is_dead(); });
}

// �A�N�^�[�̌���
std::shared_ptr<Actor> ActorManager::find(const std::string& name) const
{

	for (auto i = actors_.begin(); i != actors_.end();++i ) 
	{
		if ((*i)->name() == name) 
		{
			return *i;
		}
	}
	return nullptr;
}

// �w�肵���^�O�������A�N�^�[�̌���
std::vector<std::shared_ptr<Actor>> ActorManager::find_with_tag(const std::string& tag) const 
{
	std::vector<std::shared_ptr<Actor>> result;
	for (auto i = actors_.begin(); i != actors_.end();++i ) 
	{
		if ((*i)->tag() == tag) 
		{
			result.push_back(*i);
		}
	}
	return result;

}

// �A�N�^�[����Ԃ�
int ActorManager::count() const
{
	return (int)actors_.size();
}

// �w�肵���^�O�̃A�N�^�[����Ԃ�
int ActorManager::count_with_tag(const std::string& tag) const 
{
	int result = 0;
	for (auto i = actors_.begin(); i != actors_.end();++i ) 
	{
		if ((*i)->tag() == tag) 
		{
			++result;
		}
	}
	return result;
}

// ���b�Z�[�W�̑��M
void ActorManager::send_message(const std::string& message, std::shared_ptr<void> param) 
{
	for (auto i = actors_.begin(); i != actors_.end(); ++i) 
	{
		(*i)->handle_message(message, param);
	}
}

// ����
void ActorManager::clear() 
{
	actors_.clear();
}

