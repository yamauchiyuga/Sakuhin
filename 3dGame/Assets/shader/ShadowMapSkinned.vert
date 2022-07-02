// スキニングメッシュ用頂点シェーダー（シャドウマップ用）
#version 330
layout(location = 0)  in vec4 in_Position;      // 頂点座標
layout(location = 1)  in vec4 in_BoneWeight;    // ボーンのウェイト
layout(location = 7)  in vec4 in_BoneIndices;   // ボーンのインデクス

// 透視変換行列
uniform mat4 gs_ModelViewProjectionMatrix;
// ボーンの変換行列
uniform mat4 gs_BoneMatrices[200];

void main(void) {
    // スキニング用の変換行列の計算
    mat4 matLocal = gs_BoneMatrices[int(in_BoneIndices.x)] * in_BoneWeight.x
                  + gs_BoneMatrices[int(in_BoneIndices.y)] * in_BoneWeight.y
                  + gs_BoneMatrices[int(in_BoneIndices.z)] * in_BoneWeight.z
                  + gs_BoneMatrices[int(in_BoneIndices.w)] * in_BoneWeight.w;
    // スキニング後のローカル座標の計算
    vec4 localPosition = matLocal * in_Position;
    // 透視変換
    gl_Position = gs_ModelViewProjectionMatrix * localPosition;
}
