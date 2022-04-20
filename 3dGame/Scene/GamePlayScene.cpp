#include "GamePlayScene.h"
#include"../Collision/Field.h"
#include "../Collision/Field.h"
#include "../Camera/CameraTPS.h"
#include "../Actor/Light.h"
#include"../Actor/Player/Player.h"
#include"../Asset.h"


void GamePlayScene::start() {
    // �I���t���O��������
    is_end_ = false;

    //
    gsLoadMesh(Mesh_Player, "Assets/model/Kendo/kendo.msh");
    gsLoadSkeleton(Mesh_Player, "Assets/model/Kendo/kendo.skl");
    gsLoadAnimation(Mesh_Player, "Assets/model/Kendo/kendo.anm");

    // �X�J�C�{�b�N�X���b�V���̓ǂݍ���
    gsLoadMesh(Mesh_Skybox, "Assets/Skybox/skydome.msh");
    // �`��p�I�N�c���[�̓ǂݍ���
    gsLoadOctree(Octree_Stage, "Assets/Octree/stage.oct");
    // �Փ˔���p�I�N�c���[�̓ǂݍ���
    gsLoadOctree(Octree_Collider, "Assets/Octree/stage_collider.oct");

    // �t�B�[���h�N���X�̒ǉ�
    world_.add_field(new Field{ Octree_Stage, Octree_Collider, Mesh_Skybox });
    // �J�����N���X�̒ǉ�
    world_.add_camera(new CameraTPS{
              &world_, GSvector3{0.0f, 3.2f, -4.8f}, GSvector3{0.0f, 1.92, 0.0f} });
    // ���C�g�N���X�̒ǉ�
    world_.add_light(new Light{ &world_ });

    // �v���[���[�̒ǉ�
    world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.125f, 0.0f } });

}

// �X�V
void GamePlayScene::update(float delta_time) {
    // �G���^�[�L�[�ŃV�[���I���Ƃ��Ă���
    if (gsGetKeyTrigger(GKEY_RETURN)) {
        is_end_ = true;
    }
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
    return is_end_;         // �I���t���O��Ԃ�
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
    gsDeleteSkeleton(Mesh_Player);
    gsDeleteAnimation(Mesh_Player);
    gsDeleteMesh(Mesh_Skybox);
    // �I�N�g���[�̍폜
    gsDeleteOctree(Octree_Stage);
    gsDeleteOctree(Octree_Collider);
}
