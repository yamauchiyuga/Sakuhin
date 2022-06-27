#include "BoundingSphere.h"

// �R���X�g���N�^
BoundingSphere::BoundingSphere(float radius, const GSvector3& center) :
    radius_{ radius }, center_{ center } {
}

// ���s�ړ�
BoundingSphere BoundingSphere::translate(const GSvector3& position) const {
    return BoundingSphere{ radius_, center_ + position };
}

// ���W�ϊ�
BoundingSphere BoundingSphere::transform(const GSmatrix4& matrix) const {
    return BoundingSphere{ radius_ * matrix.getScale().y, matrix.transform(center_) };
}


// �������Ă��邩�H
bool BoundingSphere::intersects(const BoundingSphere& other) const {
    return gsCollisionSphereAndSphere(
        &center_, radius_, &other.center_, other.radius_) == GS_TRUE;
}

// �f�o�b�O�\��
void BoundingSphere::draw() const {
    glPushMatrix();
    glTranslatef(center_.x, center_.y, center_.z);
    glutWireSphere(radius_, 16, 16); // ���C���[�t���[���̋��̂�`��
    glPopMatrix();
}

