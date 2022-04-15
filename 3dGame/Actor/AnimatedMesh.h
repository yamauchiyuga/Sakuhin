#ifndef ANIMATED_MESH_H_
#define ANIMATED_MESH_H_

#include <gslib.h>
#include <array>

// �A�j���[�V�����t�����b�V���N���X
class AnimatedMesh {
public:
    // �R���X�g���N�^
    AnimatedMesh(GSuint mesh, GSuint skeleton, GSuint animation, GSuint motion = 0, bool loop = true);
    // �X�V
    void update(float delta_time);
    // �`��
    void draw() const;
    // ���[�V�����̕ύX
    void change_motion(GSuint motion, bool loop = true);
    // �ϊ��s���ݒ肷��
    void transform(const GSmatrix4& matrix);
    // ���[�V�����̏I�����Ԃ��擾
    float motion_end_time() const;
    // �{�[���̕ϊ��s����擾
    const GSmatrix4& bone_matrices(int bone_no) const;

private:
    // ���b�V��
    GSuint		mesh_;
    // �X�P���g��
    GSuint		skeleton_;
    // �A�j���[�V����
    GSuint		animation_;
    // ���[�V�����ԍ�
    GSuint		motion_;
    // ���[�V�����^�C�}
    GSfloat		motion_timer_;
    // ���[�V�������[�v�t���O
    bool		motion_loop_;
    // �O��Đ��������[�V�����ԍ�
    GSuint		prev_motion_;
    // �O��Đ������ŏI�A�j���[�V�����^�C�}
    GSfloat		prev_motion_timer_;
    // ��ԃA�j���[�V�����^�C�}
    GSfloat		lerp_timer_;
    // �{�[���̃��[�J���p�ϊ��s��
    std::array<GSmatrix4, 256>	local_bone_matrices_;
    // �{�[���̃��[���h�ϊ��s��
    std::array<GSmatrix4, 256>	world_bone_matrices_;
};

#endif

