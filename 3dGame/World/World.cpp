#include"World.h"
#include"../Collision/Field.h"
#include"../Actor/Actor.h"
#include"../Assets.h"
#include"../Tween/Tween.h"
#include <GSeffect.h>
//デストラクタ
World::~World() {
	clear();
}

void World::update(float delta_time) {
	//フィールド更新
	field_->update(delta_time);
	//アクター更新
	actors_.update(delta_time);
	//アクター衝突
	actors_.collide();
	//アクター遅延更新
	actors_.late_update(delta_time);
	//アクター削除
	actors_.remove();
	//カメラ更新
	camera_->update(delta_time);
	//エフェクトの更新処理を行う
	gsUpdateEffect(delta_time);
	light_->update(delta_time);
}
// 描画
void World::draw() const {
	// カメラの描画
	camera_->draw();
	//エフェクト用のカメラを設定
	gsSetEffectCamera();
	// ライトの描画
	light_->draw();
	 //シャドウマップの描画
	gsDrawShadowMap(World::shadow_map_callback, (void*)this);
	// フィールドの描画
	field_->draw();
	// アクターの描画
	actors_.draw();
	// 半透明アクターの描画
	actors_.draw_transparent();
	//エフェク描画
	gsDrawEffect();
	
}

void World::draw_gui() const {
	// GUIの描画
	actors_.draw_gui();
	//ゲームクリアを描画
	draw_game_cler_texture();
	//ゲームオーバーを描画
	draw_game_over_texture();
	//フェイドアウト
	draw_fade();


}
// 消去
void World::clear() {
	// アクターを消去
	actors_.clear();
	// カメラを消去
	//delete camera_;
	camera_ = nullptr;
	// ライトを消去
	//delete light_;
	light_ = nullptr;
	// フィールドを消去
	//delete field_;
	field_ = nullptr;
	//各変数の値を初期化
	is_game_over_ = false;
	fade_alpha_ = 0.0f;
	game_over_alpha_ = 0.0f;
	clear_alpha_ = 0.0f;
}
// カメラの追加
void World::add_camera(std::shared_ptr<Actor> camera) {
	camera_ =camera;
}

// ライトの追加
void World::add_light(std::shared_ptr<Actor> light) {
	light_ = light;
}

// フィールドの追加
void World::add_field(std::shared_ptr<Field> field) {
	field_ = field;
}

// シャドウマップの描画用の関数
void World::shadow_map_callback(void* param, const GSmatrix4* view, const GSmatrix4* projection) {
	World* self = (World*)param;
	// シャドウマップにはアクターのみ描画
	self->actors_.draw();
}


// アクターの追加
void World::add_actor(std::shared_ptr<Actor> actor) {
	actors_.add(std::move(actor));
}

// アクターの検索
std::shared_ptr<Actor> World::find_actor(const std::string& name) const {
	return actors_.find(name);
}

// 指定したタグ名を持つアクターの検索
std::vector<std::shared_ptr<Actor>> World::find_actor_with_tag(const std::string& tag) const {
	return actors_.find_with_tag(tag);
}

// アクター数を返す
int World::count_actor() const {
	return actors_.count();
}

// 指定したタグ名を持つアクター数を返す
int World::count_actor_with_tag(const std::string& tag) const {
	return actors_.count_with_tag(tag);
}

// メッセージ送信
void World::send_message(const std::string& message, std::shared_ptr<void> param) {
	actors_.send_message(message, param);
}
void World::draw_fade()const {
	const GScolor color{ 1.0f, 1.0f, 1.0f, fade_alpha_ };
	gsDrawSprite2D(Texture_Fade, NULL, NULL, NULL, &color, NULL, 0.0f);
}

//ゲームオーバー
void World::game_over(){
	//120フレームかけて死亡画像をフェードイン
	Tween::value(0.0f, 1.0f, 120.0f, [=](GSfloat val) {game_over_alpha_ = val; }).delay(120.0f);
	//100フレームかけてシーンをフェードアウト
	Tween::value(0.0f, 1.0f, 100.0f, [=](GSfloat val) {fade_alpha_ = val; }).delay(300.0f)
		.on_complete([=] {is_game_over_ = true; });
}

//ゲームクリア
void World::game_cler(){

	//画像の大きさをセット
	clear_scale_ = { 1.5f, 1.5f };
	//100フレームかけてシーンをフェードアウト
	Tween::value(0.0f, 1.0f, 100.0f, [=](GSfloat val) {fade_alpha_ = val; }).delay(540.0f)
		.on_complete([=] {is_game_over_ = true; });
	//20フレームかけてクリア画像をフェードイン
	Tween::value(0.0f, 1.0f, 20.0f, [=](GSfloat val) {clear_alpha_ = val; }).delay(300.0f)
		.on_complete([=] {gsPlaySE(Se_GameClear); });
	//20フレームかけてクリア画像を小さくする
	Tween::vector2(clear_scale_, GSvector2{ 1.0f, 1.0f }, 20.0f,[=](GSvector2 val) {clear_scale_ = val; })
		.delay(300.0f);
}

//ゲームオーバーテクスチャ
void World::draw_game_over_texture() const{
	const GScolor color{ 1.0f, 1.0f, 1.0f, game_over_alpha_ };
	gsDrawSprite2D(Texture_GameOver, NULL, NULL, NULL, &color, NULL, 0.0f);
}

//ゲームクリアテクスチャ
void World::draw_game_cler_texture() const{
	const GSvector2 position{ 640.0f, 360.0f };
	const GScolor color{ 1.0f, 1.0f, 1.0f, clear_alpha_ };
	gsDrawSprite2D(Texture_GameClear, &position, NULL, &position, &color, &clear_scale_, 0.0f);
}

// カメラの取得
std::shared_ptr<Actor> World::camera() {
	return camera_;
}

// ライトの取得
std::shared_ptr<Actor> World::light() {
	return light_;
}

// フィールドの取得
std::shared_ptr<Field> World::field() {
	return field_;
}

//ゲームオーバーか？
bool World::is_game_over()const {
	return is_game_over_;
}