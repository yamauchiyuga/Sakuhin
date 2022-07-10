#ifndef CAMERA_TPS_H_
#define CAMERA_TPS_H_

#include "../Actor/Actor.h"
#include<memory>
#include<string>
class CameraTPS :public Actor
{
public:
	//�R���X�g���N�^
	CameraTPS(std::shared_ptr<IWorld> world, const GSvector3& position, const GSvector3& at);
	//�X�V
	virtual void update(float delta_time)override;
	//�`��
	virtual void draw()const override;

private:
	//���͍X�V
	void input_update();
	//��Ԃ̍X�V
	void state_update(float delta_time);
	//�v���C���[
	void player_update();
	//�v���C���[�����b�N�I��
	void player_lock_on(float delta_time);
	//�G�����b�N�I��
	bool enemy_lock_on(float delta_time);
	//�G����v���C���[�֎��_��߂�
	bool lock_on_enemy_to_player(float delta_taime);
	//�^�[�Q�b�g������ł邩
	bool decide_targe();
	//�t�B�[���h�̏Փ˔���
	void field_react();
	//���_�̐ݒ�
	void set_parameter();
	//�^�[�Q�b�g���擾
	void set_target(const std::shared_ptr<Actor>& target);
	//�����_�̕⊮
	void set_reference_point(const GSvector3& at);
	//���_�̉�]�̏���
	void operation_processor(float delta_time);
	//���_�ړ��̕⊮
	void linear_interpolation(const float time, const float max_speed, float delta_time);
	//�v���C���[������ł邩
	bool is_player_dead() const;
private:
	
	std::shared_ptr<Actor> player_;
	std::shared_ptr<Actor> target_;

	GSvector3 at_;
	GSvector3 position_;

	int state_;

	float timer_{ 0.0f };
	float yaw_{ 0.0f };
	float pitch_{ 0.0f };
	float input_horizontal_amount_{ 0.0f };
	float input_vertical_amount_{ 0.0f };	

};

#endif // !CAMERA_TPS_H_
