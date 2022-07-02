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

	// �|�X�g�G�t�F�N�g�̃p�����[�^
static GScolor color_{ 1.0f, 1.0f, 1.0f, 1.0f };
static float   saturation_{ 1.0f };
static float   luminance_{ 1.4f };
static float   exposure_{ 0.8f };

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
	gsSetVolumeBGM(0.5f);
	gsPlayBGM(Sound_Wind);
	// �V���h�E�}�b�v�̍쐬�i�Q���̃J�X�P�[�h�V���h�E�}�b�v�j
	static const GSuint shadow_map_size[] = { 2024, 1024 };
	gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
	// �V���h�E�}�b�v�̌����p�����[�^
	gsSetShadowMapAttenuation(0.6f);
	// �V���h�E�}�b�v�̋�����ݒ�
	gsSetShadowMapDistance(60.0f);

	// �G�t�F�N�g�̓ǂݍ��݁i�����̉��j
	gsLoadEffect(Effect_TorchFlame, "Assets/Effect/Fire/Fire.efk");
	gsLoadEffect(Effect_Blood, "Assets/Effect/Blood.efk");
	gsLoadEffect(Effect_HitSpark, "Assets/Effect/HitSpark1.efk");
	gsLoadEffect(Effect_Explosion, "Assets/Effect/Explosion.efk");
	gsLoadEffect(Effect_FireBall, "Assets/Effect/Ball.efk");
	gsLoadEffect(Effect_Smoke, "Assets/Effect/Smoke.efk");



	// �v���[���[�̒ǉ�
	world_->add_actor(std::make_shared<Player>(world_, GSvector3{ -5,4.0,10 }));
	world_->add_actor(std::make_shared<Dragon>(world_, GSvector3{ -5,4.0,50 }));
	// �t�B�[���h�N���X�̒ǉ�
	world_->add_field(std::make_shared<Field>(Octree_Stage, Octree_Collider));
	// �J�����N���X�̒ǉ�
	world_->add_camera(std::make_shared<CameraTPS>(
		world_, GSvector3{ 0.0f, 3.2f, -4.8f }, GSvector3{ 0.0f, 1.0, 0.0f }));
	// ���C�g�N���X�̒ǉ�
	world_->add_light(std::make_shared<Light>(world_));
	//�X�e�[�W�f�[�^�ǂݍ���
	enemy_generator_ = std::make_shared<EnemyGenerator>(world_, "Assets/StageData/StatgeData.csv", "Assets/StageData/FasePos.csv");

	// ���̃G�t�F�N�g���Đ�����(0�`8�������p���C�g�j
	for (GSint i = 0; i < 17; ++i) {
		// �⏕���C�g�̈ʒu���擾
		GSvector3 position;
		gsGetAuxLightPosition(Effect_TorchFlame, i, &position);
		// �⏕���C�g�̈ʒu�ɉ��̃G�t�F�N�g���o��������
		gsPlayEffect(Effect_TorchFlame, &position);
	}


}

// �X�V
void GamePlayScene::update(float delta_time) {
	// ���[���h�̍X�V
	world_->update(delta_time);
	//�W�F�l���[�^�̍X�V
	//enemy_generator_->update(delta_time);
}

// �`��
void GamePlayScene::draw() const {
	// �����_�[�^�[�Q�b�g��L���ɂ���
	gsBeginRenderTarget(0);
	// �����_�[�^�[�Q�b�g�̃N���A
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	// ���[���h�̕`��
	world_->draw();
	//GUI�`��
	world_->draw_gui();
	//fog�`��
	fog_.draw();
	//�����_�[�^�[�Q�b�g�𖳌��ɂ���
	gsEndRenderTarget();

	//�V�F�[�_�[��L���ɂ���
	GScolor col = color_ * luminance_;
	col.a = 1.0f;
	gsBeginShader(0);
	// �ʓx�̐ݒ�
	gsSetShaderParam1f("u_Saturation", saturation_);
	// �e�N�X�`���̐ݒ�
	gsSetShaderParamTexture("u_RenderTexture", 0);
	// �F����
	gsSetShaderParam4f("u_Color", &col);
	// �I�o
	gsSetShaderParam1f("u_Exposure", exposure_);
	// �����_�[�^�[�Q�b�g�p�̃e�N�X�`�����o�C���h
	gsBindRenderTargetTextureEx(0, 0, 0);
	// �����_�[�^�[�Q�b�g��`��
	gsDrawRenderTargetEx(0);
	// �����_�[�^�[�Q�b�g�p�̃e�N�X�`���̉���
	gsUnbindRenderTargetTextureEx(0, 0, 0);
	// �V�F�[�_�[�𖳌��ɂ���
	gsEndShader();
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
	// �Đ����̑S�G�t�F�N�g���~�i�폜�j����
	gsStopAllEffects();
	//BGM �폜
	gsDeleteBGM(Sound_Wind);
	//SE�폜
	gsDeleteSE(Se_GameStart);
}
