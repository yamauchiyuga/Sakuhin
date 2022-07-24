#include "thunder.h"
#include"../../World/IWorld.h"
#include"../../Assets.h"
#include<GSeffect.h>

constexpr float DelayTime{ 12.0f };

Thunder::Thunder(std::shared_ptr<IWorld> world, const GSvector3& position,const std::string& attack_name)
{
	//
	world_ = world;
	//
	player_ = world_->find_actor("Player");
	//
	tag_ = "EnemyAttackTag";
	// アクター名
	name_ = attack_name;
	// 衝突判定用の球体を設定
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
		die();
	}
	++timer_;
}


void Thunder::react(Actor& other)
{
	if (other.tag() == "PlayerTag") {
		// 衝突したら死亡
		die();
		gsStopEffect(Effect_Thunder);
	}
}
