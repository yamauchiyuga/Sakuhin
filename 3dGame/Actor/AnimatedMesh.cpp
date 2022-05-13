#include "AnimatedMesh.h"
#include <algorithm>
#include <cmath>

// ��ԃt���[����
const GSfloat LerpTime{ 10.0f };

// �R���X�g���N�^
AnimatedMesh::AnimatedMesh(GSuint mesh, GSuint skeleton, GSuint animation, GSuint motion, bool loop, GSuint layer_count, GSuint bone_count) :
	mesh_{ mesh },
	skeleton_{ skeleton },
	animation_layers_(layer_count, Animation{ animation, motion, loop }),
	animation_layer_indices_(bone_count, 0),
	local_bone_matrices_(bone_count, GS_MATRIX4_IDENTITY),
	bone_matrices_(bone_count, GS_MATRIX4_IDENTITY),
	transform_{ GS_MATRIX4_IDENTITY } {
}

// �X�V
void AnimatedMesh::update(float delta_time) {
	// �A�j���[�V�������C���[���X�V
	for (auto& layer : animation_layers_) {
		layer.update(delta_time);
	}
}

// �`��
void AnimatedMesh::draw() const {
	// �X�P���g���̎����v�Z�𖳌��ɂ���
	gsDisable(GS_CALC_SKELETON);
	// �X�P���g���̃o�C���h
	gsBindSkeleton(skeleton_);
	// �{�[���̃��[���h�ϊ��s���ݒ�
	gsSetMatrixSkeleton(bone_matrices_.data());

	glPushMatrix();
	// ���W�ϊ��s���ݒ肷��
	glMultMatrixf(transform_);
	// ���b�V���̕`��
	gsDrawMesh(mesh_);
	glPopMatrix();

	// �X�P���g���̎����v�Z��L���ɂ���
	gsEnable(GS_CALC_SKELETON);
}

// ���[�V�����̕ύX�i���C���[�w��Ȃ��j
void AnimatedMesh::change_motion(GSuint motion, bool loop) {
	for (auto& layer : animation_layers_) {
		layer.change_motion(motion, loop);
	}
}

// ���[�V�����̕ύX�i���C���[�w�肠��j
void AnimatedMesh::change_motion(GSuint layer, GSuint motion, bool loop) {
	animation_layers_[layer].change_motion(motion, loop);
}

// ���C���[�C���f�b�N�X�̐ݒ�
void AnimatedMesh::set_layer_indices(GSuint layer_index, const GSuint layer_bones[], GSuint layer_bones_count) {
	for (GSuint i = 0; i < layer_bones_count; ++i) {
		animation_layer_indices_[layer_bones[i]] = layer_index;
	}
}

// ���W�ϊ����s��
void AnimatedMesh::transform(const GSmatrix4& matrix) {
	// ���W�ϊ��s����X�V
	transform_ = matrix;
	// �e���C���[�̂��烍�[�J���ϊ��s����擾
	for (GSuint i = 0; i < gsGetSkeletonNumBones(skeleton_); ++i) {
		local_bone_matrices_[i] = animation_layers_[animation_layer_indices_[i]].local_bone_matrix(i);
	}
	// �{�[���̕ϊ��s����v�Z
	gsBindSkeleton(skeleton_);
	gsCalculateSkeleton(NULL, local_bone_matrices_.data(), bone_matrices_.data());
}

// ���[�V�����̏I�����Ԃ��擾
float AnimatedMesh::motion_end_time(GSuint layer) const {
	return animation_layers_[layer].motion_end_time();
}

// ���[�V�������I�����Ă��邩�H
bool AnimatedMesh::is_motion_end(GSuint layer) const {
	return animation_layers_[layer].is_motion_end();
}

// �{�[���̃��[���h�ϊ��s����擾
GSmatrix4 AnimatedMesh::bone_matrices(int bone_no) const {
	return bone_matrices_[bone_no] * transform_;
}

// ���݂̃��[�V�����̍Đ����Ԃ��擾
float AnimatedMesh::current_motion_time(GSuint layer) const {
	return animation_layers_[layer].current_motion_time();
}

// ���݂̃��[�V�����̍Đ����Ԃ��擾
void AnimatedMesh::current_motion_time(float time, GSuint layer) {
	animation_layers_[layer].current_motion_time(time);
}

//�A�j���[�V�����C�x���g�o�^
void AnimatedMesh::add_animation_event(GSuint motion, float time, std::function<void()> callback) {
	for (auto& layer : animation_layers_) {
		layer.add_animation_event(motion, time, callback);
	}
}

// �R���X�g���N�^
AnimatedMesh::Animation::Animation(GSuint animation, GSuint motion, bool loop) :
	animation_{ animation },
	motion_{ motion },
	motion_timer_{ 0.0f },
	motion_loop_{ loop },
	prev_motion_{ motion },
	prev_motion_timer_{ 0.0f },
	lerp_timer_{ 0.0f } {
}

//�f�X�g���N�^
AnimatedMesh::Animation::~Animation() {
	std::for_each(animation_events_.begin(), animation_events_.end(), [](AnimationEvent* animation_event) {delete animation_event; });
	animation_events_.clear();
}
// �X�V
void AnimatedMesh::Animation::update(float delta_time) {
	//�X�V�O�̃^�C�}�[�l������Ă���
	GSfloat prev_timer = motion_timer_;
	// �A�j���[�V�����^�C�}�̍X�V
	motion_timer_ += delta_time;
	if (motion_loop_) {
		// ���[�V�����^�C�}�����[�v������
		motion_timer_ = std::fmod(motion_timer_, motion_end_time());
	}
	else {
		// ���[�V�����^�C�}���N�����v����
		motion_timer_ = std::min(motion_timer_, motion_end_time() - 1.0f);
	}
	// ��Ԓ��^�C�}�̍X�V
	lerp_timer_ = std::min(lerp_timer_ + delta_time, LerpTime);

	//���[�V���������[�v�������ǂ����B
    // ���݂̎��Ԃ�1�t���[���O�̎��Ԃ�菬�����ꍇ�́A���[�v�����Ɣ��f�ł���B
	bool looped = motion_timer_ < prev_timer;

	//�S�C�x���g���`�F�b�N���A�K�v�ł���΃C�x���g�𔭍s����
	for (const auto& event : animation_events_) {
		// ���݂̃��[�V�������C�x���g�Ώۂ̃��[�V�����łȂ���΁A�������Ȃ�
		if (event->motion_ != motion_) continue;

		if (looped) {
			if (prev_timer < event->time_ || event->time_ <= motion_timer_) {
				event->callback_(); // �R�[���o�b�N�������Ăяo��
			}
		}
		else {
			if (prev_timer < event->time_ && event->time_ <= motion_timer_) {
				event->callback_(); // �R�[���o�b�N�������Ăяo��
			}
		}
	}

}

// ���[�V�����̕ύX
void AnimatedMesh::Animation::change_motion(GSuint motion, bool loop) {
	// ���݂Ɠ������[�V�����̏ꍇ�͉������Ȃ�
	if (motion_ == motion) return;
	// ��Ԓ��̑O���́A�O��̃��[�V�������X�V�����Ȃ��悤�ɂ���
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

// ���[�V�����̏I�����Ԃ��擾
float AnimatedMesh::Animation::motion_end_time() const {
	return gsGetEndAnimationTime(animation_, motion_);
}

// ���ݍĐ����̃��[�V�����^�C�}���擾
float AnimatedMesh::Animation::current_motion_time() const {
	return motion_timer_;
}

// ���ݍĐ����̃��[�V�����^�C�}��ݒ�
void AnimatedMesh::Animation::current_motion_time(float time) {
	motion_timer_ = time;
}

// ���[�V�������I�����Ă��邩�H
bool AnimatedMesh::Animation::is_motion_end() const {
	// ���[�v���[�V�����͏I�����Ȃ�
	if (motion_loop_) return false;
	// �I�����Ă��邩�H
	return motion_timer_ >= (motion_end_time() - 1.0f);
}

// �{�[���̃��[�J���ϊ��s����擾
GSmatrix4 AnimatedMesh::Animation::local_bone_matrix(GSuint bone_no) const {
	GSmatrix4 result;
	// ��ԕt���A�j���[�V�����ϊ��s����v�Z
	gsCalculateBoneAnimationLerp(
		animation_, prev_motion_, prev_motion_timer_,
		animation_, motion_, motion_timer_,
		bone_no,
		lerp_timer_ / LerpTime,
		&result
	);
	return result;
}

// �{�[������Ԃ�
GSuint AnimatedMesh::Animation::bone_count() const {
	return gsGetAnimationNumBones(animation_, motion_);
}

//�A�j���[�V�����C�x���g�o�^
void AnimatedMesh::Animation::add_animation_event(GSuint motion, float time, std::function<void()> callback) {
	//�A�j���[�V�����C�x���g��o�^
	animation_events_.push_back(new AnimationEvent(motion, time, callback));
}