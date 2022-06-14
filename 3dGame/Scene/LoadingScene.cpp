#include "LoadingScene.h"
#include <GSstandard_shader.h>
#include "../Assets.h"
#include <GSmusic.h>
#include <gslib.h>
#include <GSgame.h>
#include <cmath>

void LoadingScene::start()
{

    is_end_ = false;
	// ���[�h�p�X���b�h�̎��s
	gslib::Game::run_thread(load_assets, this);
}

void LoadingScene::update(float delta_time)
{

}

void LoadingScene::draw() const
{
	gsFontParameter(0, 50, "�l�r �S�V�b�N");
	gsTextPos(80, 215);
	gsDrawText("Loading�V�[���i���j");
	gsFontParameter(0, 16, "�l�r �S�V�b�N");
}

bool LoadingScene::is_end() const
{
    return is_end_;
}

std::string LoadingScene::next() const
{
    return "TitleScene";
}

void LoadingScene::end(){
}

void LoadingScene::load_assets(void* self) {

	LoadingScene* load = (LoadingScene*)self;
	gsInitDefaultShader();
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


	//�v���C���[�̓ǂݍ���
	gsLoadMesh(Mesh_Player, "Assets/model/Player/Player.mshb");
	gsLoadSkeleton(Mesh_Player, "Assets/model/Player/Player.sklb");
	gsLoadAnimation(Mesh_Player, "Assets/model/Player/Player.anmb");

	//�h���S���ǂݍ���
	gsLoadMesh(Mesh_Dragon, "Assets/model/Enemy/Dragon/Dragon.mshb");
	gsLoadSkeleton(Mesh_Dragon, "Assets/model/Enemy/Dragon/Dragon.sklb");
	gsLoadAnimation(Mesh_Dragon, "Assets/model/Enemy/Dragon/Dragon.anmb");
	//
	gsLoadMesh(Mesh_Skeleton, "Assets/model/Enemy/Skeleton.mshb");
	gsLoadSkeleton(Mesh_Skeleton, "Assets/model/Enemy/Skeleton/Skeleton.sklb");
	gsLoadAnimation(Mesh_Skeleton, "Assets/model/Enemy/Skeleton/Skeleton.anmb");
	//
	gsLoadMesh(Mesh_Witch, "Assets/model/Enemy/Witch/Witch.mshb");
	gsLoadSkeleton(Mesh_Witch, "Assets/model/Enemy/Witch/Witch.sklb");
	gsLoadAnimation(Mesh_Witch, "Assets/model/Enemy/Witch/Witch.anmb");

	gsLoadTexture(Texture_Frame, "Assets/Texture/GaugeFrame.png");

	// �X�J�C�{�b�N�X���b�V���̓ǂݍ���
	gsLoadMesh(Mesh_Skybox, "Assets/Skybox/skydome.msh");
	// �`��p�I�N�c���[�̓ǂݍ���
	gsLoadOctree(Octree_Stage, "Assets/TtestFiled/Field.oct");
	// �Փ˔���p�I�N�c���[�̓ǂݍ���
	gsLoadOctree(Octree_Collider, "Assets/TtestFiled/Field_collider.oct");

	load->is_end_ = true;

}