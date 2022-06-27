// レンダーテクスチャ用頂点シェーダー
#version 330
layout(location = 0)  in vec4 in_Position;  // 頂点座標
layout(location = 8)  in vec2 in_TexCoord;	// テクスチャ座標

out vec2 v_TexCoord;

void main(void) {
   v_TexCoord  = in_TexCoord;
   gl_Position = in_Position;
}
