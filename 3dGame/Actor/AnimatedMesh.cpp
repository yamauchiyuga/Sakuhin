#include<cmath>
#include "AnimatedMesh.h"

// 補間フレーム数
const GSfloat LerpTime{ 10.0f };

// コンストラクタ
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

// 更新
void AnimatedMesh::update(float delta_time) {
    // アニメーションタイマの更新
    motion_timer_ += delta_time;
    // ループアニメーションか？
    if (motion_loop_) {
        // モーションタイマをループさせる
        motion_timer_ = std::fmod(motion_timer_, motion_end_time());
    }
    else {
        // モーションタイマをクランプする
        motion_timer_ = std::min(motion_timer_, motion_end_time() - 1.0f);
    }
    // 補間タイマの更新(LerpTime以上にならないようにクランプする）
    lerp_timer_ = std::min(lerp_timer_ + delta_time, LerpTime);
}

// 描画
void AnimatedMesh::draw() const {
    // スケルトンの自動計算を無効にする
    gsDisable(GS_CALC_SKELETON);
    // スケルトンのバインド
    gsBindSkeleton(skeleton_);
    // ボーンのワールド変換行列を設定
    gsSetMatrixSkeleton(world_bone_matrices_.data());
    // メッシュの描画
    gsDrawMesh(mesh_);
    // スケルトンの自動計算を有効にする
    gsEnable(GS_CALC_SKELETON);
}

// モーションの変更
void AnimatedMesh::change_motion(GSuint motion, bool loop) {
    // 現在と同じモーションの場合は何もしない
    if (motion_ == motion) return;
    // 補間中（前半）は、前回のモーションを更新をしないようにする
    if (lerp_timer_ > (LerpTime * 0.5f)) {
        // 前回のモーション番号とモーション時間を保存
        prev_motion_ = motion_;
        prev_motion_timer_ = motion_timer_;
        // 補間中タイマの初期化 
        lerp_timer_ = 0.0f;
    }
    // モーションの更新
    motion_ = motion;
    // モーションタイマの初期化
    motion_timer_ = 0.0f;
    // ループフラグの設定
    motion_loop_ = loop;
}

// 座標変換を行う
void AnimatedMesh::transform(const GSmatrix4& matrix) {
    // ボーンごとのローカル変換行列を計算
    gsCalculateAnimationLerp(
        animation_, prev_motion_, prev_motion_timer_,
        animation_, motion_, motion_timer_,
        lerp_timer_ / LerpTime,
        local_bone_matrices_.data()
    );
    // ボーンごとのワールド変換行列を計算
    gsBindSkeleton(skeleton_);
    gsCalculateSkeleton(&matrix, local_bone_matrices_.data(), world_bone_matrices_.data());
}

// モーションの終了時間を取得
float AnimatedMesh::motion_end_time() const {
    return gsGetEndAnimationTime(animation_, motion_);
}

// ボーンの変換行列を取得
const GSmatrix4& AnimatedMesh::bone_matrices(int bone_no) const {
    return world_bone_matrices_[bone_no];
}

