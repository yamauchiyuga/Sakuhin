#ifndef WORLD_H_
#define WORLD_H_

#include"IWorld.h"
#include"../Actor/ActorManager.h"
#include<gslib.h>
#include<memory>

//ワールドクラス
class World : public IWorld
{
public:
	//コンストラクタ
	World() = default;
	//デストラクタ
	~World();
	//更新
	void update(float delta_time);
	//描画
	void draw()const;
	//GUI描画
	void draw_gui()const;
	//削除
	void clear();
	//カメラ追加
	void add_camera(std::shared_ptr<Actor> camera);
	//ライト追加
	void add_light(std::shared_ptr<Actor> light);
	//フィールド追加
	void add_field(std::shared_ptr<Field> field);


	//アクター追加
	virtual void add_actor(std::shared_ptr<Actor> actor)override;
	//アクター検索
	virtual std::shared_ptr<Actor>find_actor(const std::string& name)const override;
	 // 指定したタグ名を持つアクターの検索
	virtual std::vector<std::shared_ptr<Actor>> find_actor_with_tag(const std::string& tag) const override;
	// アクター数を返す
	virtual int count_actor() const override;
	// 指定したタグ名を持つアクター数を返す
	virtual int count_actor_with_tag(const std::string& tag) const override;
	// メッセージの送信
	virtual void send_message(const std::string& message, std::shared_ptr<void> param = nullptr) override;

	//フェード画像の描画
	void draw_fade() const;
	//
	virtual void game_over()override;
	//
	virtual void game_cler()override;
	//ゲームオーバー画像
	void draw_game_over_texture()const;
	//ゲームクリア画像
	void draw_game_cler_texture()const;

	// カメラの取得
	virtual std::shared_ptr<Actor> camera() override;
	// ライトの取得
	virtual std::shared_ptr<Actor> light() override;
	// フィールドの取得
	virtual std::shared_ptr<Field> field() override;
	//ゲームオーバーかどうか？
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
	std::shared_ptr<Actor> light_{ nullptr };
	// カメラ
	std::shared_ptr<Actor> camera_{ nullptr };
	// フィールド
	std::shared_ptr<Field> field_{ nullptr };

	//ゲームオーバーか？
	bool is_game_over_{ false };
	//フェード画像のアルファ値
	float fade_alpha_{ 0.0f };
	//死亡画像のアルファ値
	float game_over_alpha_{ 0.0f };
	//クリア画像のアルファ値
	float clear_alpha_{ 0.0f };
	//クリア画像の大きさ
	GSvector2 clear_scale_{ 1.5f, 1.5f };
};
#endif // !WORLD_H_
