#version 330
in vec4 v_WorldPosition;                // 頂点座標
in vec3 v_WorldNormal;                  // 法線ベクトル
in vec3 v_WorldTangent;                 // 接ベクトル
in vec3 v_WorldBinormal;                // 従法線ベクトル
in vec2 v_TexCoord;                     // テクスチャ座標

layout(location = 0) out vec4 out_FragColor;

// マテリアル情報
uniform vec4 gs_MaterialDiffuse;
uniform vec4 gs_MaterialSpecular;
uniform vec4 gs_MaterialEmission;

// カラーパラメータ
uniform vec4 gs_Color;
uniform vec4 gs_SecondaryColor;

// テクスチャ
uniform sampler2D   gs_DiffseTexture;
uniform sampler2D   gs_SpecularTexture;
uniform sampler2D   gs_NormalTexture;
uniform sampler2D   gs_AmbientOcclusionTexture;
uniform sampler2D   gs_EmissionTexture;

// スペキュラキューブマップ
uniform samplerCube gs_SpecularCubemap0;
uniform samplerCube gs_SpecularCubemap1;
uniform float       gs_SpeculerCubemapBlendLerp;
uniform float       gs_SpecularCubemap0_MipmapCount;
uniform float       gs_SpecularCubemap1_MipmapCount;

// ライトのパラメータ
uniform vec4        gs_LightPosition;
uniform vec4        gs_LightAmbient;
uniform vec4        gs_LightDiffuse;
uniform vec4        gs_LightSpecular;

// シャドウマップ数
uniform int gs_ShadowMapCount;
// シャドウマップの変換行列
uniform mat4 gs_ShadowMapMatrices[4];
uniform float gs_ShadowMapSplitDepth[4];
uniform sampler2DShadow gs_ShadowMaps[4];
// シャドウマップの光の減衰率
uniform float gs_ShadowMapAttenuation;
// シャドウマップバイアス
uniform float gs_ShadowMapBias;

// 視点の座標
uniform vec3 gs_ViewPosition;

// 円周率
const float PI = 3.1415926;

// ラフネスの最小値
const float MIN_PERCEPTUAL_ROUGHNESS = 0.045;
const float MIN_ROUGHNESS            = 0.002025;

// ノンリニア空間からリニアに変換
vec3 ConvertToLinear(vec3 color) {
    return pow(color, vec3(2.2));
}

// リニア空間からノンリニア空間に変換
vec3 ConvertToNoneLinear(vec3 color) {
    return pow(color, vec3(1 / 2.2));
}

// スムースネスをラフネスに変換
float SmoothnessToPerceptualRoughness(float smoothness) {
    return (1.0f - smoothness);
}

// リニア空間のラフネスに変換
float PerceptualRoughnessToRoughness(float perceptualRoughness) {
    return perceptualRoughness * perceptualRoughness;
}

// 5乗
float Pow5(float x) {
    return x * x * x * x * x;
}

// フレネル反射
vec3 F_Schlick(float cosA, vec3 f0) {
    return f0 + (vec3(1.0) - f0) * Pow5(1.0 - cosA);
}

// マイクロファセット分布関数 Trowbridge-Reitz (GGX)
float D_GGX(float NoH, float roughness) {
    float a2 = roughness * roughness;
    float f = (NoH * a2 - NoH) * NoH + 1.0;
    return a2 / (PI * f * f);
}

// 可視性関数
float V_SmithGGXCorrelatedFast(float NoV, float NoL, float roughness) {
    float a = roughness;
    float GGXV = NoL * (NoV * (1.0 - a) + a);
    float GGXL = NoV * (NoL * (1.0 - a) + a);
    return 0.5f / (GGXV + GGXL + 1e-5);
}

// 鏡面反射BRDF（Cook-Torrance approximation）
vec3 SpecularBRDF(vec3 specularColor, float NoH, float NoV, float NoL, float LoH, float roughness) {
    float D = D_GGX(NoH, roughness);
    float V = V_SmithGGXCorrelatedFast(NoV, NoL, roughness);
    vec3 F = F_Schlick(LoH, specularColor);
    return min(vec3(1.0), D * V * F);
}

// 拡散反射
float Fd_Lambert() {
    return 1.0 / PI;
}

// 拡散反射BRDF
vec3 DiffuseBRDF(vec3 diffuseColor) {
    return diffuseColor * Fd_Lambert();
}

// 環境BRFFの計算
vec3 EnvBRDFApprox(vec3 specularColor, float roughness, float NoV) {
    const vec4 c0 = vec4(-1, -0.0275, -0.572, 0.022);
    const vec4 c1 = vec4( 1,  0.0425,  1.04, -0.04 );
    vec4 r = roughness * c0 + c1;
    float a004 = min( r.x * r.x, exp2( -9.28 * NoV ) ) * r.x + r.y;
    vec2 AB = vec2(-1.04, 1.04) * a004 + r.zw;
    return specularColor * AB.x + AB.y;
}

// IBL鏡面反射光マップからカラーを取得する
vec3 sampleSpecularCubemap(vec3 p, float lod) {
    vec3 cubemap0 = textureLod(gs_SpecularCubemap0, p, lod).rgb;
    vec3 cubemap1 = textureLod(gs_SpecularCubemap1, p, lod).rgb;
    return mix(cubemap0, cubemap1, gs_SpeculerCubemapBlendLerp) + gs_LightAmbient.rgb;
}

// IBLの計算
vec3 ImageBasedLighting(vec3 N, vec3 R, float NoV, vec3 diffuseColor, vec3 specularColor, float perceptualRoughness) {
    vec3 Ld  = sampleSpecularCubemap(N, gs_SpecularCubemap0_MipmapCount - 0.5).rgb * diffuseColor;// * Fd_Lambert();
    vec3 Lld = sampleSpecularCubemap(R, perceptualRoughness * gs_SpecularCubemap0_MipmapCount).xyz;
    vec3 Lr  = EnvBRDFApprox(specularColor, perceptualRoughness, NoV) * Lld;
    return Ld + Lr;
}

// 大域照明の計算
vec3 GlobalIllumination(vec3 N, vec3 R, float NoV, vec3 diffuseColor, vec3 specularColor, float perceptualRoughness) {
    // IBL拡散反射光
    vec3 Id  = sampleSpecularCubemap(N, gs_SpecularCubemap0_MipmapCount - 0.5).rgb;
    // 拡散反射光の計算
    vec3 Ld  = Id * diffuseColor;
    // 鏡面反射光の計算
    vec3 Lld = sampleSpecularCubemap(R, perceptualRoughness * gs_SpecularCubemap0_MipmapCount).xyz;
    vec3 Lr  = EnvBRDFApprox(specularColor, perceptualRoughness, NoV) * Lld;
    return Ld + Lr;
}

// シャドウマップの計算
float ShadowFactor() {
    for (int i = 0; i < gs_ShadowMapCount; ++i) {
      if (gl_FragCoord.z <= gs_ShadowMapSplitDepth[i]) {
        vec4 shadowCoord = gs_ShadowMapMatrices[i] * v_WorldPosition;
        shadowCoord.z -= gs_ShadowMapBias;
        float shadow = texture(gs_ShadowMaps[i], shadowCoord.xyz);
        return mix(gs_ShadowMapAttenuation, 1.0, shadow);
      }
  }
  return 1.0;
}

// 法線マップの展開
vec3 UnpackNormalDXT5nm(vec4 packednormal) {
	vec3 normal;
	normal.xy = packednormal.wy * 2.0 - 1.0;
    normal.z = sqrt(1.0 - clamp(dot(normal.xy, normal.xy), 0.0, 1.0));
	return normal;
}

void main() {
    // アルベドカラー
    vec4 albedoColor  = texture(gs_DiffseTexture, v_TexCoord) * gs_MaterialDiffuse;
    // リニアカラーに変換
    albedoColor.rgb  = ConvertToLinear(albedoColor.rgb);
    // メタリック・スムースネス
    vec4 metallicSmoothness = texture(gs_SpecularTexture, v_TexCoord) * gs_MaterialSpecular;
    // 拡散反射カラーの取得（非金属）
    vec3 diffuseColor = mix(albedoColor.rgb, vec3(0.0), metallicSmoothness.r);
    // 鏡面反射カラーの取得（金属）
    vec3 specularColor = mix(vec3(0.04), albedoColor.rgb, metallicSmoothness.r);
    // スムースネスをラフネスに変換 0.0(ツルツル）～1.0(ザラザラ）
    float perceptualRoughness = SmoothnessToPerceptualRoughness(metallicSmoothness.a);
    // 知覚的なラフネスを変換　0.0(ツルツル）～1.0(ザラザラ）
    float roughness = PerceptualRoughnessToRoughness(perceptualRoughness);
    // ラフネスの最小値を補正する（ハイライトがなくなるの防ぐ）
    roughness = max(roughness, MIN_ROUGHNESS);
    // アンビエントオクルージョン
    float ao = texture(gs_AmbientOcclusionTexture, v_TexCoord).r;
    // エミッションカラー
    vec4 emissonColor = texture(gs_EmissionTexture, v_TexCoord) * gs_MaterialEmission;
    // リニアカラーに変換する
    emissonColor.rgb = ConvertToLinear(emissonColor.rgb);

    // 各種ベクトルの計算
    mat3 TBN = mat3(normalize(v_WorldTangent), normalize(v_WorldBinormal), normalize(v_WorldNormal));
    vec3 N = TBN * normalize(UnpackNormalDXT5nm(texture(gs_NormalTexture, v_TexCoord)));
    vec3 L = normalize(gs_LightPosition.xyz);
    vec3 V = normalize(gs_ViewPosition.xyz  - v_WorldPosition.xyz);
    vec3 H = normalize(L + V);
    vec3 R = reflect(-V, N);
    float NoV = abs(dot(N, V));
    float NoL = max(0.0001, dot(N, L));
    float NoH = max(0.0001, dot(N, H));
    float LoV = max(0.0001, dot(L, V));
    float LoH = max(0.0001, dot(L, H));

    // 放射照度の計算
    vec3 irradiance = NoL * gs_LightDiffuse.rgb * PI * ShadowFactor();
    // 双方向反射率分布関数(BRDF)の計算
    vec3 Fd = DiffuseBRDF(diffuseColor);
    vec3 Fr = SpecularBRDF(specularColor, NoH, NoV, NoL, LoH, roughness);
    vec3 BRDF = (Fd + Fr) * irradiance;
    // イメージベースドライティング
    vec3 GI = GlobalIllumination(N, R, NoV, diffuseColor, specularColor, perceptualRoughness);

    // 最終的なカラーの計算
    vec3 finalColor = (BRDF + GI) * ao + emissonColor.rgb;
    // ノンリニアカラーに変換して出力
    finalColor = ConvertToNoneLinear(finalColor);
    // カラー調整
    finalColor.rgb = finalColor.rgb * gs_Color.rgb + gs_SecondaryColor.rgb;
    // 最終結果を計算
    out_FragColor = vec4(finalColor, albedoColor.a);
}
