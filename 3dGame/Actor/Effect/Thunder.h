#ifndef THUNDER_H_
#define THUNDER_H_

#include<memory>
#include "../Actor.h"
class Thunder :public Actor
{
public:
	Thunder(std::shared_ptr<IWorld> world, const GSvector3& position);
    // 更新
    virtual void update(float delta_time) override;
    // 衝突リアクション
    virtual void react(Actor& other) override;

private:
    //
    std::shared_ptr<Actor> player_;
    //
    float timer_;
};

#endif // !THUNDER_H_



