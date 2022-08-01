#ifndef IWORLD_H_
#define IWORLD_H_
#include<string>
#include<vector>
#include<memory>

class Actor;
class Field;

//���[���h���ۃC���^�[�t�F�[�X
class IWorld {
public:
	//���z�f�X�g���N�^
	virtual ~IWorld() = default;

	//�A�N�^�[�ǉ�
	virtual void add_actor(std::shared_ptr<Actor> actor) = 0;
	//�A�N�^�[����
	virtual std::shared_ptr<Actor>find_actor(const std::string& name) const = 0;
	//�w�肵���^�O�������A�N�^�[�̌���
	virtual std::vector<std::shared_ptr<Actor>> find_actor_with_tag(const std::string& tag)const = 0;
	//�A�N�^�[����Ԃ�
	virtual int count_actor()const = 0;
	//�w�肵���^�O�������A�N�^�[��Ԃ�
	virtual int count_actor_with_tag(const std::string& tag)const = 0;
	//���b�Z�[�W���M
	virtual void send_message(const std::string& message, std::shared_ptr<void> param = nullptr) = 0;

	//�Q�[���I�[�o�[
	virtual void game_over() = 0;
	//�Q�[���N���A
	virtual void game_clear() = 0;

	// �t�B�[���h�̎擾
	virtual std::shared_ptr<Field> field() = 0;
	// �J�����̎擾
	virtual std::shared_ptr<Actor> camera() = 0;
	// ���C�g�̎擾
	virtual std::shared_ptr<Actor> light() = 0;

};
#endif 

