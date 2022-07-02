#version 330
layout(location = 0) out vec4 out_FragColor;
in vec2 v_TexCoord;

// �V�[���̃e�N�X�`��
uniform sampler2D u_BaseTexture;
// �ʓx
uniform float u_Saturation = 1.0;
// �J���[
uniform vec4 u_Color = vec4(1.0, 1.0, 1.0, 1.0);
// �I�o
uniform float u_Exposure = 0.6;

// ACES�g�[���}�b�s���O
vec3 ACESFilm(vec3 x) {
    x *= u_Exposure;
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0, 1.0);
}

// �K���}�J���[��Ԃ��烊�j�A�J���[��Ԃɕϊ�
vec3 GammaToLinearSpace(vec3 color) {
    return pow(color, vec3(2.2));
}

// ���j�A�J���[��Ԃ���K���}�J���[��Ԃɕϊ�
vec3 LinearToGammaSpace(vec3 color) {
    return pow(color, vec3(1.0/2.2));
}

void main(void) {
    // �V�[���̃J���[���擾
    vec4 col = texture(u_BaseTexture, v_TexCoord);
    // ���j�A�J���[��Ԃɕϊ�
    col.rgb = GammaToLinearSpace(col.rgb);
    // �g�[���}�b�s���O
    col.rgb = ACESFilm(col.rgb);
    // ���摜��RGB�J���[���O���[�X�P�[���ɕϊ�����
    vec3 intensity = vec3(col.r * 0.299 + col.g * 0.587 + col.b * 0.114);    
    // �ʓx�̒���
    col.rgb = mix(intensity.rgb, col.rgb, u_Saturation) * GammaToLinearSpace(u_Color.rgb);
    // �K���}�J���[��Ԃɕϊ�
    col.rgb = LinearToGammaSpace(col.rgb);
    out_FragColor = vec4(col.rgb, 1.0);
}
