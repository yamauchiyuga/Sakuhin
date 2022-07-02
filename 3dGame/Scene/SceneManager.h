#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include <unordered_map>
#include<string>
#include<memory>

class IScene;

class SceneManager {
public:
	//�R���X�g���N�^
	SceneManager();
	//�f�X�g���N�^
	~SceneManager();
	//�X�V
	void update(float delta_time);
	//�`��
	void draw()const;
	//�I��
	void end();
	//�V�[���̒ǉ�
	void add(const std::string& name, std::shared_ptr<IScene> scene);
	//�V�[���؂�ւ�
	void change(const std::string& name);
	//�폜
	void clear();
	//�R�s�[�֎~
	SceneManager(const SceneManager& other) = default;
	SceneManager& operator=(const SceneManager& other) = default;
private:
	// �V�[��
	std::unordered_map<std::string, std::shared_ptr<IScene>> scenes_;
	// ���݂̃V�[��
	std::shared_ptr<IScene> current_scene_;
};
#endif // !SCENE_MANAGER_H_

