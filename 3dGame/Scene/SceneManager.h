#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include <unordered_map>
#include<string>

class IScene;

//
class SceneManager {
public:
	//
	SceneManager();
	//
	~SceneManager();
	//
	void update(float delta_time);
	//
	void draw()const;
	//
	void end();
	//
	void add(const std::string& name, IScene* scene);
	//
	void change(const std::string& name);
	//
	void clear();
	//
	SceneManager(const SceneManager& other) = default;
	SceneManager& operator=(const SceneManager& other) = default;
private:
	// シーン
	std::unordered_map<std::string, IScene*> scenes_;
	// 現在のシーン
	IScene* current_scene_;
};
#endif // !SCENE_MANAGER_H_

