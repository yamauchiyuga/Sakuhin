#ifndef RAY_H_
#define RAY_H_

#include<gslib.h>


class Ray
{
public:
	//�f�t�H���g�R���X�g���N�^
	Ray() = default;
	//�R���X�g���N�^
	Ray(const GSvector3& position, const GSvector3& direction) :
		position{ position }, direction{ direction }{};
public:
	//���W
	GSvector3 position{ 0.0f,0.0f,0.0f };
	//����
	GSvector3 direction{ 0.0f,0.0f,0.0f };
};
#endif // !RAY_H_
