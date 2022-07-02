#include "Witch.h"
#include"../Player/Player.h"
#include"../Effect/FireSphere.h"
#include"../../World/IWorld.h"
#include"../../Collision/Field.h"
#include"../../Collision/Line.h"
#include"../AttackCollider.h"
#include"../../Assets.h"
#include<GSeffect.h>

//���[�V�����ԍ�
enum {
	MotionExplosion,
	MotionSpitFire,
	MotionDead,
	MotionIdle,
	MotionRun
};
//�d��
const float Gravity{ -0.016f };
//�ő�̗�
const float MaxHP{ 15 };
//����o������
const float RunDistance{ 10.0f };
//����X�s�[�h
const float RunSpeed{ 0.1f };
//�_���[�W��
const int HitDamage{ 5 };

Witch::Witch(std::shared_ptr<IWorld> world, const GSvector3& position) :
	mesh_{ Mesh_Witch,Mesh_Witch, Mesh_Witch, MotionIdle },
	state_{ State::Idle },
	motion_{ MotionIdle }{
	//���[���h�ݒ�
	world_ = world;
	//�^�O
	tag_ = "EnemyTag";
	//���O
	name_ = "Witch";
	//�v���C���[�����悤
	player_ = nullptr;
	//�ǂɏՓˉ����H
	hit_wall = false;
	//���S�������H
	is_dead_ = false;
	//�����蔻�苅
	collider_ = BoundingSphere{ 0.5f, GSvector3{0.0f, 1.0f, 0.0f} };
	HP_ = { MaxHP };
	//���W�̏�����
	transform_.position(position);
	// ���b�V���̕ϊ��s���������
	mesh_.transform(transform_.localToWorldMatrix());
	//�C�x���g�o�^
	mesh_.add_event(MotionSpitFire, 23.0f, [=] {spit_fire(); });
}

//�X�V
void Witch::update(float delta_time) {
	//�v���C���[����
	player_ = world_->find_actor("Player");
	//��Ԃ̍X�V
	update_state(delta_time);
	//�d��
	velocity_.y += Gravity * delta_time;
	transform_.translate(0.0f, velocity_.y, 0.0f);
	//�t�B�[���h�̔���
	collide_field();
	//���[�V�����ύX
	mesh_.change_motion(motion_, motion_loop_);
	//�A�j���[�V�����X�V
	mesh_.update(delta_time);
	//// ���b�V���̕ϊ��s����X�V
	mesh_.transform(transform_.localToWorldMatrix());
}

//�`��
void Witch::draw() const 
{
	mesh_.draw();
}

//�����蔻��
void Witch::react(Actor& other) 
{
	//�G�ƏՓ˂�����
	if (other.tag() == "EnemyTag")
	{
		collide_actor(other);
	}
	//�v���C���[�ƏՓ˂�����
	if (other.tag() == "PlayerAttackTag")
	{
		//se��炷
		gsPlaySE(Se_EnemyDamage);
		//�C���l
		GSvector3 Offset{ 0.0f,0.5f,0.0f };
		//�G�t�F�N���o���ʒu
		GSvector3 Pos = transform_.position()+Offset;
		//�G�t�F�N�g�\��
		gsPlayEffect(Effect_Blood, &Pos);
		//�_���[�W
		HP_.hit_damage(HitDamage);
	}
	//���S���Ă邩�H
	if (HP_.is_end())
	{
		//�����蔻��𖳌�
		enable_collider_ = false;
		//���S
		is_dead_ = true;
		gsPlaySE(Se_WitchDetate);
		change_state(Witch::State::Dead, MotionDead, false);
	}
}

//��Ԃ̍X�V
void Witch::update_state(float delta_time) {
	switch (state_)
	{
	case Witch::State::Idle:idle(delta_time); break;
	case Witch::State::Run:run(delta_time); break;
	case Witch::State::Dead:dead(delta_time); break;
	case Witch::State::Attack:attack(delta_time); break;
	}
	state_timer_ += delta_time;
}

//��Ԃ̕ύX
void Witch::change_state(State state, int motion, bool loop) 
{
	motion_ = motion;
	motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.0f;
	hit_wall_ = false;
}

//�A�C�h��
void Witch::idle(float delta_time) 
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
			attack_selection();
			return;
		}
	}

}
//����
void Witch::run(float delta_time) 
{
	// �O�i����i���[�J�����W��j
	transform_.translate(0.0f, 0.0f, RunSpeed * delta_time);
	//�U���͈͂��A�ǂɂԂ��������H
	if (is_attack() || is_hit_wall()) 
	{
		attack_selection();
	}
}

//���S
void Witch::dead(float delta_time)
{
	if (state_timer_ >= mesh_.motion_end_time())
	{
		//���S
		dead_ = true;
	}
}

//�U����
void Witch::attack(float delta_time)
{
	if (state_timer_ >= mesh_.motion_end_time())
	{
		change_state(State::Idle, MotionIdle, false);
	}

}

//���̍U��
void Witch::attack_selection() 
{
	// �^�[�Q�b�g�����̊p�x�����߂�
	float angle = target_signed_angle();
	// ������ς���
	transform_.rotate(0.0f, angle, 0.0f);
	change_state(State::Attack, MotionSpitFire, false);
}

void Witch::spit_fire() 
{
	// �e�𐶐�����ꏊ�̋���
	const float GenerateDistance{ 1.0f };
	// ��������ʒu�̍����̕␳�l
	const float GenerateHeight{ 1.0f };
	// �e�̈ړ��X�s�[�h
	const float Speed{ 0.4f };
	// �����ʒu�̌v�Z
	GSvector3 position = transform_.position() + transform_.forward() * GenerateDistance;
	// �����ʒu�̍�����␳����
	position.y += GenerateHeight;
	// �ړ��ʂ̌v�Z
	GSvector3 velocity = GSvector3::zero();
	velocity = transform_.forward() * Speed;

	// �e�̐���
	world_->add_actor(std::make_unique< FireSphere>(world_, position, velocity));
}

//���邩�H
bool Witch::is_run() const
{
	return target_distance() < RunDistance;
}
//�U���͈͂��H
bool Witch::is_attack() const
{
	return target_distance() >= RunDistance;
}
