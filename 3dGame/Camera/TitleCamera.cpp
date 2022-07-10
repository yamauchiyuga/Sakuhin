#include"TitleCamera.h"

TitleCamera::TitleCamera(std::shared_ptr<IWorld> world, const GSvector3& position, const GSvector3& at)
{
	world_ = world;
	tag_ = "CameraTag";
	name_ = "Camera";
	transform_.position(position);
	transform_.lookAt(at);
}
 
void TitleCamera::draw()const 
{
	//バッファのクリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//透視射影後の変換行列を設定する
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		45.0f,
		1280.0f / 720.0f,
		0.3f,
		500.0f
	);
	//視点の位置
	GSvector3 eye = transform_.position();
	//注視点の位置
	GSvector3 at = eye + transform_.forward();
	//視点の上方向
	GSvector3 up = transform_.up();
	//カメラの設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,     // 視点の位置
		at.x, at.y, at.z,      // 注視点の位置
		up.x, up.y, up.z      // 視点の上方向
	);
}