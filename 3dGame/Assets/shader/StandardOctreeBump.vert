#version 330
layout(location = 0) in vec4  in_Position;      // 頂点座標
layout(location = 2) in vec3  in_Normal;        // 法線ベクトル
layout(location = 8) in vec2  in_TexCoord;      // テクスチャ座標
layout(location = 14) in vec3 in_Tangent;       // 接ベクトル
layout(location = 15) in vec3 in_Binormal;      // 従法線ベクトル

out vec4 v_WorldPosition;                       // 頂点座標
out vec3 v_WorldNormal;                         // 法線ベクトル
out vec3 v_WorldTangent;                        // 接ベクトル
out vec3 v_WorldBinormal;                       // 従法線ベクトル
out vec2 v_TexCoord;                            // テクスチャ座標
out vec2 v_TexCoord2;                           // ライトマップ用のテクスチャ座標

// 透視変換行列
uniform mat4 gs_ModelViewProjectionMatrix;

void main(void) {
    // ワールド変換系の頂点座標
    v_WorldPosition = in_Position;
    // ワールド座標系の法線ベクトル
    v_WorldNormal = in_Normal;
    // ワールド座標系の接ベクトル
    v_WorldTangent = in_Tangent;
    // ワールド座標系の従法線ベクトル
    v_WorldBinormal = in_Binormal;
    // テクスチャ座標の出力
    v_TexCoord = in_TexCoord;
    // 透視変換
    gl_Position = gs_ModelViewProjectionMatrix * in_Position;
}
