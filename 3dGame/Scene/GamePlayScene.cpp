#include "GamePlayScene.h"
#include"../Collision/Field.h"
#include "../Collision/Field.h"
#include "../Camera/CameraTPS.h"
#include "../Actor/Light.h"
#include"../Actor/Player/Player.h"
#include"../Assets.h"
#include"../Actor/Enemy/Skeleton.h"
#include"../Actor/Enemy/Dragon.h"
#include<imgui/imgui.h>
#include <GSstandard_shader.h>
#include<GSeffect.h>

// �W���V�F�[�_�[�̐ݒ�
#define GS_ENABLE_AUX_LIGHT                 // �⏕���C�g��L���ɂ���
#define GS_ENABLE_BAKED_LIGHTMAP_SHADOW     // �x�C�N�������C�g�}�b�v�ɉe�𗎂Ƃ�
#define GS_ENABLE_SOFT_SHADOW               // �\�t�g�V���h�E�i�e�̗֊s���ڂ����j
#define GS_ENABLE_RIM_LIGHT                 // �������C�g��L���ɂ���

void GamePlayScene::start() {
	// �I���t���O��������
	is_end_ = false;
	// �G�t�F�N�g�̏�����
	gsInitEffect();
	// ������J�����O��L���ɂ���
	gsCreateRenderTarget(0, 1280, 720, GS_TRUE, GS_TRUE, GS_TRUE);
	//������J�����O��L���ɂ���
	gsEnable(GS_FRUSTUM_CULLING);

	world_ = std::make_shared<World>();

	// �V���h�E�}�b�v�̍쐬�i�Q���̃J�X�P�[�h�V���h�E�}�b�v�j
	static const GSuint shadow_map_size[] = { 2024, 1024 };
	gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
	// �V���h�E�}�b�v�̌����p�����[�^
	gsSetShadowMapAttenuation(0.6f);
	// �V���h�E�}�b�v�̋�����ݒ�
	gsSetShadowMapDistance(60.0f);

	// �G�t�F�N�g�̓ǂݍ��݁i�����̉��j
	gsLoadEffect(0, "Assets/Effect/Fire/Fire.efk");
	// �v���[���[�̒ǉ�
	world_->add_actor( std::make_shared<Player>( world_, GSvector3{-5,4.0,10} ));
	// �t�B�[���h�N���X�̒ǉ�
	world_->add_field(std::make_shared<Field>( Octree_Stage, Octree_Collider, Mesh_Skybox ));
	// �J�����N���X�̒ǉ�
	world_->add_camera(std::make_shared<CameraTPS>(
			  world_, GSvector3{0.0f, 3.2f, -4.8f}, GSvector3{0.0f, 1.0, 0.0f} ));
	// ���C�g�N���X�̒ǉ�
	world_->add_light(std::make_shared<Light>( world_ ));
	//
	enemy_generator_ = std::make_shared<EnemyGenerator>(world_, "Assets/StageData/StatgeData.csv", "Assets/StageData/FasePos.csv");
	

	// ���̃G�t�F�N�g���Đ�����(0�`8�������p���C�g�j
	for (GSint i = 0; i < 17; ++i) {
		// �⏕���C�g�̈ʒu���擾
		GSvector3 position;
		gsGetAuxLightPosition(0, i, &position);
		// �⏕���C�g�̈ʒu�ɉ��̃G�t�F�N�g���o��������
		gsPlayEffect(0, &position);
	}

}

// �X�V
void GamePlayScene::update(float delta_time) {
	// ���[���h�̍X�V
	world_->update(delta_time);
	fog_.update(delta_time);
	enemy_generator_->update(delta_time);

}

// �`��
void GamePlayScene::draw() const {

	// ���[���h�̕`��
	world_->draw();
	//
	world_->draw_gui();
	//
	fog_.draw();
	
}

// �I�����Ă��邩�H
bool GamePlayScene::is_end() const {
	return world_->is_game_over();         // �I���t���O��Ԃ�
}

// ���̃V�[����Ԃ�
std::string GamePlayScene::next() const {
	return "TitleScene";    // �^�C�g���V�[���ɖ߂�
}

// �I��
void GamePlayScene::end() {
	// ���[���h������
	world_->clear();
}
