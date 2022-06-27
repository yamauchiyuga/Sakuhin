#include "AnimatedMesh.h"
#include <algorithm>
#include <cmath>

// 補間フレーム数
const GSfloat LerpTime{ 10.0f };

// コンストラクタ
AnimatedMesh::AnimatedMesh(GSuint mesh, GSuint skeleton, GSuint animation, GSuint motion, bool loop, GSuint layer_count, GSuint bone_count) :
	mesh_{ mesh },
	skeleton_{ skeleton },
	animation_layers_(layer_count, Animation{ animation, motion, loop }),
	animation_layer_indices_(bone_count, 0),
	local_bone_matrices_(bone_count, GS_MATRIX4_IDENTITY),
	bone_matrices_(bone_count, GS_MATRIX4_IDENTITY),
	transform_{ GS_MATRIX4_IDENTITY } {
}

// 更新
void AnimatedMesh::update(float delta_time) {
	// アニメーションレイヤーを更新
	for (auto& layer : animation_layers_) {
		layer.update(delta_time);
	}
}

// 描画
void AnimatedMesh::draw() const {
	// スケルトンの自動計算を無効にする
	gsDisable(GS_CALC_SKELETON);
	// スケルトンのバインド
	gsBindSkeleton(skeleton_);
	// ボーンのワールド変換行列を設定
	gsSetMatrixSkeleton(bone_matrices_.data());

	glPushMatrix();
	// 座標変換行列を設定する
	glMultMatrixf(transform_);
	// メッシュの描画
	gsDrawMesh(mesh_);
	glPopMatrix();

	// スケルトンの自動計算を有効にする
	gsEnable(GS_CALC_SKELETON);
}

// モーションの変更（レイヤー指定なし）
void AnimatedMesh::change_motion(GSuint motion, bool loop) {
	for (auto& layer : animation_layers_) {
		layer.change_motion(motion, loop);
	}
}

// モーションの変更（レイヤー指定あり）
void AnimatedMesh::change_motion(GSuint layer, GSuint motion, bool loop) {
	animation_layers_[layer].change_motion(motion, loop);
}

// レイヤーインデックスの設定
void AnimatedMesh::set_layer_indices(GSuint layer_index, const GSuint layer_bones[], GSuint layer_bones_count) {
	for (GSuint i = 0; i < layer_bones_count; ++i) {
		animation_layer_indices_[layer_bones[i]] = layer_index;
	}
}

// 座標変換を行う
void AnimatedMesh::transform(const GSmatrix4& matrix) {
	// 座標変換行列を更新
	transform_ = matrix;
	// 各レイヤーのからローカル変換行列を取得
	for (GSuint i = 0; i < gsGetSkeletonNumBones(skeleton_); ++i) {
		local_bone_matrices_[i] = animation_layers_[animation_layer_indices_[i]].local_bone_matrix(i);
	}
	// ボーンの変換行列を計算
	gsBindSkeleton(skeleton_);
	gsCalculateSkeleton(NULL, local_bone_matrices_.data(), bone_matrices_.data());
}

// モーションの終了時間を取得
float AnimatedMesh::motion_end_time(GSuint layer) const {
	return animation_layers_[layer].motion_end_time();
}

// モーションが終了しているか？
bool AnimatedMesh::is_motion_end(GSuint layer) const {
	return animation_layers_[layer].is_motion_end();
}

// ボーンのワールド変換行列を取得
GSmatrix4 AnimatedMesh::bone_matrices(int bone_no) const {
	return bone_matrices_[bone_no] * transform_;
}

// 現在のモーションの再生時間を取得
float AnimatedMesh::current_motion_time(GSuint layer) const {
	return animation_layers_[layer].current_motion_time();
}

// 現在のモーションの再生時間を取得
void AnimatedMesh::current_motion_time(float time, GSuint layer) {
	animation_layers_[layer].current_motion_time(time);
}

//アニメーションイベント登録
void AnimatedMesh::add_event(GSuint motion, GSfloat time, std::function<void()> callback){
	for (auto& layer : animation_layers_) {
		layer.add_event(motion, time, callback);
	}
}


// コンストラクタ
AnimatedMesh::Animation::Animation(GSuint animation, GSuint motion, bool loop) :
	animation_{ animation },
	motion_{ motion },
	motion_timer_{ 0.0f },
	motion_loop_{ loop },
	prev_motion_{ motion },
	prev_motion_timer_{ 0.0f },
	lerp_timer_{ 0.0f } {
}


// 更新
void AnimatedMesh::Animation::update(float delta_time) {
	//更新前のアニメーションタイマー
	GSfloat prev_timer = motion_timer_;
	//アニメーションタイマの更新
	motion_timer_ += delta_time;

	//アニメーションイベントの実行
	for (const auto& animation_event : events_) {
		if (animation_event->motion_ == motion_ &&
			prev_timer < animation_event->time_ &&
			animation_event->time_ <= motion_timer_) {
			animation_event->callback_();
		}
	}

	if (motion_loop_) {
		//モーションタイマをループさせる
		motion_timer_ = std::fmod(motion_timer_, motion_end_time());
	}
	else {
		//モーションタイマをクランプする
		motion_timer_ = std::min(motion_timer_, motion_end_time() - 1.0f);
	}
	//補間タイマの更新
	lerp_timer_ = std::min(lerp_timer_ + delta_time, LerpTime);

}

// モーションの変更
void AnimatedMesh::Animation::change_motion(GSuint motion, bool loop) {
	// 現在と同じモーションの場合は何もしない
	if (motion_ == motion) return;
	// 補間中の前半は、前回のモーションを更新をしないようにする
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

// モーションの終了時間を取得
float AnimatedMesh::Animation::motion_end_time() const {
	return gsGetEndAnimationTime(animation_, motion_);
}

// 現在再生中のモーションタイマを取得
float AnimatedMesh::Animation::current_motion_time() const {
	return motion_timer_;
}

// 現在再生中のモーションタイマを設定
void AnimatedMesh::Animation::current_motion_time(float time) {
	motion_timer_ = time;
}

// モーションが終了しているか？
bool AnimatedMesh::Animation::is_motion_end() const {
	// ループモーションは終了しない
	if (motion_loop_) return false;
	// 終了しているか？
	return motion_timer_ >= (motion_end_time() - 1.0f);
}

// ボーンのローカル変換行列を取得
GSmatrix4 AnimatedMesh::Animation::local_bone_matrix(GSuint bone_no) const {
	GSmatrix4 result;
	// 補間付きアニメーション変換行列を計算
	gsCalculateBoneAnimationLerp(
		animation_, prev_motion_, prev_motion_timer_,
		animation_, motion_, motion_timer_,
		bone_no,
		lerp_timer_ / LerpTime,
		&result
	);
	return result;
}

// ボーン数を返す
GSuint AnimatedMesh::Animation::bone_count() const {
	return gsGetAnimationNumBones(animation_, motion_);
}

void AnimatedMesh::Animation::add_event(GSuint motion, GSfloat time, std::function<void()> callback){
	//アニメーションイベントを登録
	events_.push_back( std::make_shared<AnimationEvent>(motion, time, callback));
}
