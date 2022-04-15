#ifndef ANIMATED_MESH_H_
#define ANIMATED_MESH_H_

#include <gslib.h>
#include <array>

// アニメーション付きメッシュクラス
class AnimatedMesh {
public:
    // コンストラクタ
    AnimatedMesh(GSuint mesh, GSuint skeleton, GSuint animation, GSuint motion = 0, bool loop = true);
    // 更新
    void update(float delta_time);
    // 描画
    void draw() const;
    // モーションの変更
    void change_motion(GSuint motion, bool loop = true);
    // 変換行列を設定する
    void transform(const GSmatrix4& matrix);
    // モーションの終了時間を取得
    float motion_end_time() const;
    // ボーンの変換行列を取得
    const GSmatrix4& bone_matrices(int bone_no) const;

private:
    // メッシュ
    GSuint		mesh_;
    // スケルトン
    GSuint		skeleton_;
    // アニメーション
    GSuint		animation_;
    // モーション番号
    GSuint		motion_;
    // モーションタイマ
    GSfloat		motion_timer_;
    // モーションループフラグ
    bool		motion_loop_;
    // 前回再生したモーション番号
    GSuint		prev_motion_;
    // 前回再生した最終アニメーションタイマ
    GSfloat		prev_motion_timer_;
    // 補間アニメーションタイマ
    GSfloat		lerp_timer_;
    // ボーンのローカル用変換行列
    std::array<GSmatrix4, 256>	local_bone_matrices_;
    // ボーンのワールド変換行列
    std::array<GSmatrix4, 256>	world_bone_matrices_;
};

#endif

