#include "TitlePlayer.h"
#include"../../Assets.h"
#include "../../World/IWorld.h"
#include "../../Collision/Field.h"
#include "../../Collision/Line.h"
#include"../Enemy/TitleDragon.h"

enum
{
	MotionIdle = 5
};

//�⊮
constexpr float FootOffset{ 0.1f };
//�d��
constexpr float Gravity{ -0.016f };

TitlePlayer::TitlePlayer(std::shared_ptr<IWorld> world, const GSvector3& position) :
	mesh_{ Mesh_TitlePlayer,Mesh_TitlePlayer, Mesh_TitlePlayer, MotionIdle ,true },
	motion_{ MotionIdle },
	motion_loop_{ true }{
	// ���[���h��ݒ�
	world_ = world;
	//���O
	name_ = "TitlePlayer";
	//�^�O
	tag_ = "TitlePlayerTag";
	// ���W�̏�����
	transform_.position(position);
	// ���b�V���̕ϊ��s���������
	mesh_.transform(transform_.localToWorldMatrix());
	std::shared_ptr<Actor> enemy = world_->find_actor("TitleDragon");
	transform_.lookAt(enemy->transform().position());
};


void TitlePlayer::update(float delta_time)
{
	//�d�͉��Z
	//velocity_.y += Gravity * delta_time;
	//transform_.translate(0.0f, velocity_.y, 0.0f);
	//�t�B�[���h�̓����蔻��
	collide_field();
	//��Ԃ̕ύX
	mesh_.change_motion(motion_, motion_loop_);
	//�A�j���[�V�����X�V
	mesh_.update(delta_time);
	// ���b�V���̕ϊ��s����X�V
	mesh_.transform(transform_.localToWorldMatrix());
}

void TitlePlayer::draw() const
{
	mesh_.draw();
}

//�t�B�[���h�����蔻��
void TitlePlayer::collide_field() {
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