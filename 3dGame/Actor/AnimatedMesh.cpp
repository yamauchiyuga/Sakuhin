#include<cmath>
#include "AnimatedMesh.h"

// ��ԃt���[����
const GSfloat LerpTime{ 10.0f };

// �R���X�g���N�^
AnimatedMesh::AnimatedMesh(GSuint mesh, GSuint skeleton, GSuint animation, GSuint motion, bool loop) :
    mesh_{ mesh },
    skeleton_{ skeleton },
    animation_{ animation },
    motion_{ motion },
    motion_timer_{ 0.0f },
    motion_loop_{ loop },
    prev_motion_{ motion },
    prev_motion_timer_{ 0.0f },
    lerp_timer_{ 0.0f } {
}

// �X�V
void AnimatedMesh::update(float delta_time) {
    // �A�j���[�V�����^�C�}�̍X�V
    motion_timer_ += delta_time;
    // ���[�v�A�j���[�V�������H
    if (motion_loop_) {
        // ���[�V�����^�C�}�����[�v������
        motion_timer_ = std::fmod(motion_timer_, motion_end_time());
    }
    else {
        // ���[�V�����^�C�}���N�����v����
        motion_timer_ = std::min(motion_timer_, motion_end_time() - 1.0f);
    }
    // ��ԃ^�C�}�̍X�V(LerpTime�ȏ�ɂȂ�Ȃ��悤�ɃN�����v����j
    lerp_timer_ = std::min(lerp_timer_ + delta_time, LerpTime);
}

// �`��
void AnimatedMesh::draw() const {
    // �X�P���g���̎����v�Z�𖳌��ɂ���
    gsDisable(GS_CALC_SKELETON);
    // �X�P���g���̃o�C���h
    gsBindSkeleton(skeleton_);
    // �{�[���̃��[���h�ϊ��s���ݒ�
    gsSetMatrixSkeleton(world_bone_matrices_.data());
    // ���b�V���̕`��
    gsDrawMesh(mesh_);
    // �X�P���g���̎����v�Z��L���ɂ���
    gsEnable(GS_CALC_SKELETON);
}

// ���[�V�����̕ύX
void AnimatedMesh::change_motion(GSuint motion, bool loop) {
    // ���݂Ɠ������[�V�����̏ꍇ�͉������Ȃ�
    if (motion_ == motion) return;
    // ��Ԓ��i�O���j�́A�O��̃��[�V�������X�V�����Ȃ��悤�ɂ���
    if (lerp_timer_ > (LerpTime * 0.5f)) {
        // �O��̃��[�V�����ԍ��ƃ��[�V�������Ԃ�ۑ�
        prev_motion_ = motion_;
        prev_motion_timer_ = motion_timer_;
        // ��Ԓ��^�C�}�̏����� 
        lerp_timer_ = 0.0f;
    }
    // ���[�V�����̍X�V
    motion_ = motion;
    // ���[�V�����^�C�}�̏�����
    motion_timer_ = 0.0f;
    // ���[�v�t���O�̐ݒ�
    motion_loop_ = loop;
}

// ���W�ϊ����s��
void AnimatedMesh::transform(const GSmatrix4& matrix) {
    // �{�[�����Ƃ̃��[�J���ϊ��s����v�Z
    gsCalculateAnimationLerp(
        animation_, prev_motion_, prev_motion_timer_,
        animation_, motion_, motion_timer_,
        lerp_timer_ / LerpTime,
        local_bone_matrices_.data()
    );
    // �{�[�����Ƃ̃��[���h�ϊ��s����v�Z
    gsBindSkeleton(skeleton_);
    gsCalculateSkeleton(&matrix, local_bone_matrices_.data(), world_bone_matrices_.data());
}

// ���[�V�����̏I�����Ԃ��擾
float AnimatedMesh::motion_end_time() const {
    return gsGetEndAnimationTime(animation_, motion_);
}

// �{�[���̕ϊ��s����擾
const GSmatrix4& AnimatedMesh::bone_matrices(int bone_no) const {
    return world_bone_matrices_[bone_no];
}

