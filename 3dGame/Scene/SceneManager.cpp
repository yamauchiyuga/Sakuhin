#include"SceneManager.h"
#include"SceneNull.h"

//static SceneNull scene_null;		//nullシーン（何もしないダミーシーン）
//static std::shared_ptr<SceneNull> scene_null;
//コンストラクタ
SceneManager::SceneManager() :
	current_scene_{ std::make_shared<SceneNull>() } 
{
}

//デストラクタ
SceneManager::~SceneManager() 
{
	clear();
}

//更新
void SceneManager::update(float delta_time) 
{
	//シーン更新
	current_scene_->update(delta_time);
	//シーンが終了しているか
	if (current_scene_->is_end())
	{
		//シーン変更する
		change(current_scene_->next());
	}
}

//描画
void SceneManager::draw()const 
{
	//現在のシーンを描画
	current_scene_->draw();
}

//終了
void SceneManager::end() 
{
	//現在のシーンを終了
	current_scene_->end();
	//nullシーンにする
	current_scene_ = std::make_shared<SceneNull>();
}

//シーンの追加
void SceneManager::add(const std::string& name, std::shared_ptr<IScene> scene)
{
	scenes_[name] = scene;
}
//シーン変更
void SceneManager::change(const std::string& name) 
{
	//現在のシーンを終了
	end();
	//現在のシーンを変更
	current_scene_ = scenes_[name];
	//現在のシーンを開始
	current_scene_->start();
}

//
void SceneManager::clear()
{
	//終了
	end();
	scenes_.clear();
}