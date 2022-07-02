#version 330
layout(location = 0) out vec4 out_FragColor;
in vec2 v_TexCoord;

// シーンのテクスチャ
uniform sampler2D u_BaseTexture;
// 彩度
uniform float u_Saturation = 1.0;
// カラー
uniform vec4 u_Color = vec4(1.0, 1.0, 1.0, 1.0);
// 露出
uniform float u_Exposure = 0.6;

// ACESトーンマッピング
vec3 ACESFilm(vec3 x) {
    x *= u_Exposure;
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0, 1.0);
}

// ガンマカラー空間からリニアカラー空間に変換
vec3 GammaToLinearSpace(vec3 color) {
    return pow(color, vec3(2.2));
}

// リニアカラー空間からガンマカラー空間に変換
vec3 LinearToGammaSpace(vec3 color) {
    return pow(color, vec3(1.0/2.2));
}

void main(void) {
    // シーンのカラーを取得
    vec4 col = texture(u_BaseTexture, v_TexCoord);
    // リニアカラー空間に変換
    col.rgb = GammaToLinearSpace(col.rgb);
    // トーンマッピング
    col.rgb = ACESFilm(col.rgb);
    // 元画像のRGBカラーをグレースケールに変換する
    vec3 intensity = vec3(col.r * 0.299 + col.g * 0.587 + col.b * 0.114);    
    // 彩度の調整
    col.rgb = mix(intensity.rgb, col.rgb, u_Saturation) * GammaToLinearSpace(u_Color.rgb);
    // ガンマカラー空間に変換
    col.rgb = LinearToGammaSpace(col.rgb);
    out_FragColor = vec4(col.rgb, 1.0);
}
