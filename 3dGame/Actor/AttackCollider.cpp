#include"AttackCollider.h"

//�R���X�g���N�^
AttackCollider::AttackCollider(std::shared_ptr<IWorld>world, const BoundingSphere& collider,
	const std::string& tag, const std::string& name,
	const std::string& owner_tag,
	float lifespan, float delay) :
	lifespan_timer_{ lifespan }, delay_timer_{ delay }
{
	//���[���h�ݒ�
	world_ = world;
	//�^�O���̐ݒ�
	tag_ = tag;
	//�����҂̃^�O��
	owner_tag_ = owner_tag;
	//���O�̐ݒ�
	name_ = name;

	//�Փ˔���̏�����
	collider_ = BoundingSphere{ collider.radius_ };
	//���W�̏�����
	transform_.position(collider.center_);
	//�Փ˔����������
	enable_collider_ = false;
}

//�X�V
void AttackCollider::update(float delta_time)
{
	//�x�����Ԃ��o�߂�����Փ˔����L���ɂ���
	if (delay_timer_ <= 0) 
	{
		//�Փ˔����L��
		enable_collider_ = true;

		//�������s�����玀�S
		if (lifespan_timer_ <= 0) 
		{
			die();
		}
		//�����̍X�V
		lifespan_timer_ -= delta_time;
	}
	//�x�����Ԃ̍X�V
	delay_timer_ -= delta_time;
}

void AttackCollider::draw()const 
{
	//�Փ˔���p�̃f�o�b�O�\��
	if (enable_collider_) {
		collider().draw();
	}
}

//�Փ˃��A�N�V����
void AttackCollider::react(Actor& ohter)
{
	// �����Ɠ����^�O�܂��́A�I�[�i�[�Ɠ����^�O�͏Փ˔��肵�Ȃ�
	if (ohter.tag() == tag() || ohter.tag() == owner_tag_)return;
	//�Փ˂����玀�S
	die();
}