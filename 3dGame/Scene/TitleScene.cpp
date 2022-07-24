#include "TitleScene.h"
#include"../Input.h"
#include"../Assets.h"
#include"../Tween/Tween.h"
#include"../Collision/Field.h"
#include"../Actor/Light.h"
#include"../Actor/Enemy/TitleDragon.h"
#include"../Actor/Player/TitlePlayer.h"
#include"../Camera/TitleCamera.h"
#include<GSeffect.h>
#include <GSstandard_shader.h>


// �J�n
void TitleScene::start()
{
	//�����_�[�^�[�Q�b�g�̍쐬
	gsCreateRenderTarget(0, 1280, 720, GS_TRUE, GS_TRUE, GS_TRUE);
	//������J�����O��L���ɂ���
	gsEnable(GS_FRUSTUM_CULLING);

	// �I���t���O�̏�����
	is_end_ = false;
	//�t�F�[�h�t���O��������
	is_fade_ = true;
	//�t�F�[�h�̃A���t�@�l��������
	alpha_ = 1.0f;
	
	gsPlayBGM(Sound_Titel);
	gsSetVolumeBGM(0.4f);
	//40�t���[�������ăt�F�[�h�C��
	Tween::value(1.0f, 0.0f, 120.0f, [=](GSfloat val) {alpha_ = val; })
		.on_complete([=] {is_fade_ = false; });

	//�V���h�E�}�b�v�̍쐬�i2���̃J�X�P�[�h�V���h�E�j
	static const GSuint shadow_map_size[] = { 2048, 1024 };
	gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
	//�V���h�E�}�b�v��K�p���鋗��
	gsSetShadowMapDistance(80.0f);
	//�V���h�E�}�b�v�o�C�A�X
	gsSetShadowMapBias(0.0f);
	// ���C�g�}�b�v�̓ǂݍ���(0�Ԃɓǂݍ��߂Ύ����I�ɓK�p�����j
	gsLoadLightmap(0, "Assets/Lightmap/Lightmap.txt");
	// �⏕���C�g�̓ǂݍ���
	gsLoadAuxLight(0, "Assets/AuxLight/TitelAuxLight.txt");


	world_ = std::make_shared<World>();
	world_->add_actor(std::make_shared<TitleDragon>(world_, GSvector3{ 0.0f,3.0f,-5.0f }));
	world_->add_actor(std::make_shared<TitlePlayer>(world_, GSvector3{ 0.0f,0.0f,10.0f }));

	// �t�B�[���h�N���X�̒ǉ�
	world_->add_field(std::make_shared<Field>(Octree_TitelStage, Octree_TitelCollider));
	world_->add_camera(std::make_shared<TitleCamera>(world_, GSvector3{ 0.0f, 1.0f, 17.0f }, GSvector3{ 0.0f, 6.5, 0.0f }));
	// ���C�g�N���X�̒ǉ�
	world_->add_light(std::make_shared<Light>(world_));

	for (GSint i = 0; i < 8; ++i) {
		// �⏕���C�g�̈ʒu���擾
		GSvector3 position;
		gsGetAuxLightPosition(0, i, &position);
		// �⏕���C�g�̈ʒu�ɉ��̃G�t�F�N�g���o��������
		gsPlayEffect(0, &position);
	}
}

// �X�V
void TitleScene::update(float delta_time)
{
	world_->update(delta_time);
	if (is_fade_)return;
	// �G���^�[�L�[����������V�[���I��
	if (Input::is_a_push()) {
		gsPlaySE(Se_GameStart);
		is_end_ = true;     // �V�[���I��
	}
}

// �`��
void TitleScene::draw() const
{
	//�����_�[�^�[�Q�b�g��L���ɂ���
	gsBeginRenderTarget(0);
	//�����_�[�^�[�Q�b�g�̃N���A
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	world_->draw();
	fog.draw();
	static const GSvector2 Pos{ 100.0f,-50.0f };
	gsDrawSprite2D(Texture_TitelName, &Pos, NULL, NULL, NULL, NULL, 0.0f);
	draw_fade();
	//�����_�[�^�[�Q�b�g�𖳌��ɂ���
	gsEndRenderTarget();

	// �|�X�g�G�t�F�N�g�̃p�����[�^
	static GScolor color_{ 1.0f, 1.0f, 1.0f, 1.0f };
	static float   saturation_{ 0.8f };
	static float   luminance_{ 1.0f };
	static float   exposure_{ 0.8f };

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
bool TitleScene::is_end() const {
	return is_end_;         // �I���t���O��Ԃ�
}

// ���̃V�[������Ԃ�
std::string TitleScene::next() const {
	return "GamePlayScene"; // ���̃V�[������Ԃ�
}

// �I��
void TitleScene::end() {
	// ���[���h������
	world_->clear();
	//Tween�̍폜
	Tween::clear();
	//BGM�̍폜
	gsStopBGM();
	//
	gsDeleteAuxLight(0);
	//
	gsDeleteLightmap(0);
}

//�t�F�[�h����
void TitleScene::draw_fade()const {
	GScolor Color{ 1.0f, 1.0f, 1.0f, alpha_ };
	gsDrawSprite2D(Texture_Fade, NULL, NULL, NULL, &Color, NULL, 0.0f);
}

