#include "ActorManager.h"
#include "Actor.h"

#include<algorithm>
// デストラクタ
ActorManager::~ActorManager()
{
	clear();
}

// アクターの追加
void ActorManager::add(std::shared_ptr<Actor> actor) 
{
	actors_.push_back(actor);
}

// アクターの更新
void ActorManager::update(float delta_time) 
{
	for (auto i = actors_.begin(); i != actors_.end(); ++i) 
	{
		(*i)->update(delta_time);
	}

}

// アクターの遅延更新
void ActorManager::late_update(float delta_time) 
{
	for (auto i = actors_.begin(); i != actors_.end(); ++i) 
	{
		(*i)->late_update(delta_time);
	}
}

// アクターの描画
void ActorManager::draw() const 
{
	for (auto i = actors_.begin(); i != actors_.end(); ++i) 
	{
		(*i)->draw();
	}
}

// 半透明アクターの描画
void ActorManager::draw_transparent() const 
{
	for (auto i = actors_.begin(); i != actors_.end(); ++i) 
	{
		(*i)->draw_transparent();
	}
}

// アクターのGUI描画 
void ActorManager::draw_gui() const 
{
	for (auto i = actors_.begin(); i != actors_.end(); ++i) 
	{
		(*i)->draw_gui();
	}
}

// アクターの衝突判定
void ActorManager::collide() {
	for (auto i = actors_.begin(); i != actors_.end(); ++i) {
		for (auto j = std::next(i); j != actors_.end(); ++j) {
			(*i)->collide(**j);
		}
	}
}

// 死亡しているアクターの削除
void ActorManager::remove() 
{
	actors_.remove_if([](const auto& actor) {return actor->is_dead(); });
}

// アクターの検索
std::shared_ptr<Actor> ActorManager::find(const std::string& name) const
{

	for (auto i = actors_.begin(); i != actors_.end();++i ) 
	{
		if ((*i)->name() == name) 
		{
			return *i;
		}
	}
	return nullptr;
}

// 指定したタグ名を持つアクターの検索
std::vector<std::shared_ptr<Actor>> ActorManager::find_with_tag(const std::string& tag) const 
{
	std::vector<std::shared_ptr<Actor>> result;
	for (auto i = actors_.begin(); i != actors_.end();++i ) 
	{
		if ((*i)->tag() == tag) 
		{
			result.push_back(*i);
		}
	}
	return result;

}

// アクター数を返す
int ActorManager::count() const
{
	return (int)actors_.size();
}

// 指定したタグのアクター数を返す
int ActorManager::count_with_tag(const std::string& tag) const 
{
	int result = 0;
	for (auto i = actors_.begin(); i != actors_.end();++i ) 
	{
		if ((*i)->tag() == tag) 
		{
			++result;
		}
	}
	return result;
}

// メッセージの送信
void ActorManager::send_message(const std::string& message, std::shared_ptr<void> param) 
{
	for (auto i = actors_.begin(); i != actors_.end(); ++i) 
	{
		(*i)->handle_message(message, param);
	}
}

// 消去
void ActorManager::clear() 
{
	actors_.clear();
}

