#include"World.h"
#include"../Collision/Field.h"
#include"../Actor/Actor.h"
#include"../Assets.h"
#include"../Tween/Tween.h"
#include <GSeffect.h>
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
	//�G�t�F�N�g�̍X�V�������s��
	gsUpdateEffect(delta_time);
	light_->update(delta_time);
}
// �`��
void World::draw() const {
	// �J�����̕`��
	camera_->draw();
	//�G�t�F�N�g�p�̃J������ݒ�
	gsSetEffectCamera();
	// ���C�g�̕`��
	light_->draw();
	 //�V���h�E�}�b�v�̕`��
	gsDrawShadowMap(World::shadow_map_callback, (void*)this);
	// �t�B�[���h�̕`��
	field_->draw();
	// �A�N�^�[�̕`��
	actors_.draw();
	// �������A�N�^�[�̕`��
	actors_.draw_transparent();
	//�G�t�F�N�`��
	gsDrawEffect();
	
}

void World::draw_gui() const {
	// GUI�̕`��
	actors_.draw_gui();
	//�Q�[���N���A��`��
	draw_game_cler_texture();
	//�Q�[���I�[�o�[��`��
	draw_game_over_texture();
	//�t�F�C�h�A�E�g
	draw_fade();


}
// ����
void World::clear() {
	// �A�N�^�[������
	actors_.clear();
	// �J����������
	//delete camera_;
	camera_ = nullptr;
	// ���C�g������
	//delete light_;
	light_ = nullptr;
	// �t�B�[���h������
	//delete field_;
	field_ = nullptr;
	//�e�ϐ��̒l��������
	is_game_over_ = false;
	fade_alpha_ = 0.0f;
	game_over_alpha_ = 0.0f;
	clear_alpha_ = 0.0f;
}
// �J�����̒ǉ�
void World::add_camera(std::shared_ptr<Actor> camera) {
	camera_ =camera;
}

// ���C�g�̒ǉ�
void World::add_light(std::shared_ptr<Actor> light) {
	light_ = light;
}

// �t�B�[���h�̒ǉ�
void World::add_field(std::shared_ptr<Field> field) {
	field_ = field;
}

// �V���h�E�}�b�v�̕`��p�̊֐�
void World::shadow_map_callback(void* param, const GSmatrix4* view, const GSmatrix4* projection) {
	World* self = (World*)param;
	// �V���h�E�}�b�v�ɂ̓A�N�^�[�̂ݕ`��
	self->actors_.draw();
}


// �A�N�^�[�̒ǉ�
void World::add_actor(std::shared_ptr<Actor> actor) {
	actors_.add(std::move(actor));
}

// �A�N�^�[�̌���
std::shared_ptr<Actor> World::find_actor(const std::string& name) const {
	return actors_.find(name);
}

// �w�肵���^�O�������A�N�^�[�̌���
std::vector<std::shared_ptr<Actor>> World::find_actor_with_tag(const std::string& tag) const {
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
void World::send_message(const std::string& message, std::shared_ptr<void> param) {
	actors_.send_message(message, param);
}
void World::draw_fade()const {
	const GScolor color{ 1.0f, 1.0f, 1.0f, fade_alpha_ };
	gsDrawSprite2D(Texture_Fade, NULL, NULL, NULL, &color, NULL, 0.0f);
}

//�Q�[���I�[�o�[
void World::game_over(){
	//120�t���[�������Ď��S�摜���t�F�[�h�C��
	Tween::value(0.0f, 1.0f, 120.0f, [=](GSfloat val) {game_over_alpha_ = val; }).delay(120.0f);
	//100�t���[�������ăV�[�����t�F�[�h�A�E�g
	Tween::value(0.0f, 1.0f, 100.0f, [=](GSfloat val) {fade_alpha_ = val; }).delay(300.0f)
		.on_complete([=] {is_game_over_ = true; });
}

//�Q�[���N���A
void World::game_cler(){

	//�摜�̑傫�����Z�b�g
	clear_scale_ = { 1.5f, 1.5f };
	//100�t���[�������ăV�[�����t�F�[�h�A�E�g
	Tween::value(0.0f, 1.0f, 100.0f, [=](GSfloat val) {fade_alpha_ = val; }).delay(540.0f)
		.on_complete([=] {is_game_over_ = true; });
	//20�t���[�������ăN���A�摜���t�F�[�h�C��
	Tween::value(0.0f, 1.0f, 20.0f, [=](GSfloat val) {clear_alpha_ = val; }).delay(300.0f)
		.on_complete([=] {gsPlaySE(Se_GameClear); });
	//20�t���[�������ăN���A�摜������������
	Tween::vector2(clear_scale_, GSvector2{ 1.0f, 1.0f }, 20.0f,[=](GSvector2 val) {clear_scale_ = val; })
		.delay(300.0f);
}

//�Q�[���I�[�o�[�e�N�X�`��
void World::draw_game_over_texture() const{
	const GScolor color{ 1.0f, 1.0f, 1.0f, game_over_alpha_ };
	gsDrawSprite2D(Texture_GameOver, NULL, NULL, NULL, &color, NULL, 0.0f);
}

//�Q�[���N���A�e�N�X�`��
void World::draw_game_cler_texture() const{
	const GSvector2 position{ 640.0f, 360.0f };
	const GScolor color{ 1.0f, 1.0f, 1.0f, clear_alpha_ };
	gsDrawSprite2D(Texture_GameClear, &position, NULL, &position, &color, &clear_scale_, 0.0f);
}

// �J�����̎擾
std::shared_ptr<Actor> World::camera() {
	return camera_;
}

// ���C�g�̎擾
std::shared_ptr<Actor> World::light() {
	return light_;
}

// �t�B�[���h�̎擾
std::shared_ptr<Field> World::field() {
	return field_;
}

//�Q�[���I�[�o�[���H
bool World::is_game_over()const {
	return is_game_over_;
}