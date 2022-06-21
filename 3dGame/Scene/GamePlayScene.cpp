#include "GamePlayScene.h"
#include"../Collision/Field.h"
#include "../Collision/Field.h"
#include "../Camera/CameraTPS.h"
#include "../Actor/Light.h"
#include"../Actor/Player/Player.h"
#include"../Assets.h"
#include"../Actor/Enemy/Skeleton.h"
#include"../Actor/Enemy/Dragon.h"
#include <GSstandard_shader.h>

void GamePlayScene::start() {
	// �I���t���O��������
	is_end_ = false;
	// ������J�����O��L���ɂ���
	gsEnable(GS_FRUSTUM_CULLING);

	// ���t���N�V�����v���[�u�̓ǂݍ���(0�Ԃɓǂݍ��߂Ύ����I�ɓK�p�����j
	gsLoadReflectionProbe(0, "Assets/RefProbe/ReflectionProbe.txt");
	// ���C�g�}�b�v�̓ǂݍ���(0�Ԃɓǂݍ��߂Ύ����I�ɓK�p�����j
	gsLoadLightmap(0, "Assets/Lightmap/Lightmap.txt");
	// �⏕���C�g�̓ǂݍ���
	gsLoadAuxLight(0, "Assets/AuxLight/AuxLight.txt");

	// �V���h�E�}�b�v�̍쐬�i�Q���̃J�X�P�[�h�V���h�E�}�b�v�j
	static const GSuint shadow_map_size[] = { 2024, 1024 };
	gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
	// �V���h�E�}�b�v��K�p���鋗��
	gsSetShadowMapDistance(80.0f);
	// �V���h�E�}�b�v�o�C�A�X
	gsSetShadowMapBias(0.0f);

	// �v���[���[�̒ǉ�
	world_.add_actor(new Player{ &world_, GSvector3{ -7.0f, 4.0f, -100.0f } });
	// �t�B�[���h�N���X�̒ǉ�
	world_.add_field(new Field{ Octree_Stage, Octree_Collider, Mesh_Skybox });
	// �J�����N���X�̒ǉ�
	world_.add_camera(new CameraTPS{
			  &world_, GSvector3{0.0f, 3.2f, -4.8f}, GSvector3{0.0f, 1.0, 0.0f} });
	// ���C�g�N���X�̒ǉ�
	world_.add_light(new Light{ &world_ });
	//
	//enemy_generator_= std::make_shared<EnemyGenerator>(&world_,"Assets/StageData/StatgeData.csv","Assets/StageData/FasePos.csv" );

	//world_.add_actor(new Dragon{ &world_,GSvector3{-7.0f,4.0f,-116.0f} });
	world_.add_actor(new Skeketon{ &world_,GSvector3{-7.0f,4.0f,-120.0f} });
	//world_.add_actor(new Skeketon{ &world_,GSvector3{-7.0f,4.0f,-110.0f} });


}

// �X�V
void GamePlayScene::update(float delta_time) {
	// ���[���h�̍X�V
	world_.update(delta_time);
	//enemy_generator_->update(delta_time);
}

// �`��
void GamePlayScene::draw() const {
	// ���[���h�̕`��
	world_.draw();
	gsDrawText(std::to_string(world_.count_actor()).c_str());
}

// �I�����Ă��邩�H
bool GamePlayScene::is_end() const {
	return world_.is_game_over();         // �I���t���O��Ԃ�
}

// ���̃V�[����Ԃ�
std::string GamePlayScene::next() const {
	return "TitleScene";    // �^�C�g���V�[���ɖ߂�
}

// �I��
void GamePlayScene::end() {
	// ���[���h������
	world_.clear();
}
