#ifndef ASSETS_H_
#define ASSETS_H_

//���b�V���f�[�^
enum {
    Mesh_Player,
    Mesh_Dragon,
    Mesh_Skeleton,
    Mesh_Witch,
    Mesh_Skybox
};

//�I�N�g�[
enum {       
    Octree_Stage,
    Octree_Collider
};

//�e�N�X�`���f�[�^
enum {
    Texture_HP,   
    Texture_Frame,
    Texture_ST,

};

enum {
    Sound_TitleBGM,
    Sound_PlayingBGM,
    Sound_ResultBGM
};

//SE
enum {
    Se_GameStart,
    Se_GameCler,
    Se_GameOver,
    Se_PlayerAttack,        
    Se_PlayerDamage, 
    Se_PlayerRun,
    Se_PlayerBlock,
    Se_DragonAttack1,
    Se_DragonAttack2,
    Se_DragonDeath,
    Se_DragonFire,
    Se_DragonFoot,
    Se_DragonSpitFire,
    Se_Skeleton_Attack,
    Se_WitchAttack
};

// �V�F�[�_�[
enum {
    Shader_StandardMesh,            // ���b�V���p�V�F�[�_�[
    Shader_StandardSkinnedMesh,     // �X�L�j���O���b�V���p�V�F�[�_�[
    Shader_StandardOctree,          // �I�N�c���[�p�V�F�[�_�[
    Shader_ShadowMapMesh,           // ���b�V���p�V���h�E�}�b�v�V�F�[�_�[
    Shader_ShadowMapSkinnedMesh     // �X�L�j���O���b�V���p�V���h�E�}�b�v�V�F�[�_�[
};
#endif // !ASSETS_H_
