#include"CameraTPS.h"
#include"../World/IWorld.h"
#include"../Collision/Field.h"
#include"../Collision/Line.h"
#include"../Actor/Enemy/Enemy.h"
#include"../Assets.h"
#include"../Input.h"
#include<vector>
#include<algorithm>
#include<utility>


enum  State
{
	PlayerLockOn,
	EnemyLockOn,
	LookAtPlayerFromEnemy
};

//�v���C���[����̑��΍��W
const GSvector3 PlayerOffset{ 0.0f, 2.0f, -6.5f };

//�R���X�g���N�^
CameraTPS::CameraTPS(std::shared_ptr<IWorld> world, const GSvector3& position, const GSvector3& at) :
	state_{ State::PlayerLockOn },
	player_{ nullptr },
	target_{ nullptr }
{
	world_ = world;
	// �^�O�̐ݒ�
	tag_ = "CameraTag";
	// ���O�̐ݒ�
	name_ = "Camera";
	// ���_�̈ʒu��ݒ�
	transform_.position(position);

	// �����_��ݒ�i�����_�̕����Ɍ�����ς���j
	transform_.lookAt(at);
	//x�����̉�]�p�x�̏�����
	pitch_ = 0;
	//y�����̉�]�p�x�̏�����
	yaw_ = 0;
	//
	timer_ = 0;
}

void CameraTPS::update(float delta_time)
{
	player_update();

	if (is_player_dead()) return;
	input_update();

	state_update(delta_time);
	field_react();
	set_parameter();
}
void CameraTPS::draw()const
{
	// �����ˉe�̕ϊ��s���ݒ肷��
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		45.0f,
		1280.0f / 720.0f,
		0.3f,
		100.0f
	);


	// ���_�̈ʒu
	GSvector3 eye = transform_.position();
	// �����_�̈ʒu
	GSvector3 at = eye + transform_.forward();
	// �����
	GSvector3 up = transform_.up();

	// �J�����̐ݒ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,     // ���_�̈ʒu
		at.x, at.y, at.z,      // �����_�̈ʒu
		up.x, up.y, up.z       // ���_�̏����
	);

	if (state_ == State::EnemyLockOn)
	{
		//���b�N�I���̃e�N�X�`���`��
		const GSvector3 Offset{ 0.0f,1.0f,0.0f };
		GSvector3 Pos = target_->transform().position() + Offset;
		GSrect billboard_size{ -1.0f, 1.0f, 1.0f, -1.0f };  // �r���{�[�h�̑傫��
		gsDrawSprite3D(Texture_Lock, &Pos, &billboard_size, NULL, NULL, NULL, 0.0f);

	}
}
void CameraTPS::input_update()
{
	input_horizontal_amount_ = Input::get_right_horizontal();
	input_vertical_amount_ = Input::get_right_vertical();

	if (!Input::is_lock_on()) return;
	if (state_ == State::LookAtPlayerFromEnemy) return;
	if (state_ == State::EnemyLockOn) state_++;
	else if (decide_targe()) state_++;
}
void CameraTPS::state_update(float delta_time)
{
	switch (state_)
	{
	case State::PlayerLockOn:player_lock_on(delta_time); break;
	case State::EnemyLockOn:
		if (!enemy_lock_on(delta_time)) state_ = State::PlayerLockOn; break;
	case State::LookAtPlayerFromEnemy:
		if (lock_on_enemy_to_player(delta_time)) state_ = State::PlayerLockOn;
		break;
	default:break;
	}
}
void CameraTPS::player_update()
{
	std::shared_ptr<Actor> temp{ world_->find_actor("Player") };
	player_ = std::move(temp);
}
void CameraTPS::player_lock_on(float delta_time)
{
	operation_processor(delta_time);
	//�����_�̍��W�����߂�
	set_reference_point(player_->transform().position());
	//�J�����̍��W�����߂�
	position_ = at_ + GSquaternion::euler(pitch_, yaw_, 0.0f) * PlayerOffset;

	static const float SmoothTime{ 30.0f }, MaxSpeed{ 0.5f };
	GSvector3::smoothDamp(transform_.position(), position_, velocity_, SmoothTime, MaxSpeed, delta_time);
	linear_interpolation(SmoothTime, MaxSpeed, delta_time);
}
bool CameraTPS::enemy_lock_on(float delta_time)
{
	if (target_->is_dead()) return false;

	static const float Distance{ 6.5f };
	static const float CamerHeightPos{ 3.5f };

	GSvector3 EnemyToPlayer = player_->transform().position() - target_->transform().position();
	EnemyToPlayer = EnemyToPlayer.normalized() * Distance;
	EnemyToPlayer.y = CamerHeightPos;

	//�����_�̈ʒu�����߂�
	set_reference_point(target_->transform().position());
	//���_�ʒu�����߂�(�v���[���[�̔w��̍��W)
	position_ = player_->transform().position() + EnemyToPlayer;

	static const float SmoothTime{ 30.0f }, MaxSpeed{ 0.5f };
	linear_interpolation(SmoothTime, MaxSpeed, delta_time);

	return true;
}
bool CameraTPS::lock_on_enemy_to_player(float delta_time) {
	//�����_�̍��W�����߂�
	set_reference_point(player_->transform().position());
	//�J�����̍��W�����߂�
	position_ = at_ + GSquaternion::euler(pitch_, yaw_, 0.0f) * PlayerOffset;

	static const float SmoothTime{ 12.0f }, MaxSpeed{ 1.0f };
	linear_interpolation(SmoothTime, MaxSpeed, delta_time);

	timer_ += delta_time;

	if (timer_ >= SmoothTime) {
		timer_ = 0;
		return true;
	}
	return false;
}
bool CameraTPS::decide_targe()
{
	target_.reset();
	std::vector<std::shared_ptr<Actor>> enemies = world_->find_actor_with_tag("EnemyTag");
	if (enemies.empty()) return false;

	std::vector<std::pair<float, std::shared_ptr<Actor>>> min_distance;

	for (const auto& e : enemies)
	{
		if (e->name() == "Dragon") set_target(e);
		else
		{
			GSvector3 EPos = e->transform().position();
			GSvector3 PPos = player_->transform().position();
			min_distance.push_back(std::make_pair(EPos.distance(PPos), e));
		}
	}

	std::sort(min_distance.begin(), min_distance.end());
	if (!target_) set_target(min_distance.front().second);

	return true;
}
void CameraTPS::field_react()
{
	static const GSvector3 ReferencePointOffset{ 0.0, 2.0f, 0.0f };
	Line line{ player_->transform().position() + ReferencePointOffset, position_ };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect))
	{
		position_ = intersect;
	}
}
void CameraTPS::set_parameter()
{
	//���_�̈ʒu�̐ݒ�
	transform_.position(position_);
	//�����_��ݒ�(�����_�̕����Ɍ�����ς���)
	transform_.lookAt(at_);
}
void CameraTPS::set_target(const std::shared_ptr<Actor>& target)
{
	std::shared_ptr<Actor> temp{ target };
	target_ = std::move(temp);
}
void CameraTPS::set_reference_point(const GSvector3& at)
{
	//�J�����̒����_�̕⊮
	static const GSvector3 ReferencePointOffset{ 0.0f, 2.0f, 0.0f };
	at_ = at + ReferencePointOffset;
}
void CameraTPS::operation_processor(float delta_time)
{
	//��]�X�s�[�h
	static const float RotateSpeed{ 3.5f };
	///y�����ɉ�]������
	yaw_ += input_horizontal_amount_ * -RotateSpeed * delta_time;
	//x�����ɉ�]������
	pitch_ += input_vertical_amount_ * -RotateSpeed * delta_time;
	//x���̊p�x�͈�
	static const GSvector2 AngleRange{ -15.0f, 25.0f };
	//x�����̉�]�𐧌�����
	pitch_ = CLAMP(pitch_, AngleRange.x, AngleRange.y);
}
void CameraTPS::linear_interpolation(const float time, const float max_speed, float delta_time)
{
	position_ = GSvector3::smoothDamp(transform_.position(), position_, velocity_, time, max_speed, delta_time);
}
bool CameraTPS::is_player_dead() const
{
	return !(bool)player_;
}