#include "Dragon.h"
#include"../Effect/FireSphere.h"
#include"../../World/IWorld.h"
#include"../AttackCollider.h"
#include"../../Assets.h"
#include<GSeffect.h>

//���[�V�����ԍ�
enum
{
	MotionBite,
	MotionDead,
	MotionFly,
	MotionFlySpitFireball,
	MotionDamage,
	MotionGoInAir,
	MotionIdle,
	MotionLanding,
	MotionRun,
	MotionSpitFireball,
	MotionTailAttack,
	MotionTurnLeft,
	MotionTurnRight,
	MotionRunAttack
};

//�ő�̗�
const float MaxHP{ 400 };
//�d��
const float Gravity{ -0.003f };
//�U���͈�
const float AttackAngle{ 30.0f };
//����o������
const float RunDistance{ 7.0f };
//����X�s�[�h
const float RunSpeed{ 0.11f };
//��s�X�s�[�h
const float FlySpeed{ 0.12f };
//��]��
const float TurnAngle{ 2.0f };
//��]����p�x
const float TurnAroundAngle{ 25.0f };
//�_���[�W��
const int HitDamage{ 20 };

Dragon::Dragon(std::shared_ptr<IWorld>world, const GSvector3& position) :
	mesh_{ Mesh_Dragon,Mesh_Dragon, Mesh_Dragon, MotionIdle },
	state_{ State::Idle },
	motion_{ MotionIdle }
{
	//���[���h�ݒ�
	world_ = world;
	//�^�O
	tag_ = "EnemyTag";
	//���O
	name_ = "Dragon";
	player_ = nullptr;
	//����p��
	collider_ = BoundingSphere{ 2.0f, GSvector3{0.0f, 2.0f, 0.0f} };
	HP_= MaxHP;
	//���W�̏�����
	transform_.position(position);
	// ���b�V���̕ϊ��s���������
	mesh_.transform(transform_.localToWorldMatrix());
	//�C�x���g�o�^
	mesh_.add_event(MotionBite, 36.0f, [=] {bite(); });
	mesh_.add_event(MotionTailAttack, 45.0f, [=] {tail_attack(); });
	mesh_.add_event(MotionSpitFireball, 23.0f, [=] {spit_fire(); });
	mesh_.add_event(MotionSpitFireball, 23.0f, [=] {gsPlaySE(Se_DragonSpitFire); });
	mesh_.add_event(MotionFlySpitFireball, 30.0f, [=] {spit_fire(); });
	mesh_.add_event(MotionRun, 10, [] {gsPlaySE(Se_DragonFoot); });
	mesh_.add_event(MotionRun, 40, [] {gsPlaySE(Se_DragonFoot); });
	mesh_.add_event(MotionTurnRight, 15, [] {gsPlaySE(Se_DragonFoot); });
	mesh_.add_event(MotionTurnRight, 45, [] {gsPlaySE(Se_DragonFoot); });
	mesh_.add_event(MotionTurnLeft, 15, [] {gsPlaySE(Se_DragonFoot); });
	mesh_.add_event(MotionTurnLeft, 45, [] {gsPlaySE(Se_DragonFoot); });
	mesh_.add_event(MotionFly, 13, [] {gsPlaySE(Se_DragonFire); });
	mesh_.add_event(MotionLanding, 20, [] {gsPlaySE(Se_DragonLanding); });
	mesh_.add_event(MotionGoInAir, 30, [] {gsPlaySE(Se_DragonFire); });
	mesh_.add_event(MotionGoInAir, 80, [] {gsPlaySE(Se_DragonFire); });
	mesh_.add_event(MotionDead, 10, [] {gsPlaySE(Se_DragonDetate); });
}

//�X�V
void Dragon::update(float delta_time)
{
	//����
	player_ = world_->find_actor("Player");
	//��Ԃ̕ύX
	update_state(delta_time);
	//�t�B�[���h�̔���
	collide_field();
	//��Ԃ̕ύX
	mesh_.change_motion(motion_, motion_loop_);
	//�A�j���[�V�����X�V
	mesh_.update(delta_time);
	// ���b�V���̕ϊ��s����X�V
	mesh_.transform(transform_.localToWorldMatrix());
	//�d�͂��|���邩�H
	if (is_gravity())
	{
		//�d�͉��Z
		velocity_.y += Gravity * delta_time;
		transform_.translate(0.0f, velocity_.y, 0.0f);
	}
}

//�`��
void Dragon::draw() const
{
	mesh_.draw();
}

//GUI�`��
void Dragon::draw_gui() const
{
	HP_.draw_boss();
}

//
void Dragon::react(Actor& other)
{
	//�v���C���[�̍U���ɓ����������H
	if (other.tag() == "PlayerAttackTag")
	{
		if (HP_.cullent_health() ==200)
		{
			gsPlaySE(Se_DragonDamage);
			change_state(State::Damage, MotionDamage, false);
		}
		hit_stop_.set_hit_stop(15.0f);
		//SE��炷
		gsPlaySE(Se_EnemyDamage);
		const GSvector3 ZanOffset{ 0.0f,0.8f,0.0f };
		const GSvector3 ZanPos = transform_.position() + ZanOffset;
		gsPlayEffect(Effect_Attack1, &ZanPos);
		//�_���[�W
		HP_.hit_damage(HitDamage);
		//�G�t�F�N�̕`��
		GSvector3 Offset{ 0.0f,0.5f,0.0f };
		GSvector3 Pos = transform_.position() + Offset;
		gsPlayEffect(Effect_Blood, &Pos);
	}
	//���S
	if (HP_.is_end())
	{
		//�����蔻��𖳌�
		enable_collider_ = false;
		//���S����
		is_dead_ = true;
		change_state(Dragon::State::Dead, MotionDead, false);
		//�N���A��ʂ��Ă�
		world_->game_cler();
	}
}

//��Ԃ̍X�V
void Dragon::update_state(float delta_time)
{
	switch (state_) {
	case Dragon::State::Idle:idle(delta_time); break;
	case Dragon::State::Run:run(delta_time); break;
	case Dragon::State::Turn:turn(delta_time); break;
	case Dragon::State::Attack:attack(delta_time); break;
	case Dragon::State::FlyStart:fly_start(delta_time); break;
	case Dragon::State::FlyIdel:fly_idle(delta_time); break;
	case Dragon::State::FlyMove:fly_move(delta_time); break;
	case Dragon::State::FlyAttack:fly_attack(delta_time); break;
	case Dragon::State::FlyEnd:fly_end(delta_time); break;
	case Dragon::State::Damage:damage(delta_time); break;
	case Dragon::State::Dead:dead(delta_time); break;
	}
	state_timer_ += delta_time;
}

//��Ԃ̕ύX
void Dragon::change_state(State state, int motion, bool loop) {
	motion_ = motion;
	motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.0f;
}

//�A�C�h��
void Dragon::idle(float delta_time)
{
	//�U��������H
	if (is_trun())
	{
		//�v���C���[�Ƃ̊p�x��0�ȏォ�H
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
		//�U���������_���ɑI��
		int next = gsRand(0, 2);
		//��s�X�^�[�g
		if (next == 0)
		{
			GSvector3 position{ transform_.position() };
			gsPlayEffect(Effect_Smoke, &position);
			change_state(State::FlyStart, MotionGoInAir, false);
		}
		else
		{
			attack_selection();
		}
		return;
	}
}


//����
void Dragon::run(float delta_time)
{
	// �^�[�Q�b�g�����̊p�x�����߂�
	float angle = CLAMP(target_signed_angle(), -TurnAngle, TurnAngle);
	transform_.rotate(0.0f, angle * delta_time, 0.0f);
	transform_.translate(0.0f, 0.0f, RunSpeed * delta_time);
	if (is_attack())
	{
		attack_selection();
	}
}

//�U�����
void Dragon::turn(float delta_time)
{
	if (state_timer_ >= mesh_.motion_end_time())
	{
		//�s���������_���ɑI��
		int next = gsRand(0, 3);
		if (is_run())
		{
			//��s�X�^�[�g
			if (next == 0) {
				// �G�t�F�N���o��
				GSvector3 position{ transform_.position() };
				gsPlayEffect(Effect_Smoke, &position);
				change_state(State::FlyStart, MotionGoInAir, false);
			}
			//����
			else if (next == 1)
			{
				change_state(State::Run, MotionRun, true);
			}
			//�΋�
			else
			{
				change_state(State::Attack, MotionSpitFireball, false);
			}
			return;
		}

		if (is_attack())
		{
			attack_selection();
		}
	}

	// �U��������[�V���������Ȃ���^�[�Q�b�g����������
	float angle = (target_signed_angle() >= 0.0f) ? TurnAngle : -TurnAngle;
	transform_.rotate(0.0f, angle * delta_time, 0.0f);
}

//�U��
void Dragon::attack(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		if (is_trun())
		{
			GSint motion = (target_signed_angle() >= 0.0f) ? MotionTurnLeft : MotionTurnRight;
			// �U�������ԂɑJ��
			change_state(State::Turn, motion);
		}
		change_state(State::Idle, MotionIdle, false);
	}
}

//��s�X�^�[�g
void Dragon::fly_start(float delta_time)
{
	//�������߂�
	const int FlyStart{ 50 };
	if (state_timer_ >= FlyStart)
	{
		const float Height{ 6.0f };
		const float LerpTime{ 0.08f };
		//�㏸�ʒu�̌���
		GSvector3 fly_pos{ transform_.position().x,Height,transform_.position().z };
		transform_.position(GSvector3::lerp(transform_.position(), fly_pos, LerpTime * delta_time));
	}

	if (state_timer_ >= mesh_.motion_end_time())
	{
		change_state(State::FlyIdel, MotionFly, true);
	}
}

//��s��
void Dragon::fly_idle(float delta_time)
{
	if (state_timer_ >= mesh_.motion_end_time())
	{
		//�s���������_���ɑI��
		int next = gsRand(0, 2);
		//�󒆍U��
		if (next == 0)
		{
			change_state(State::FlyAttack, MotionFlySpitFireball, false);
		}
		//�ړ�
		else
		{
			change_state(State::FlyMove, MotionFly, true);
		}
	}
}

//��s�ړ�
void Dragon::fly_move(float delta_time)
{
	//�^�[�Q�b�g�����̊p�x�����߂�
	float angle = CLAMP(target_signed_angle(), -TurnAngle, TurnAngle);
	transform_.rotate(0.0f, angle * delta_time, 0.0f);
	transform_.translate(0.0f, 0.0f, FlySpeed * delta_time);

	if (is_attack())
	{
		//�s���������_���ɑI��
		int next = gsRand(0, 1);
		//�󒆍U��
		if (next == 0)
		{
			change_state(State::FlyAttack, MotionFlySpitFireball, false);
		}
		//���n
		else
		{
			//�G�t�F�N���o��
			GSvector3 position{ transform_.position() };
			gsPlayEffect(Effect_Smoke, &position);
			change_state(State::FlyEnd, MotionLanding, false);
		}
	}

	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::FlyMove, MotionFly, true);
	}
}

//�󒆍U��
void Dragon::fly_attack(float delta_time) {
	//�^�[�Q�b�g�����̊p�x�����߂�
	float angle = (target_signed_angle() >= 0.0f) ? TurnAngle : -TurnAngle;
	transform_.rotate(0.0f, angle * delta_time, 0.0f);
	transform_.translate(0.0f, 0.0f, -FlySpeed * delta_time);
	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::FlyIdel, MotionFly, true);
	}
}

//���n
void Dragon::fly_end(float delta_time) {
	//�d�͉��Z
	velocity_.y += Gravity * delta_time;
	transform_.translate(0.0f, velocity_.y, 0.0f);
	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::Idle, MotionIdle, false);
	}
}

//
void Dragon::damage(float delta_time)
{
	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::Idle, MotionIdle, false);
	}
}

//���S
void Dragon::dead(float delta_time) {
	//�������Ȃ�
}

//�U���I��
void Dragon::attack_selection() {
	GSuint motion = NULL;
	//�U���������_���ɑI��
	int attack = gsRand(0, 2);
	//���݂�
	if (attack == 0)
	{
		motion = MotionBite;
	}
	//�K��
	else if (attack == 1)
	{
		motion = MotionTailAttack;
	}
	//�΋�
	else if (attack == 2)
	{
		motion = MotionSpitFireball;
	}
	change_state(State::Attack, motion, false);
}

//���݂��U��
void Dragon::bite() {
	// �U������̔��a
	const float AttackColliderRadius{ 1.5f };
	// �U��������o��������ꏊ�̋���
	const float AttackColliderDistance{ 4.0f };
	// �U��������o���ꏊ�̍���
	const float AttackColliderHeight{ 1.0f };
	//
	float const AttackColliderWidth{ 0.0f };

	// �U�����肪�L���ɂȂ�܂ł̒x������
	const float AttackCollideDelay{ 9.0f };
	// �U������̎���
	const float AttackCollideLifeSpan{ 10.0f };
	//
	std::string AttackName{ "DragonBite" };
	generate_attac_collider(AttackColliderRadius, AttackColliderDistance, AttackColliderHeight, AttackColliderWidth, AttackCollideDelay, AttackCollideLifeSpan, AttackName);
	gsPlaySE(Se_DragonAttack2);
}

//�K���U��
void Dragon::tail_attack() {
	// �U������̔��a
	const float AttackColliderRadius{ 3.0f };
	// �U��������o��������ꏊ�̋���
	const float AttackColliderDistance{ 5.0f };
	// �U��������o���ꏊ�̍���
	const float AttackColliderHeight{ 1.5f };
	// �U��������o���ꏊ�̕�
	const float AttackColliderWidth{ 0.0f };
	// �U�����肪�L���ɂȂ�܂ł̒x������
	const float AttackCollideDelay{ 9.0f };
	// �U������̎���
	const float AttackCollideLifeSpan{ 10.0f };
	//
	std::string AttackName{ "DragonTail" };
	//����
	generate_attac_collider(AttackColliderRadius, AttackColliderDistance, AttackColliderHeight, AttackColliderWidth, AttackCollideDelay, AttackCollideLifeSpan, AttackName);
	gsPlaySE(Se_DragonAttack1);
}

//�΋�
void Dragon::spit_fire()
{
	// �e�𐶐�����ꏊ�̋���
	const float GenerateDistance{ 3.0f };
	// ��������ʒu�̍����̕␳�l
	const float GenerateHeight{ 1.0f };
	// �e�̈ړ��X�s�[�h
	const float Speed{ 0.6f };
	// �����ʒu�̌v�Z
	GSvector3 position = transform_.position() + transform_.forward() * GenerateDistance;
	// �����ʒu�̍�����␳����
	position.y += GenerateHeight;
	// �ړ��ʂ̌v�Z
	GSvector3 velocity = GSvector3::zero();
	velocity = transform_.forward() * Speed;
	//
	std::string AttackName{ "DragonSpitFire" };
	if (state_ == State::FlyAttack)
	{
		AttackName = "DragonFlySpitFire";
		velocity = (player_->transform().position() - position).normalized() * Speed;
	}
	// �e�̐���
	world_->add_actor(std::make_unique<FireSphere>(world_, position, velocity, AttackName));
}

//�U��Ԃ邩
bool Dragon::is_trun() const
{
	return target_angle() >= TurnAroundAngle;
}

//���邩�H
bool Dragon::is_run() const
{
	return   target_distance() > RunDistance;
}

//�U���͈͂��H
bool Dragon::is_attack()const
{
	return target_distance() <= RunDistance && target_angle() <= AttackAngle;
}

//�d�͂�L���ɂ��邩�H
bool Dragon::is_gravity() const
{
	//���ł�Ƃ��͖���
	if (state_ == State::FlyStart)return false;
	if (state_ == State::FlyIdel)return false;
	if (state_ == State::FlyMove)return false;
	if (state_ == State::FlyAttack)return false;
	if (state_ == State::FlyEnd)return false;
	return true;
}
