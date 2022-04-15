#ifndef ASSETS_H_
#define ASSETS_H_

//���b�V���f�[�^
enum {
    Mesh_Player,					//���@
    Mesh_Enemy,                     // �G
    Mesh_Skybox                     // �X�J�C�{�b�N�X
};

//�I�N�g�[
enum {
    Octree_Koutei,                  //�Z��
    Octree_KouteiCollider           //�Z��Փ˔���
};

//�e�N�X�`���f�[�^
enum {
    Texture_Title,          // �^�C�g��
    Texture_Kendo,          // ��������
    Texture_Karate,         // ��蕔��
    Texture_Start,          // �J�n���b�Z�[�W
    Texture_Number,         // �����t�H���g
    Texture_Text,           // �e�L�X�g�摜
    Texture_BlueBack,       // ���U���g�p�w�i
    Texture_Result1,        // ���U���g�p�e�L�X�g1
    Texture_Result2,        // ���U���g�p�e�L�X�g2
    Texture_Radar,          // ���[�_�[�摜
    Texture_RadarPoint,      // ���[�_�[�̓_
    Texturre_GaugeRed,      //HP�\���̐ԕ���
    Texturre_GaugeGreen     //HP�\���̗Ε���

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
#endif // !ASSETS_H_
