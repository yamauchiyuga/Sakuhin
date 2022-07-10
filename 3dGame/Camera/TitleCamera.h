#ifndef TITLE_CAMERA_H_
#define TITLE_CAMERA_H_

#include"../Actor/Actor.h"
#include<memory>

class TitleCamera :public Actor 
{
public:
	//
	TitleCamera(std::shared_ptr<IWorld> world, const GSvector3& position, const GSvector3& target);
	//•`‰æ
	virtual void draw() const override;
};

#endif // !TITLE_CAMERA_H_