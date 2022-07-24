#include"FireSphere.h"
#include"../../World/IWorld.h"
#include"../../Collision/Field.h"
#include"../../Collision/Line.h"
#include"../../Assets.h"
#include<GSeffect.h>

FireSphere::FireSphere(std::shared_ptr<IWorld> world, const GSvector3& position, const GSvector3& velocity,const std::string& attack_name) {
	// ���[���h��ݒ�
	world_ = world;
	// �^�O��
	tag_ = "EnemyAttackTag";
	// �A�N�^�[��
	name_ = attack_name;
	// �ړ��ʂ̏�����
	velocity_ = velocity;
	// �Փ˔���p�̋��̂�ݒ�
	collider_ = BoundingSphere{ 1.0f };
	// ���W�̏�����
	transform_.position(position);
	 effect_=gsPlayEffect(Effect_FireBall, &position);
	gsPlaySE(Se_DragonSpitFire);
}

void FireSphere::update(float delta_time) {
	// �t�B�[���h�Ƃ̏Փ˔���
	Line line;
	line.start = transform_.position();
	line.end = transform_.position() + velocity_;
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		// ��_�̍��W�ɕ␳
		transform_.position(intersect);
		die();
		// �t�B�[���h�ɏՓ˂����玀�S
		GSvector3 position{ transform_.position() };
		gsStopEffect(effect_);
		effect_=gsPlayEffect(Effect_Explosion, &position);
		return;
	}

	// �ړ�����i���[���h���W�n��j
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
}

void FireSphere::draw() const {
	//�G�t�F�N�g�Ɏ��g�̃��[���h�ϊ��s���ݒ�
	GSmatrix4 world = transform_.localToWorldMatrix();
	gsSetEffectMatrix(effect_, &world);
}

void FireSphere::react(Actor& other) {

	if (other.tag() == "PlayerTag") {
		// �Փ˂����玀�S
		die();
		GSvector3 position{ transform_.position() };
		gsPlaySE(Se_DragonExplosion);
		gsStopEffect(effect_);
		effect_= gsPlayEffect(Effect_Explosion, &position);
	}

}
