#include"World.h"
#include"../Collision/Field.h"
#include"../Actor/Actor.h"
#include"../Asset.h"
//�f�X�g���N�^
World::~World() {
	clear();
}

void World::update(float delta_time) {
	//�t�B�[���h�X�V
	field_->update(delta_time);
	//�A�N�^�[�X�V
	actors_.update(delta_time);
	//�A�N�^�[�Փ�
	actors_.collide();
	//�A�N�^�[�x���X�V
	actors_.late_update(delta_time);
	//�A�N�^�[�폜
	actors_.remove();
	//�J�����X�V
	camera_->update(delta_time);
	//���C�g�X�V
	light_->update(delta_time);
	// �V���h�E�}�b�v�̕`��
	gsDrawShadowMap(World::shadow_map_callback, (void*)this);

}
// �`��
void World::draw() const {
	// �J�����̕`��
	camera_->draw();
	// ���C�g�̕`��
	light_->draw();

	// �V���h�E�}�b�v�p�V�F�[�_�[���o�C���h����
	gsBindDefaultMeshShader(Shader_ShadowMapMesh);
	gsBindDefaultSkinMeshShader(Shader_ShadowMapSkinnedMesh);
	gsBindDefaultOctreeShader(Shader_ShadowMapMesh);

	// �V���h�E�}�b�v�̕`��
	gsDrawShadowMap(World::shadow_map_callback, (void*)this);

	// �ʏ�`��p�̃V�F�[�_�[���o���h����
	gsBindDefaultMeshShader(Shader_StandardMesh);
	gsBindDefaultSkinMeshShader(Shader_StandardSkinnedMesh);
	gsBindDefaultOctreeShader(Shader_StandardOctree);


	// �t�B�[���h�̕`��
	field_->draw();
	// �A�N�^�[�̕`��
	actors_.draw();
	// �������A�N�^�[�̕`��
	actors_.draw_transparent();
	// GUI�̕`��
	actors_.draw_gui();
}

// ����
void World::clear() {
	// �A�N�^�[������
	actors_.clear();
	// �J����������
	delete camera_;
	camera_ = nullptr;
	// ���C�g������
	delete light_;
	light_ = nullptr;
	// �t�B�[���h������
	delete field_;
	field_ = nullptr;
}
// �J�����̒ǉ�
void World::add_camera(Actor* camera) {
	delete camera_;	// ���݂̃J�������폜
	camera_ = camera;
}

// ���C�g�̒ǉ�
void World::add_light(Actor* light) {
	delete light_;	// ���݂̃��C�g���폜
	light_ = light;
}

// �t�B�[���h�̒ǉ�
void World::add_field(Field* field) {
	delete field_;	// ���݂̃t�B�[���h���폜
	field_ = field;
}

// �V���h�E�}�b�v�̕`��p�̊֐�
void World::shadow_map_callback(void* param, const GSmatrix4* view, const GSmatrix4* projection) {
	World* self = (World*)param;
	// �V���h�E�}�b�v�ɂ̓A�N�^�[�̂ݕ`��
	self->actors_.draw();
}


// �A�N�^�[�̒ǉ�
void World::add_actor(Actor* actor) {
	actors_.add(actor);
}

// �A�N�^�[�̌���
Actor* World::find_actor(const std::string& name) const {
	return actors_.find(name);
}

// �w�肵���^�O�������A�N�^�[�̌���
std::vector<Actor*> World::find_actor_with_tag(const std::string& tag) const {
	return actors_.find_with_tag(tag);
}

// �A�N�^�[����Ԃ�
int World::count_actor() const {
	return actors_.count();
}

// �w�肵���^�O�������A�N�^�[����Ԃ�
int World::count_actor_with_tag(const std::string& tag) const {
	return actors_.count_with_tag(tag);
}

// ���b�Z�[�W���M
void World::send_message(const std::string& message, void* param) {
	actors_.send_message(message, param);
}

void World::game_over(){

}

void World::game_cler(){

}

// �J�����̎擾
Actor* World::camera() {
	return camera_;
}

// ���C�g�̎擾
Actor* World::light() {
	return light_;
}

// �t�B�[���h�̎擾
Field* World::field() {
	return field_;
}

bool World::is_game_over()const {
	return is_game_over_;
}