#include "thunder.h"
#include"../../World/IWorld.h"
#include"../../Assets.h"
#include<GSeffect.h>

constexpr float DelayTime{ 12.0f };

Thunder::Thunder(std::shared_ptr<IWorld> world, const GSvector3& position)
{
	//
	world_ = world;
	//
	player_ = world_->find_actor("Player");
	//
	tag_ = "EnemyAttackTag";
	// �A�N�^�[��
	name_ = "EnemyAttack";
	// �Փ˔���p�̋��̂�ݒ�
	collider_ = BoundingSphere{ 2.0f };
	enable_collider_ = false;
	transform_.position(position);
	gsPlayEffect(Effect_Thunder, &position);
	
}

void Thunder::update(float delta_time)
{

	if (timer_ > DelayTime) 
	{
		enable_collider_ = true;
	}
	++timer_;
}


void Thunder::react(Actor& other)
{
	if (other.tag() == "PlayerTag") {
		// �Փ˂����玀�S
		die();
		gsStopEffect(Effect_Thunder);
	}
}
