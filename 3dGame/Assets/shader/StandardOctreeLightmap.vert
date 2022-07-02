#version 330
layout(location = 0) in vec4 in_Position;
layout(location = 2) in vec3 in_Normal;
layout(location = 8) in vec2 in_TexCoord;
layout(location = 9) in vec2 in_TexCoord2;

out vec4 v_WorldPosition;                       // 頂点座標
out vec3 v_WorldNormal;                         // 法線ベクトル
out vec2 v_TexCoord;                            // テクスチャ座標
out vec2 v_TexCoord2;                           // ライトマップ用のテクスチャ座標

// 透視変換行列
uniform mat4 gs_ModelViewProjectionMatrix;

void main(void) {
    // ワールド変換系の頂点座標
    v_WorldPosition = in_Position;
    // ワールド座標系の法線ベクトル
    v_WorldNormal = in_Normal;
    // テクスチャ座標の出力
    v_TexCoord = in_TexCoord;
    // テクスチャ座標の出力
    v_TexCoord2 = in_TexCoord2;

    // 透視変換
    gl_Position = gs_ModelViewProjectionMatrix * in_Position;
}
