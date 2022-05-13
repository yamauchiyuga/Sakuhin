#ifndef WORLD_H_
#define WORLD_H_

#include"IWorld.h"
#include"../Actor/ActorManager.h"
#include<gslib.h>

//ワールドクラス
class World : public IWorld{
public:
	//コンストラクタ
	World() = default;
	//デストラクタ
	~World();
	//更新
	void update(float delta_time);
	//描画
	void draw()const;
	//削除
	void clear();
	//カメラ追加
	void add_camera(Actor* camera);
	//ライト追加
	void add_light(Actor* light);
	//フィールド追加
	void add_field(Field* field);
	


	//アクター追加
	virtual void add_actor(Actor* actor)override;
	//アクター検索
	virtual Actor* find_actor(const std::string& name)const override;
	 // 指定したタグ名を持つアクターの検索
	virtual std::vector<Actor*> find_actor_with_tag(const std::string& tag) const override;
	// アクター数を返す
	virtual int count_actor() const override;
	// 指定したタグ名を持つアクター数を返す
	virtual int count_actor_with_tag(const std::string& tag) const override;
	// メッセージの送信
	virtual void send_message(const std::string& message, void* param = nullptr) override;

	//
	virtual void game_over()override;
	//
	virtual void game_cler()override;

	// カメラの取得
	virtual Actor* camera() override;
	// ライトの取得
	virtual Actor* light() override;
	// フィールドの取得
	virtual Field* field() override;

	bool is_game_over()const;

	// シャドウマップの描画用の関数
	static void shadow_map_callback(void* param, const GSmatrix4* view, const GSmatrix4* projection);

	// コピー禁止
	World(const World& other) = delete;
	World& operator = (const World& other) = delete;

private:
	// アクターマネージャー
	ActorManager  actors_;
	// ライト
	Actor* light_{ nullptr };
	// カメラ
	Actor* camera_{ nullptr };
	// フィールド
	Field* field_{ nullptr };

	//
	bool is_game_over_{ false };
};
#endif // !WORLD_H_
