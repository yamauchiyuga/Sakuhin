// �����_�[�e�N�X�`���p���_�V�F�[�_�[
#version 330
layout(location = 0)  in vec4 in_Position;  // ���_���W
layout(location = 8)  in vec2 in_TexCoord;	// �e�N�X�`�����W

out vec2 v_TexCoord;

void main(void) {
   v_TexCoord  = in_TexCoord;
   gl_Position = in_Position;
}
