#include "GamePlayScene.h"
#include"../Collision/Field.h"
#include "../Collision/Field.h"
#include "../Camera/CameraTPS.h"
#include "../Actor/Light.h"
#include"../Actor/Player/Player.h"
#include"../Asset.h"
#include"../Actor/Enemy/Dragon.h"

#include <GSstandard_shader.h>

void GamePlayScene::start() {

	

	// �I���t���O��������
	is_end_ = false;
	// ������J�����O��L���ɂ���
	gsEnable(GS_FRUSTUM_CULLING);
	// �f�t�H���g�V�F�[�_�[��L���ɂ���
	gsEnable(GS_DEFAULT_SHADER_MODE);

	// ���b�V���p�̃V�F�[�_�[
	gsLoadShader(Shader_StandardMesh, "Assets/shader/StandardMeshBump.vert", "Assets/shader/StandardMeshBump.frag");
	// �X�L�j���O���b�V���p�̃V�F�[�_�[
	gsLoadShader(Shader_StandardSkinnedMesh, "Assets/shader/StandardSkinnedMeshBump.vert", "Assets/shader/StandardMeshBump.frag");
	// �I�N�c���[�p�̃V�F�[�_�[
	gsLoadShader(Shader_StandardOctree, "Assets/shader/StandardOctreeLightmap.vert", "Assets/shader/StandardOctreeLightmap.frag");
	// �V���h�E�}�b�v�p�̃��b�V���V�F�[�_�[
	gsLoadShader(Shader_ShadowMapMesh, "Assets/shader/ShadowMap.vert", "Assets/shader/ShadowMap.frag");
	// �V���h�E�}�b�v�p�̃X�L�j���O���b�V���V�F�[�_�[
	gsLoadShader(Shader_ShadowMapSkinnedMesh, "Assets/shader/ShadowMapSkinned.vert", "Assets/shader/ShadowMap.frag");

	// ���t���N�V�����v���[�u�̓ǂݍ���(0�Ԃɓǂݍ��߂Ύ����I�ɓK�p�����j
	gsLoadReflectionProbe(0, "Assets/RefProbe/ReflectionProbe.txt");
	// ���C�g�}�b�v�̓ǂݍ���(0�Ԃɓǂݍ��߂Ύ����I�ɓK�p�����j
	gsLoadLightmap(0, "Assets/Lightmap/Lightmap.txt");

	// �V���h�E�}�b�v�̍쐬�i�Q���̃J�X�P�[�h�V���h�E�}�b�v�j
	static const GSuint shadow_map_size[] = { 2024, 1024 };
	gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
	// �V���h�E�}�b�v��K�p���鋗��
	gsSetShadowMapDistance(80.0f);
	// �V���h�E�}�b�v�o�C�A�X
	gsSetShadowMapBias(0.0f);




	//�v���C���[�̓ǂݍ���
	gsLoadMesh(Mesh_Player, "Assets/model/Player/Player.mshb");
	gsLoadSkeleton(Mesh_Player, "Assets/model/Player/Player.sklb");
	gsLoadAnimation(Mesh_Player, "Assets/model/Player/Player.anmb");

	gsLoadMesh(Mesh_Dragon, "Assets/model/Enemy/Dragon.mshb");
	gsLoadSkeleton(Mesh_Dragon, "Assets/model/Enemy/Dragon.sklb");
	gsLoadAnimation(Mesh_Dragon, "Assets/model/Enemy/Dragon.anmb");

	gsLoadTexture(Texture_HP, "Assets/Texture/HPGauge.png");
	gsLoadTexture(Texture_Frame, "Assets/Texture/GaugeFrame.png");

	// �X�J�C�{�b�N�X���b�V���̓ǂݍ���
	gsLoadMesh(Mesh_Skybox, "Assets/Skybox/skydome.msh");
	// �`��p�I�N�c���[�̓ǂݍ���
	gsLoadOctree(Octree_Stage, "Assets/TtestFiled/Field.oct");
	// �Փ˔���p�I�N�c���[�̓ǂݍ���
	gsLoadOctree(Octree_Collider, "Assets/TtestFiled/Field_collider.oct");

	// �t�B�[���h�N���X�̒ǉ�
	world_.add_field(new Field{ Octree_Stage, Octree_Collider, Mesh_Skybox });
	// �J�����N���X�̒ǉ�
	world_.add_camera(new CameraTPS{
			  &world_, GSvector3{0.0f, 3.2f, -4.8f}, GSvector3{0.0f, 1.0, 0.0f} });
	// ���C�g�N���X�̒ǉ�
	world_.add_light(new Light{ &world_ });

	// �v���[���[�̒ǉ�
	world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.125f, 0.0f } });
	//
	world_.add_actor(new Dragon{ &world_,GSvector3{0.0f,0.125,30.0f} });
}

// �X�V
void GamePlayScene::update(float delta_time) {
	// ���[���h�̍X�V
	world_.update(delta_time);
}

// �`��
void GamePlayScene::draw() const {
	// ���[���h�̕`��
	world_.draw();
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
	// ���b�V���̍폜
	gsDeleteMesh(Mesh_Player);
	gsDeleteMesh(Mesh_Dragon);
	gsDeleteMesh(Mesh_Skybox);
	// �X�P���g���̍폜
	gsDeleteSkeleton(Mesh_Player);
	gsDeleteSkeleton(Mesh_Dragon);
	// �A�j���[�V�����̍폜
	gsDeleteAnimation(Mesh_Player);
	gsDeleteAnimation(Mesh_Dragon);
	// �I�N�g���[�̍폜
	gsDeleteOctree(Octree_Stage);
	gsDeleteOctree(Octree_Collider);
}
