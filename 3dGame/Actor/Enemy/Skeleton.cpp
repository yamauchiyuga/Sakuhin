#include "Skeleton.h"
#include"../Player/Player.h"
#include"../../World/IWorld.h"
#include"../../Collision/Field.h"
#include"../../Collision/Line.h"
#include"../AttackCollider.h"
#include"../../Assets.h"
#include<GSeffect.h>

//���[�V�����ԍ�
enum
{
	MotionAttack,
	MotionDead,
	MotionRun,
	MotionTurnLeft,
	MotionTurnRight,
	MotionGeneration,
	MotionIdle
};

// �d��
const float Gravity{ -0.016f };
//�ő�̗�
const float MaxHP{ 10 };
//����o������
const float RunDistance{ 2.5f };
//����X�s�[�h
const float RunSpeed{ 0.06f };
//��]��
const float TurnAngle{ 2.5f };
//��]����p�x
const float TurnAroundAngle{ 10.0f };
//
const float AnimationDelaytime{ 60.0f };
//�_���[�W
const int HitDamage{ 5 };

Skeleton::Skeleton(std::shared_ptr<IWorld> world, const GSvector3& position) :
	mesh_{ Mesh_Skeleton,Mesh_Skeleton, Mesh_Skeleton, MotionGeneration,false },
	state_{ State::Generation },
	motion_{ MotionGeneration }
{
	//���[���h�ݒ�
	world_ = world;
	//�^�O
	tag_ = "EnemyTag";
	//���O
	name_ = "Skeleton";
	is_dead_ = false;
	player_ = nullptr;
	//����p��
	collider_ = BoundingSphere{ 0.4f, GSvector3{0.0f, 1.0f, 0.0f} };
	HP_ = { MaxHP };
	//���W�̏�����
	transform_.position(position);
	// ���b�V���̕ϊ��s���������
	mesh_.transform(transform_.localToWorldMatrix());
	//�C�x���g�o�^
	mesh_.add_event(MotionAttack, 10.0f, [=] {slash(); });
	mesh_.add_event(MotionAttack, 10.0f, [] {gsPlaySE(Se_PlayerAttack); });
}

//�X�V
void Skeleton::update(float delta_time)
{
	//�v���C���[����
	player_ = world_->find_actor("Player");
	//��Ԃ̍X�V
	update_state(delta_time);
	//�d�͉��Z
	velocity_.y += Gravity * delta_time;
	transform_.translate(0.0f, velocity_.y, 0.0f);
	//�t�B�[���h�̔���
	collide_field();
	//��Ԃ̕ύX
	mesh_.change_motion(motion_, motion_loop_);
	//�A�j���[�V�����X�V
	mesh_.update(delta_time);
	//// ���b�V���̕ϊ��s����X�V
	mesh_.transform(transform_.localToWorldMatrix());
}

//�`��
void Skeleton::draw() const
{
	mesh_.draw();
}

void Skeleton::react(Actor& other)
{
	//�������Ȃ画�肵�Ȃ�
	if (state_ == State::Generation)return;
	//�G�Ƃ̔���
	if (other.tag() == "EnemyTag")
	{
		collide_actor(other);
	}
	//�v���C���[�Ƃ̔���
	if (other.tag() == "PlayerAttackTag")
	{
		//SE��炷
		gsPlaySE(Se_EnemyDamage);
		//�C���l
		GSvector3 Offset{ 0.0f,0.6f,0.0f };
		//�G�t�F�N���o���ʒu
		GSvector3 Pos = transform_.position() + Offset;
		//�G�t�F�N�`��
		gsPlayEffect(Effect_Blood, &Pos);
		//�_���[�W
		HP_.hit_damage(HitDamage);
	}

	//���S������
	if (HP_.is_end())
	{
		//�����蔻��𖳌���
		enable_collider_ = false;
		//���S
		is_dead_ = true;
		gsPlaySE(Se_SkeletonDetate);
		change_state(Skeleton::State::Dead, MotionDead, false);
	}

}

//��Ԃ̍X�V
void Skeleton::update_state(float delta_time)
{
	switch (state_) {
	case Skeleton::State::Generation: generation(delta_time); break;
	case Skeleton::State::Idle:idle(delta_time); break;
	case Skeleton::State::Run:run(delta_time); break;
	case Skeleton::State::Turn:turn(delta_time); break;
	case Skeleton::State::Attack:attack(delta_time); break;
	case Skeleton::State::Dead:dead(delta_time); break;
	}
	state_timer_ += delta_time;
}

//��Ԃ̕ύX
void Skeleton::change_state(State state, int motion, bool loop) {
	motion_ = motion;
	motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.0f;
}

//����
void Skeleton::generation(float delta_time)
{
	if (state_timer_ >= mesh_.motion_end_time())
	{
		change_state(State::Idle, MotionIdle, true);
	}
}

//�A�C�h��
void Skeleton::idle(float delta_time)
{
	if (state_timer_ >= mesh_.motion_end_time() + AnimationDelaytime)
	{
		//�U��������H
		if (is_trun())
		{
			GSint motion = (target_signed_angle() >= 0.0f) ? MotionTurnLeft : MotionTurnRight;
			// �U�������ԂɑJ��
			change_state(State::Turn, motion, true);
			return;
		}
		//���邩�H
		if (is_run())
		{
			change_state(State::Run, MotionRun, true);
			return;
		}
		//�U���͈͂��H
		if (is_attack())
		{
			change_state(State::Attack, MotionAttack, false);
			return;
		}
	}
}

//����
void Skeleton::run(float delta_time) 
{
	// �^�[�Q�b�g�����̊p�x�����߂�
	float angle = CLAMP(target_signed_angle(), -TurnAngle, TurnAngle);
	// ������ς���
	transform_.rotate(0.0f, angle, 0.0f);
	// �O�i����i���[�J�����W��j
	transform_.translate(0.0f, 0.0f, RunSpeed * delta_time);
	if (is_attack())
	{
		change_state(State::Attack, MotionAttack, false);
	}
}

//�U�����
void Skeleton::turn(float delta_time) 
{
	if (state_timer_ >= mesh_.motion_end_time())
	{
		//���邩�H
		if (is_run())
		{
			change_state(State::Run, MotionRun, true);
			return;
		}
		//�U���͈͂��H
		if (is_attack())
		{
			change_state(State::Attack, MotionAttack, false);
			return;
		}
	}
	// �U��������[�V���������Ȃ���^�[�Q�b�g����������
	float angle = (target_signed_angle() >= 0.0f) ? TurnAngle : -TurnAngle;
	transform_.rotate(0.0f, angle * delta_time, 0.0f);
}

//���S
void Skeleton::dead(float delta_time) 
{
	if (state_timer_ >= mesh_.motion_end_time()) {
		dead_ = true;
	}
}

//�U��
void Skeleton::attack(float delta_time)
{
	if (state_timer_ >= mesh_.motion_end_time()) 
	{
		if (is_trun()) {
			GSint motion = (target_signed_angle() >= 0.0f) ? MotionTurnLeft : MotionTurnRight;
			// �U�������ԂɑJ��
			change_state(State::Turn, motion);
		}
		change_state(State::Idle, MotionIdle, true);
	}
}

//�U���̔��萶��
void Skeleton::slash() 
{
	// �U������̔��a
	const float AttackColliderRadius{ 0.5f };
	// �U��������o��������ꏊ�̋���
	const float AttackColliderDistance{ 1.5f };
	// �U��������o���ꏊ�̍���
	const float AttackColliderHeight{ 1.0f };
	// �U�����肪�L���ɂȂ�܂ł̒x������
	const float AttackCollideDelay{ 0.0f };
	// �U������̎���
	const float AttackCollideLifeSpan{ 5.0f };
	//�U���p������
	generate_attac_collider(AttackColliderRadius, AttackColliderDistance, AttackColliderHeight, 0.0f, AttackCollideDelay, AttackCollideLifeSpan);
}

//�����邩�H
bool Skeleton::is_run() const
{
	return target_distance() > RunDistance;
}

//�U��������H
bool Skeleton::is_trun() const
{
	return target_angle() >= TurnAroundAngle;
}

//�U���͈͂��H
bool Skeleton::is_attack() const 
{
	return target_distance() <= RunDistance && target_angle() <= TurnAroundAngle;
}
