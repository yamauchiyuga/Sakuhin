#ifndef ISCENE_H_
#define ISCENE_H_

#include <string>

class IScene 
{
public:
	//���z�f�X�g���N�^
	virtual ~IScene() = default;
	//�J�n
	virtual void start() = 0;
	//�X�V
	virtual void update(float delta_time) = 0;
	//�`��
	virtual void draw()const = 0;

	//�I�����Ă��邩
	virtual bool is_end() const = 0;
	//���̃V�[����Ԃ�
	virtual std::string next() const = 0;
	//�I��
	virtual void end() = 0;
};
#endif // !ISCENE_H_
