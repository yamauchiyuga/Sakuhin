#ifndef WORLD_H_
#define WORLD_H_

#include"IWorld.h"
#include"../Actor/ActorManager.h"
#include<gslib.h>
#include<memory>

//���[���h�N���X
class World : public IWorld
{
public:
	//�R���X�g���N�^
	World() = default;
	//�f�X�g���N�^
	~World();
	//�X�V
	void update(float delta_time);
	//�`��
	void draw()const;
	//GUI�`��
	void draw_gui()const;
	//�폜
	void clear();
	//�J�����ǉ�
	void add_camera(std::shared_ptr<Actor> camera);
	//���C�g�ǉ�
	void add_light(std::shared_ptr<Actor> light);
	//�t�B�[���h�ǉ�
	void add_field(std::shared_ptr<Field> field);


	//�A�N�^�[�ǉ�
	virtual void add_actor(std::shared_ptr<Actor> actor)override;
	//�A�N�^�[����
	virtual std::shared_ptr<Actor>find_actor(const std::string& name)const override;
	 // �w�肵���^�O�������A�N�^�[�̌���
	virtual std::vector<std::shared_ptr<Actor>> find_actor_with_tag(const std::string& tag) const override;
	// �A�N�^�[����Ԃ�
	virtual int count_actor() const override;
	// �w�肵���^�O�������A�N�^�[����Ԃ�
	virtual int count_actor_with_tag(const std::string& tag) const override;
	// ���b�Z�[�W�̑��M
	virtual void send_message(const std::string& message, std::shared_ptr<void> param = nullptr) override;

	//�t�F�[�h�摜�̕`��
	void draw_fade() const;
	//
	virtual void game_over()override;
	//
	virtual void game_cler()override;
	//�Q�[���I�[�o�[�摜
	void draw_game_over_texture()const;
	//�Q�[���N���A�摜
	void draw_game_cler_texture()const;

	// �J�����̎擾
	virtual std::shared_ptr<Actor> camera() override;
	// ���C�g�̎擾
	virtual std::shared_ptr<Actor> light() override;
	// �t�B�[���h�̎擾
	virtual std::shared_ptr<Field> field() override;
	//�Q�[���I�[�o�[���ǂ����H
	bool is_game_over()const;

	// �V���h�E�}�b�v�̕`��p�̊֐�
	static void shadow_map_callback(void* param, const GSmatrix4* view, const GSmatrix4* projection);
	// �R�s�[�֎~
	World(const World& other) = delete;
	World& operator = (const World& other) = delete;

private:
	// �A�N�^�[�}�l�[�W���[
	ActorManager  actors_;
	// ���C�g
	std::shared_ptr<Actor> light_{ nullptr };
	// �J����
	std::shared_ptr<Actor> camera_{ nullptr };
	// �t�B�[���h
	std::shared_ptr<Field> field_{ nullptr };

	//�Q�[���I�[�o�[���H
	bool is_game_over_{ false };
	//�t�F�[�h�摜�̃A���t�@�l
	float fade_alpha_{ 0.0f };
	//���S�摜�̃A���t�@�l
	float game_over_alpha_{ 0.0f };
	//�N���A�摜�̃A���t�@�l
	float clear_alpha_{ 0.0f };
	//�N���A�摜�̑傫��
	GSvector2 clear_scale_{ 1.5f, 1.5f };
};
#endif // !WORLD_H_
