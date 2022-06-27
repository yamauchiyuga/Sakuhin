#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include <unordered_map>
#include<string>
#include<memory>

class IScene;

class SceneManager {
public:
	//コンストラクタ
	SceneManager();
	//デストラクタ
	~SceneManager();
	//更新
	void update(float delta_time);
	//描画
	void draw()const;
	//終了
	void end();
	//シーンの追加
	void add(const std::string& name, std::shared_ptr<IScene> scene);
	//シーン切り替え
	void change(const std::string& name);
	//削除
	void clear();
	//コピー禁止
	SceneManager(const SceneManager& other) = default;
	SceneManager& operator=(const SceneManager& other) = default;
private:
	// シーン
	std::unordered_map<std::string, std::shared_ptr<IScene>> scenes_;
	// 現在のシーン
	std::shared_ptr<IScene> current_scene_;
};
#endif // !SCENE_MANAGER_H_

