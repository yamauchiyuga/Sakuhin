#include"Player.h"
#include"../AttackCollider.h"
#include "../../World/IWorld.h"
#include "../../Collision/Field.h"
#include "../../Collision/Line.h"
#include "../../Assets.h"
#include"../../Input.h"
#include <GSeffect.h>

//���[�V�����ԍ�
enum
{
	MotionAttack,
	MotionAttack2,
	MotionAttack3,
	MotionDamage,
	MotionEnd,
	MotionIdle,
	MotionGuardAccept,
	MotionGuardLoop,
	MotionGuardStart,
	MotionDodge,
	MotionRun
};
//�X�|�[������ʒu
const GSvector3 RestartPosition{ -5.0f,6.0f,10.0f };


//����
const float PlayerHeight{ 1.0f };
//���̔��a
const float PlayerRadius{ 0.7f };
//�⊮
const float FootOffset{ 0.1f };
//�d��
const float Gravity{ -0.016f };
//�������
const float DodgeDistance{ 0.7 };
//�ő�̗�
const float MaxHP{ 100.0f };
//�ő�X�^�~�i
const float MaxST{ 100.0f };
//����
const float Input{ 0.3f };
//���X�|�[������y���̍���
const float EndtLinePos_Y{ -50.0f };
//�������X�^�~�i
const int DodgeStamina{ 10 };
//�K�[�h����X�^�~�i
const int GuradStamin{ 20 };
//�U������X�^�~�i
const int AttackStamina{ 10 };
//�ő�R���{��
const int MaxCombo{ 2 };
//�_���[�W��
const int Damage{ 5 };


//�R���X�g���N�^
Player::Player(std::shared_ptr<IWorld> world, const GSvector3& position) :
	mesh_{ Mesh_Player,Mesh_Player, Mesh_Player, MotionIdle ,true },
	motion_{ MotionIdle },
	motion_loop_{ true },
	det_line_{ false },
	state_{ State::Move },
	state_timer_{ 0.0f },
	combo_{ 0 },
	HP_{ MaxHP },
	ST_{ MaxST }
{
	// ���[���h��ݒ�
	world_ = world;
	//���O
	name_ = "Player";
	//�^�O
	tag_ = "PlayerTag";
	//�����蔻�苅
	collider_ = BoundingSphere{ PlayerRadius,GSvector3{0.0f,PlayerHeight,0.0f} };
	// ���W�̏�����
	transform_.position(position);
	// ���b�V���̕ϊ��s���������
	mesh_.transform(transform_.localToWorldMatrix());

	//�C�x���g�o�^
	mesh_.add_event(MotionAttack, 15.0f, [=] {generate_attac_collider(); });
	mesh_.add_event(MotionAttack2, 15.0f, [=] {generate_attac_collider(); });
	mesh_.add_event(MotionAttack3, 22.0f, [=] {generate_attac_collider(); });
	mesh_.add_event(MotionRun, 8.0f, [] {gsPlaySE(Se_PlayerRun); });
	mesh_.add_event(MotionRun, 30.0f, [] {gsPlaySE(Se_PlayerRun); });
	mesh_.add_event(MotionDodge, 5.0f, [] {gsPlaySE(Se_PlayerDrop); });
	mesh_.add_event(MotionAttack, 10.0f, [] {gsPlaySE(Se_PlayerAttack); });
	mesh_.add_event(MotionAttack2, 10.0f, [] {gsPlaySE(Se_PlayerAttack); });
	mesh_.add_event(MotionAttack3, 10.0f, [] {gsPlaySE(Se_PlayerAttack); });
}

//�X�V
void Player::update(float delta_time)
{
	//
	update_state(delta_time);
	//�d�͉��Z
	velocity_.y += Gravity * delta_time;
	transform_.translate(0.0f, velocity_.y, 0.0f);
	//�t�B�[���h�̓����蔻��
	collide_field();
	//��Ԃ̕ύX
	mesh_.change_motion(motion_, motion_loop_);
	//�A�j���[�V�����X�V
	mesh_.update(delta_time);
	// ���b�V���̕ϊ��s����X�V
	mesh_.transform(transform_.localToWorldMatrix());
	//�X�^�~�i�̍X�V
	ST_.update(delta_time);
	//���X�|�[��
	if (end_line())
	{
		transform_.position() = GSvector3::zero();
		transform_.position(RestartPosition);
	}

}

//�`��
void Player::draw()const
{
	mesh_.draw();
	if (enable_collider_) {
		collider().draw();
	}
}

//GUI�`��
void Player::draw_gui()const
{
	HP_.draw_player();
	ST_.draw();
}

//�Փ˔���
void Player::react(Actor& other)
{
	//�G�Ƃ̔���
	if (other.tag() == "EnemyTag")
	{
		collide_actor(other);
		return;
	}
	//��𒆂Ȃ牽�����Ȃ�
	if (state_ == State::Dodge)return;
	//�_���[�W���Ȃ牽�����Ȃ�
	if (state_ == State::Damage)return;
	//���S���Ȃ牽�����Ȃ�
	if (state_ == State::End)return;

	//�G�̍U��������������
	if (other.tag() == "EnemyAttackTag")
	{
		//�K�[�h�\���H
		if (can_guard())
		{
			// �m�b�N�o�b�N����
			knock_back(other, 0.2f);
			//SE��炷
			gsPlaySE(Se_PlayerBlock);
			//�K�[�h�̃X�^�~�i����
			ST_.consumption_stamina(GuradStamin);
			change_state(State::Guarding, MotionGuardAccept, false);
			return;
		}
		//�G�t�F�N���o��
		const GSvector3 Offset{ 0.0f,0.5f,0.0f };
		const GSvector3 Pos = transform_.position() + Offset;
		gsPlayEffect(Effect_Blood, &Pos);
		//SE��炷
		gsPlaySE(Se_PlayerDamage);
		//�m�b�N�o�b�N
		knock_back(other, 0.3f);
		//�����蔻��𖳌�
		enable_collider_ = false;
		//�R���{�̃��b�Z�g
		combo_ = 0;
		change_state(State::Damage, MotionDamage, false);
		//�_���[�W
		HP_.hit_damage(Damage);
	}
	//���S
	if (HP_.is_end())
	{
		change_state(State::End, MotionEnd, false);
		//�Q�[���I�[�o�[���Ăяo��
		world_->game_over();
	}
}

//��Ԃ̍X�V
void Player::update_state(float delta_time) {
	//
	switch (state_) {
	case Player::State::Move: move(delta_time);    break;
	case Player::State::Attack:attack(delta_time); break;
	case Player::State::Dodge: dodge(delta_time); break;
	case Player::State::GuardStart: guard_start(delta_time); break;
	case Player::State::Guarding: guarding(delta_time); break;
	case Player::State::Damage:damage(delta_time); break;
	case Player::State::End: end(delta_time); break;
	}
	//
	state_timer_ += delta_time;
}

//��Ԃ̕ύX
void Player::change_state(State state, GSuint motion, bool loop) {
	motion_ = motion;
	motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.0f;
}

//�ړ�
void Player::move(float delta_time)
{
	//����{�^�������������A�X�^�~�i�͑���邩
	if (Input::is_dodge() && ST_.get_stamina() > DodgeStamina) {
		//�O�����ɉ���
		velocity_ = transform_.forward() * DodgeDistance;
		//�X�^�~�i����
		ST_.consumption_stamina(DodgeStamina);
		change_state(State::Dodge, MotionDodge, false);
		return;
	}

	if (Input::is_attack() && ST_.get_stamina() > AttackStamina) {
		ST_.consumption_stamina(AttackStamina);
		change_state(State::Attack, MotionAttack, false);
		return;
	}
	//�K�[�h�\���H
	if (Input::is_guard()) {
		change_state(State::GuardStart, MotionGuardStart, false);
		return;
	}

	int motion = MotionIdle;
	//�ړ����x
	const float WalkSpeed{ 0.1f };
	//�J�����̑O�������擾
	GSvector3 forward = world_->camera()->transform().forward();
	forward.y = 0.0f;
	//�J�����̉E�������擾
	GSvector3 right = world_->camera()->transform().right();
	right.y = 0.0f;

	//�ړ���
	GSvector3 velocity{ 0.0f,0.0f,0.0f };

	if (Input::get_left_vertical() > Input)velocity += forward;
	if (Input::get_left_vertical() < -Input)velocity += -forward;
	if (Input::get_left_horizontal() > Input)velocity += right;
	if (Input::get_left_horizontal() < -Input)velocity += -right;
	//���K�����Ĉړ����x���|����
	velocity = velocity.normalize() * WalkSpeed * delta_time;

	//��]���x
	const float Rotate{ 30.0f };

	if (velocity_.length() != 0.0f)
	{
		GSquaternion rotation = GSquaternion::rotateTowards(transform_.rotation(),
			GSquaternion::lookRotation(velocity), Rotate * delta_time);
		transform_.rotation(rotation);
		// �ړ����̃��[�V�����ɂ���
		motion = MotionRun;
	}
	velocity_.x = velocity.x;
	velocity_.z = velocity.z;
	change_state(State::Move, motion);
	transform_.translate(velocity_, GStransform::Space::World);
}


//�U����
void Player::attack(float delta_time) {
	//����{�^���������Ă��邩�A�X�^�~�i�͑���Ă��邩�H
	if (Input::is_dodge() && ST_.get_stamina() > DodgeStamina) {
		turn();
		velocity_ = transform_.forward() * DodgeDistance;
		ST_.consumption_stamina(DodgeStamina);
		change_state(State::Dodge, MotionDodge, false);
		return;
	}
	//�U���{�^���������Ă��邩�A�U���\���H
	if (Input::is_attack() && can_attackable()) {
		turn();
		//�X�^�~�i����
		ST_.consumption_stamina(AttackStamina);
		//���[�V�����ԍ�
		int  motion = NULL;
		++combo_;
		switch (combo_) {
		case 1:
			motion = MotionAttack2;
			break;
		case 2:
			motion = MotionAttack3;
			break;
		}
		change_state(State::Attack, motion, false);
	}

	//���[�V�����I�����H
	if (state_timer_ >= mesh_.motion_end_time()) {
		//�R���{���Z�b�g
		combo_ = 0.0f;
		change_state(State::Move, MotionIdle, true);
	}

}
//���
void Player::dodge(float delta_time)
{
	transform_.translate(velocity_, GStransform::Space::World);
	//����
	const float decrement_value{ 0.2f };
	velocity_ -= GSvector3{ velocity_.x,0.0f,velocity_.z }*decrement_value * delta_time;
	//���[�V�����I�����H
	if (state_timer_ >= mesh_.motion_end_time() - 20.0f)
	{
		//�R���{���b�Z�g
		combo_ = 0;
		change_state(State::Move, MotionIdle, true);
	}
}
//�K�[�h�X�^�[�g
void Player::guard_start(float delta_time) {
	//�K�[�h�{�^���������ꂽ��
	if (!gsXBoxPadButtonState(0, GS_XBOX_PAD_LEFT_SHOULDER)) {
		//�ړ����ɑJ��
		change_state(State::Move, MotionIdle);
	}
}

//�K�[�h��
void Player::guarding(float delta_time) {
	//�m�b�N�o�b�N����
	velocity_ -= GSvector3{ velocity_.x, 0.0f, velocity_.z } *0.2f * delta_time;
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	//���[�V�����I�����H
	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::GuardStart, MotionGuardLoop, true);
	}
}

//�_���[�W��
void Player::damage(float delta_time) {
	//�m�b�N�o�b�N����
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	//����������
	const float damp{ 0.4f };
	velocity_ -= GSvector3{ velocity_.x, 0.0f, velocity_.z } *damp * delta_time;
	//�_���[�W���[�V�����̏I����҂�
	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::Move, MotionIdle, false);
		enable_collider_ = true;
		gsStopEffect(Effect_Blood);
	}
}


//���S
void Player::end(float delta_time) {
	//�������Ȃ�
}

//�U�����
void Player::turn()
{
	//�J�����̑O�������擾
	GSvector3 forwad = world_->camera()->transform().forward();
	forwad.y = 0.0f;
	//�J�����̉E�������擾
	GSvector3 right = world_->camera()->transform().right();
	right.y = 0.0f;
	//�ړ���
	GSvector3 velocity{ 0.0f,0.0f,0.0f };
	if (Input::get_left_vertical() > Input)velocity += forwad;
	if (Input::get_left_vertical() < -Input)velocity += -forwad;
	if (Input::get_left_horizontal() > Input)velocity += right;
	if (Input::get_left_horizontal() < -Input)velocity += -right;
	//���K��
	velocity = velocity.normalize();
	//�ړ��ʂ�0����Ȃ��Ȃ�
	if (velocity_.length() != 0.0f) {
		//��]������
		GSquaternion rotation = GSquaternion::lookRotation(velocity);
		transform_.rotation(rotation);
	}
}


//�U���\���H
bool Player::can_attackable() const
{
	//���݂̃X�^�~�i�������葽�����H
	if (ST_.get_stamina() < AttackStamina)return false;
	//���݂̃R���{���ő�R���{�𒴂��Ă��邩�H
	if (combo_ >= MaxCombo) return false;
	const float NextAttackTime{ 40.0f };
	//���݂̃^�C�}�[�����̍U�����Ԃ𒴂��Ă��邩�H
	if (state_timer_ <= mesh_.motion_end_time() - NextAttackTime)return false;
	return true;
}
//�K�[�h�\���H
bool Player::can_guard() const
{
	//���݂̃X�^�~�i�������葽�����H
	if (ST_.get_stamina() <= GuradStamin)return false;
	//�K�[�h��ԈȊO���H
	if (state_ != State::GuardStart)return false;
	//
	return true;
}
//���S���C���𒴂��Ă邩�H
bool Player::end_line()const {
	if (transform_.position().y >= EndtLinePos_Y)return false;
	return true;
}
//�m�b�N�o�b�N
void Player::knock_back(Actor& other, float power) {
	//�^�[�Q�b�g�����̃x�N�g�������߂�
	GSvector3 to_target = other.transform().position() - transform().position();
	//y�����͖����ɂ���
	to_target.y = 0.0f;
	//�^�[�Q�b�g�����Ƌt�����Ƀm�b�N�o�b�N����ړ��ʂ����߂�
	velocity_ = to_target.getNormalized() * power;
}
//�t�B�[���h�����蔻��
void Player::collide_field() {
	//
	GSvector3 center; // �Փˌ�̋��̂̒��S���W
	if (world_->field()->collide(collider(), &center)) {
		// y���W�͕ύX���Ȃ�
		center.y = transform_.position().y;
		// �␳��̍��W�ɕύX����
		transform_.position(center);
	}

	// �n�ʂƂ̏Փ˔���i�����Ƃ̌�������j
	GSvector3 position = transform_.position();
	Line line;
	line.start = position + collider_.center_;
	line.end = position + GSvector3{ 0.0f, -FootOffset, 0.0f };
	GSvector3 intersect;  // �n�ʂƂ̌�_
	if (world_->field()->collide(line, &intersect)) {
		// ��_�̈ʒu����y���W�̂ݕ␳����
		position.y = intersect.y;
		// ���W��ύX����
		transform_.position(position);
		// �d�͂�����������
		velocity_.y = 0.0f;
	}
}
//�A�N�^�[�����蔻��
void Player::collide_actor(Actor& other) {
	//y���W���������W�����߂�
	GSvector3 position = collider().center_;
	GSvector3 target = other.collider().center_;
	//����Ƃ̋���
	float distance = GSvector3::distance(position, target);
	//�Փ˔��苅�̔��a���m�����������������߂�
	float length = collider_.radius_ + other.collider().radius_;
	//�Փ˔��苅�̏d�Ȃ��Ă��钷�������߂�
	float overlap = length - distance;
	//�d�Ȃ��Ă��镔���̔����̋������������ړ��ʂ����߂�
	GSvector3 v = (position - target).getNormalized() * overlap;
	transform_.translate(v, GStransform::Space::World);
	//�t�B�[���h�Ƃ̏Փ˔���
	collide_field();
}
//�U�����萶��
void Player::generate_attac_collider() {
	// �U��������o��������ꏊ�̋���
	const float AttackColliderDistance{ 1.8f };
	// �U������̔��a
	const float AttackColliderRadius{ 1.3f };
	// �U��������o���ꏊ�̍���
	const float AttackColliderHeight{ 1.0f };

	// �U�����肪�L���ɂȂ�܂ł̒x������
	const float AttackCollideDelay{ 5.0f };
	// �U������̎���
	const float AttackCollideLifeSpan{ 5.0f };

	// �Փ˔�����o����������W�����߂�i�O���̈ʒu�j
	GSvector3 position = transform_.position() + transform_.forward() * AttackColliderDistance;
	// �����̕␳�i��������̍����j
	position.y += AttackColliderHeight;
	// �Փ˔���p�̋����쐬
	BoundingSphere collider{ AttackColliderRadius, position };
	// �Փ˔�����o��������
	world_->add_actor(std::make_unique<AttackCollider>(world_, collider,
		"PlayerAttackTag", "PlayerAttack", tag_, AttackCollideLifeSpan, AttackCollideDelay));
}