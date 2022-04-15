#include"SceneManager.h"
#include"SceneNull.h"

static SceneNull scene_null;		//null�V�[���i�������Ȃ��_�~�[�V�[���j

//�R���X�g���N�^
SceneManager::SceneManager() :
	current_scene_{ &scene_null } {
}

//�f�X�g���N�^
SceneManager::~SceneManager() {
	clear();
}

//�X�V
void SceneManager::update(float delta_time) {
	//�V�[���X�V
	current_scene_->update(delta_time);
	//�V�[�����I�����Ă��邩
	if (current_scene_->is_end()) {
		//�V�[���ύX����
		change(current_scene_->next());
	}
}

//�`��
void SceneManager::draw()const {
	//���݂̃V�[����`��
	current_scene_->draw();
}

//�I��
void SceneManager::end() {
	//���݂̃V�[�����I��
	current_scene_->end();
	//null�V�[���ɂ���
	current_scene_ = &scene_null;
}

//�V�[���̒ǉ�
void SceneManager::add(const std::string& name,IScene* scene) {
	scenes_[name] = scene;
}
//�V�[���ύX
void SceneManager::change(const std::string& name) {
	//���݂̃V�[�����I��
	end();
	//���݂̃V�[����ύX
	current_scene_ = scenes_[name];
	//���݂̃V�[�����J�n
	current_scene_->start();
}

//
void SceneManager::clear(){
	//
	end();
	//
	for (auto& pair : scenes_) {
		delete pair.second;
	}
	//
	scenes_.clear();
}