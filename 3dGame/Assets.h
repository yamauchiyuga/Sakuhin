#ifndef ASSETS_H_
#define ASSETS_H_

//���b�V���f�[�^
enum {
    Mesh_Player,					//�v���C���[
    Mesh_Dragon,                    // �h���S��
    Mesh_Skeleton,                  //�X�P���g��
    Mesh_Witch,                     //
    Mesh_Skybox                     // �X�J�C�{�b�N�X
};

//�I�N�g�[
enum {
    Octree_Koutei,                  //�Z��
    Octree_KouteiCollider,           //�Z��Փ˔���
    Octree_Stage,
    Octree_Collider
};

//�e�N�X�`���f�[�^
enum {
    Texture_HP,             //HP�\���̐ԕ���
    Texture_Frame,          //
    Texture_ST,

};

enum {
    Sound_TitleBGM,         // �^�C�g���V�[���p�a�f�l 
    Sound_PlayingBGM,       // �Q�[���V�[���p�̂a�f�l 
    Sound_ResultBGM         // ���U���g�p�a�f�l 
};

enum {
    Se_GameStart,           // �Q�[���J�n���̌��ʉ�
    Se_PlayerAttack,        // �������� �U�������ʉ�
    Se_PlayerDamage,        // �������� ������ʉ�
    Se_EnemeyDamage,        // ��蕔���� ������ʉ�
    Se_Timeout              // �^�C���A�E�g�̌��ʉ�   
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
