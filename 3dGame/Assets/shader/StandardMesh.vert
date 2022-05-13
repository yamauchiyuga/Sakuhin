// スキニングメッシュ頂点シェーダー
#version 330
layout(location = 0)  in vec4 in_Position;      // 頂点座標
layout(location = 2)  in vec3 in_Normal;        // 法線ベクトル
layout(location = 8)  in vec2 in_TexCoord;      // テクスチャ座標

out vec4 v_WorldPosition;                       // 頂点座標
out vec3 v_WorldNormal;                         // 法線ベクトル
out vec3 v_WorldTangent;                        // 接ベクトル
out vec3 v_WorldBinormal;                       // 従法線ベクトル
out vec2 v_TexCoord;                                // テクスチャ座標

// ワールド変換行列
uniform mat4 gs_ModelMatrix;
// 視野変換行列
uniform mat4 gs_ViewMatrix;
// 法線ベクトル変換行列
uniform mat4 gs_ModelNormlMatrix;
// 透視変換行列
uniform mat4 gs_ModelViewProjectionMatrix;

void main(void) {
    // ワールド変換
    v_WorldPosition = gs_ModelMatrix * in_Position;
    v_WorldNormal   = mat3(gs_ModelNormlMatrix) * in_Normal;
    // テクスチャ座標の出力
    v_TexCoord = in_TexCoord;
    // 透視変換
    gl_Position = gs_ModelViewProjectionMatrix * in_Position;
}
