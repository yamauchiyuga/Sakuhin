#ifndef IWORLD_H_
#define IWORLD_H_
#include<string>
#include<vector>
#include<memory>

class Actor;
class Field;

//ワールド抽象インターフェース
class IWorld {
public:
	//仮想デストラクタ
	virtual ~IWorld() = default;

	//アクター追加
	virtual void add_actor(std::shared_ptr<Actor> actor) = 0;
	//アクター検索
	virtual std::shared_ptr<Actor>find_actor(const std::string& name) const = 0;
	//指定したタグ名を持つアクターの検索
	virtual std::vector<std::shared_ptr<Actor>> find_actor_with_tag(const std::string& tag)const = 0;
	//アクター数を返す
	virtual int count_actor()const = 0;
	//指定したタグ名を持つアクターを返す
	virtual int count_actor_with_tag(const std::string& tag)const = 0;
	//メッセージ送信
	virtual void send_message(const std::string& message, std::shared_ptr<void> param = nullptr) = 0;

	//ゲームオーバー
	virtual void game_over() = 0;
	//ゲームクリア
	virtual void game_clear() = 0;

	// フィールドの取得
	virtual std::shared_ptr<Field> field() = 0;
	// カメラの取得
	virtual std::shared_ptr<Actor> camera() = 0;
	// ライトの取得
	virtual std::shared_ptr<Actor> light() = 0;

};
#endif 

