#version 330
layout(location = 0) in vec4 in_Position;      // 頂点座標

// 透視変換行列
uniform mat4 gs_ModelViewProjectionMatrix;

void main(void) {
    // 透視変換
    gl_Position = gs_ModelViewProjectionMatrix * in_Position;
}
