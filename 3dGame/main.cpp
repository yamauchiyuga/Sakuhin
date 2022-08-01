#include<GSgame.h>
#include "Scene/SceneManager.h"
#include"Scene//TitleScene.h"
#include "Scene/GamePlayScene.h"
#include"Scene/LoadingScene.h"
#include"Tween/Tween.h"
#include"Assets.h"
#include"Input.h"
#include<memory>
#include<GSeffect.h>



class MyGame :public gslib::Game 
{
public:
	//
	//�R���X�g���N�^
	MyGame() : gslib::Game{ 1280, 720}{
	}

	// �J�n
	void start() override 
	{
		scene_manager_.add("LoadingScene", std::make_shared<LoadingScene>());
		scene_manager_.add("TitleScene", std::make_shared<TitleScene>());
		scene_manager_.add("GamePlayScene", std::make_shared<GamePlayScene>());
		scene_manager_.change("LoadingScene");
	}
	// �X�V
	void update(float delta_time)
	{
		scene_manager_.update(delta_time);
		Input::update(delta_time);
		Tween::update(delta_time);
	}
	// �`��
	void draw() override
	{
		scene_manager_.draw();
	}
	// �I��
	void end() 
	{
		scene_manager_.end();
		
		//�摜�̍폜
		gsDeleteTexture(Texture_Frame);
		gsDeleteTexture(Texture_HP);
		gsDeleteTexture(Texture_ST);
		gsDeleteTexture(Texture_Fade);
		gsDeleteTexture(Texture_Titel);
		gsDeleteTexture(Texture_GameClear);
		gsDeleteTexture(Texture_GameOver);
		gsDeleteTexture(Texture_Lock);
		gsDeleteTexture(Texture_TitelName);

		// ���b�V���̍폜
		gsDeleteMesh(Mesh_Player);
		gsDeleteMesh(Mesh_TitlePlayer);
		gsDeleteMesh(Mesh_TitleDragon);
		gsDeleteMesh(Mesh_Dragon);
		gsDeleteMesh(Mesh_Skeleton);
		gsDeleteMesh(Mesh_Witch);
		// �X�P���g���̍폜
		gsDeleteSkeleton(Mesh_TitlePlayer);
		gsDeleteSkeleton(Mesh_Player);
		gsDeleteSkeleton(Mesh_TitleDragon);
		gsDeleteSkeleton(Mesh_Dragon);
		gsDeleteSkeleton(Mesh_Skeleton);
		gsDeleteSkeleton(Mesh_Witch);
		// �A�j���[�V�����̍폜
		gsDeleteAnimation(Mesh_Player);
		gsDeleteAnimation(Mesh_TitlePlayer);
		gsDeleteAnimation(Mesh_TitleDragon);
		gsDeleteAnimation(Mesh_Dragon);
		gsDeleteAnimation(Mesh_Skeleton);
		gsDeleteAnimation(Mesh_Witch);
		// �I�N�g���[�̍폜
		gsDeleteOctree(Octree_Stage);
		gsDeleteOctree(Octree_Collider);
		gsDeleteOctree(Octree_TitelStage);
		gsDeleteOctree(Octree_TitelCollider);
		//SE�̍폜
		gsDeleteSE(Se_GameClear);
		gsDeleteSE(Se_GameStart);
		gsDeleteSE(Se_PlayerAttack);
		gsDeleteSE(Se_PlayerBlock);
		gsDeleteSE(Se_PlayerDamage);
		gsDeleteSE(Se_PlayerRun);
		gsDeleteSE(Se_PlayerDrop);
		gsDeleteSE(Se_EnemyDamage);
		gsDeleteSE(Se_DragonAttack1);
		gsDeleteSE(Se_DragonAttack2);
		gsDeleteSE(Se_DragonDamage);
		gsDeleteSE(Se_DragonDetate);
		gsDeleteSE(Se_DragonFire);
		gsDeleteSE(Se_DragonFoot);
		gsDeleteSE(Se_DragonLanding);
		gsDeleteSE(Se_DragonSpitFire);
		gsDeleteSE(Se_DragonExplosion);
		gsDeleteSE(Se_SkeletonAttack);
		gsDeleteSE(Se_SkeletonDamage);
		gsDeleteSE(Se_SkeletonDetate);
		gsDeleteSE(Se_SkeletonGeneration);
		gsDeleteSE(Se_WitchAttack);
		gsDeleteSE(Se_WitchDamage);
		gsDeleteSE(Se_WitchDetate);
		gsDeleteSE(Se_WitchThunder);
		gsDeleteSE(Se_SpitFire);
		// ���C�g�}�b�v�̍폜
		gsDeleteLightmap(0);
		// ���t���N�V�����v���[�u�̍폜
		gsDeleteReflectionProbe(0);
		// �⏕���C�g�̍폜
		gsDeleteAuxLight(0);
		//�G�t�F�N�폜
		gsDeleteEffect(Effect_Blood);
		gsDeleteEffect(Effect_Explosion);
		gsDeleteEffect(Effect_FireBall);
		gsDeleteEffect(Effect_Smoke);
		gsDeleteEffect(Effect_TorchFlame);
		gsDeleteEffect(Effect_AppearanceBlood);
		gsDeleteEffect(Effect_Attack);
		gsDeleteEffect(Effect_Attack1);
		gsDeleteEffect(Effect_Thunder);
		gsDeleteEffect(Effect_Spark);

		//BGM�폜
		gsDeleteBGM(Sound_Titel);
		gsDeleteBGM(Sound_Wind);

	}

	bool is_running()override 
	{
		//�G�X�P�[�v�L�[��������Ă�����I��
		if (gsGetKeyState(GKEY_ESCAPE))
		{
			return false;
		}
		return true;
	}
private:
	// �V�[���}�l�[�W���[
	SceneManager scene_manager_;
};

int main()
{
	return MyGame().run();
}


