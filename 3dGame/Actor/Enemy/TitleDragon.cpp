#include "TitleDragon.h"
#include"../Player/Player.h"
#include"../../World/IWorld.h"
#include"../../Assets.h"

enum
{
	MotionIdle=2
};

TitleDragon::TitleDragon(std::shared_ptr<IWorld> world, const GSvector3& position):
	mesh_{ Mesh_TitleDragon,Mesh_TitleDragon, Mesh_TitleDragon, MotionIdle },
	motion_{ MotionIdle }
{
	//���[���h�ݒ�
	world_ = world;
	//�^�O
	tag_ = "EnemyTag";
	//���O
	name_ = "TitleDragon";
	player_ = world_->find_actor("Player");
	//���W�̏�����
	transform_.position(position);
	// ���b�V���̕ϊ��s���������
	mesh_.transform(transform_.localToWorldMatrix());
}


void TitleDragon::update(float delta_time)
{
	//�t�B�[���h�̔���
	collide_field();
	//��Ԃ̕ύX
	mesh_.change_motion(motion_, motion_loop_);
	//�A�j���[�V�����X�V
	mesh_.update(delta_time);
	// ���b�V���̕ϊ��s����X�V
	mesh_.transform(transform_.localToWorldMatrix());
}

void TitleDragon::draw() const
{
	mesh_.draw();
}
