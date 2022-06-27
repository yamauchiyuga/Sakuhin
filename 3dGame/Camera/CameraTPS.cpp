#include"CameraTPS.h"
#include"../World/IWorld.h"
#include"../Collision/Field.h"
#include"../Collision/Line.h"
#include"../Actor/Enemy/Enemy.h"
#include"../Input.h"
#include<vector>
#include<algorithm>
#include<utility>
#include<string>

enum  State
{
	PlayerLockOn,
	EnemyLockOn,
	LookAtPlayerFromEnemy
};

//�J�����̒����_�̕⊮
const GSvector3 ReferencePointOffset{ 0.0f, 2.0f, 0.0f };
//�v���C���[����̑��΍��W
const GSvector3 PlayerOffset{ 0.0f, 2.0f, -6.5f };

//�R���X�g���N�^
CameraTPS::CameraTPS(std::shared_ptr<IWorld> world, const GSvector3& position, const GSvector3& at) :
	state_{ State::PlayerLockOn }
{
	world_ = world;
	// �^�O�̐ݒ�
	tag_ = "CameraTag";
	// ���O�̐ݒ�
	name_ = "Camera";
	// ���_�̈ʒu��ݒ�
	transform_.position(position);
	//
	target_ = GSvector3::zero();
	// �����_��ݒ�i�����_�̕����Ɍ�����ς���j
	transform_.lookAt(at);
	//x�����̉�]�p�x�̏�����
	pitch_ = 0;
	//y�����̉�]�p�x�̏�����
	yaw_ = 0;
	//
	timer_=0;
}

//�X�V
void CameraTPS::update(float delta_time) 
{
	if (Input::is_lock_on()) 
	{
		state_update(delta_time);
	}
	OutputDebugString(std::to_string(state_).c_str());
	switch (state_)
	{
	case State::PlayerLockOn:player_lock_on(delta_time); break;
	case State::EnemyLockOn:enemy_lock_on(delta_time); break;
	case State::LookAtPlayerFromEnemy:look_at_player_from_enemy(delta_time); break;
	}
}

void CameraTPS::state_update(float delta_time)
{
	
	if (state_ == State::LookAtPlayerFromEnemy) return;
	if (state_ == State::EnemyLockOn) 
	{
		state_++;
	}
	else if(decide_targe())
	{
		state_++;
	}
	return;
}

void CameraTPS::player_lock_on(float delta_time) 
{
	std::shared_ptr<Actor> Player = world_->find_actor("Player");
	//�v���[���[��������Ȃ������炻�̂܂ܕԂ�
	if (!Player) return;
	//��]�X�s�[�h
	const float RotateSpeed{ 3.5f };
	///y�����ɉ�]������
	yaw_ += Input::get_right_horizontal() * -RotateSpeed * delta_time;
	//x�����ɉ�]������
	pitch_ += Input::get_right_vertical() * -RotateSpeed * delta_time;

	//x���̍Œ�p�x
	const float MinAngle{ -15.0f };
	//x���̍ő�p�x
	const float MaxAngle{ 25.0f };
	//x�����̉�]�𐧌�����
	pitch_ = CLAMP(pitch_, MinAngle, MaxAngle);


	//�����_�̍��W�����߂�
	GSvector3 at = Player->transform().position() + ReferencePointOffset;
	//�J�����̍��W�����߂�
	GSvector3 position = at + GSquaternion::euler(pitch_, yaw_, 0.0f) * PlayerOffset;

	const float SmoothTime{ 13.0f };    // ��ԃt���[����
	const float MaxSpeed{ 1.0f };       // �ړ��X�s�[�h�̍ő�l
	position = GSvector3::smoothDamp(transform_.position(), position, velocity_, SmoothTime, MaxSpeed, delta_time);

	//�t�B�[���h�Ƃ̏Փ˔���
	Line line{ at,position };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) 
	{
		position = intersect;
	}

	//���_�̈ʒu�̐ݒ�
	transform_.position(position);
	//�����_��ݒ�(�����_�̕����Ɍ�����ς���)
	transform_.lookAt(at);
}

void CameraTPS::enemy_lock_on(float delta_time) 
{
	GSvector3 at = GSvector3::zero();
	GSvector3 Position = GSvector3::zero();
	std::shared_ptr<Actor> Player = world_->find_actor("Player");

	const float Distance{ 6.5f };
	const float CamerHeightPos{ 3.5f };

	GSvector3 enemy_to_player = Player->transform().position() - target_;
	enemy_to_player.normalize();
	enemy_to_player = enemy_to_player * Distance;
	enemy_to_player.y = CamerHeightPos;

	//�����_�̈ʒu�����߂�
	at = target_ + ReferencePointOffset;
	//���_�ʒu�����߂�(�v���[���[�̔w��̍��W)
	Position = Player->transform().position() + enemy_to_player;

	const float smoothtime{ 30.0f };
	const float maxspeed{ 1.0f };

	Position = GSvector3::smoothDamp(transform_.position(), Position, velocity_, smoothtime, maxspeed, delta_time);

	//�t�B�[���h�Ƃ̏Փ˔���
	Line line{ Player->transform().position() + ReferencePointOffset,Position };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect))
	{
		Position = intersect;
	}
	//���_�̈ʒu�̐ݒ�
	transform_.position(Position);
	//�����_��ݒ�(�����_�̕����Ɍ�����ς���)
	transform_.lookAt(at);
}

void CameraTPS::look_at_player_from_enemy(float delta_time) {
	GSvector3 at = GSvector3::zero();
	GSvector3 Position = GSvector3::zero();
	std::shared_ptr<Actor> Player = world_->find_actor("Player");
	//�����_�̍��W�����߂�
	at = Player->transform().position() + ReferencePointOffset;
	//�J�����̍��W�����߂�
	Position = at + GSquaternion::euler(pitch_, yaw_, 0.0f) * PlayerOffset;
	const float smoothtime{ 12.0f };
	const float maxspeed{ 1.0f };
	Position = GSvector3::smoothDamp(transform_.position(), Position, velocity_, smoothtime, maxspeed, delta_time);
	timer_ +=delta_time;
	//���_�̈ʒu�̐ݒ�
	transform_.position(Position);
	//�����_��ݒ�(�����_�̕����Ɍ�����ς���)
	transform_.lookAt(at);
	if (timer_ > smoothtime) {
		state_ = State::PlayerLockOn;
		timer_ = 0;
	}
}



bool CameraTPS::decide_targe() 
{
	std::shared_ptr<Actor> Player = world_->find_actor("Player");
	std::vector<std::shared_ptr<Actor>> enemy = world_->find_actor_with_tag("EnemyTag");
	std::vector<std::pair<float, std::shared_ptr<Actor>>> min_distance;
	bool is_dragon = false;
	if (enemy.size() == 0) 
	{
		return false;
	}
	for (const auto& e : enemy) 
	{

		if (e->name() == "Dragon") 
		{
			target_ = e->transform().position();
			is_dragon = true;
		}
		else if (e->name() == "Skeketon" || e->name() == "Witch") 
		{
			GSvector3 e_pos = e->transform().position();
			GSvector3 p_pos = Player->transform().position();
			min_distance.push_back(std::make_pair(e_pos.distance(p_pos), e));
		}
	}
	std::sort(min_distance.begin(), min_distance.end());
	if (!is_dragon) 
	{
		target_ = min_distance[0].second->transform().position();
	}
	return true;
}


//�`��
void CameraTPS::draw()const 
{
	//���_�̈ʒu
	GSvector3 eye = transform_.position();
	//�����_�̈ʒu
	GSvector3 at = eye + transform_.forward();
	//
	GSvector3 up = transform_.up();
	// �J�����̐ݒ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,     // ���_�̈ʒu
		at.x, at.y, at.z,      // �����_�̈ʒu
		up.x, up.y, up.z       // ���_�̏����
	);
}

