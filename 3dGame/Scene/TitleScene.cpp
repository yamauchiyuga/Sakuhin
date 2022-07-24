#include "TitleScene.h"
#include"../Input.h"
#include"../Assets.h"
#include"../Tween/Tween.h"
#include"../Collision/Field.h"
#include"../Actor/Light.h"
#include"../Actor/Enemy/TitleDragon.h"
#include"../Actor/Player/TitlePlayer.h"
#include"../Camera/TitleCamera.h"
#include<GSeffect.h>
#include <GSstandard_shader.h>


// 開始
void TitleScene::start()
{
	//レンダーターゲットの作成
	gsCreateRenderTarget(0, 1280, 720, GS_TRUE, GS_TRUE, GS_TRUE);
	//視錐台カリングを有効にする
	gsEnable(GS_FRUSTUM_CULLING);

	// 終了フラグの初期化
	is_end_ = false;
	//フェードフラグを初期化
	is_fade_ = true;
	//フェードのアルファ値を初期化
	alpha_ = 1.0f;
	
	gsPlayBGM(Sound_Titel);
	gsSetVolumeBGM(0.4f);
	//40フレームかけてフェードイン
	Tween::value(1.0f, 0.0f, 120.0f, [=](GSfloat val) {alpha_ = val; })
		.on_complete([=] {is_fade_ = false; });

	//シャドウマップの作成（2枚のカスケードシャドウ）
	static const GSuint shadow_map_size[] = { 2048, 1024 };
	gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
	//シャドウマップを適用する距離
	gsSetShadowMapDistance(80.0f);
	//シャドウマップバイアス
	gsSetShadowMapBias(0.0f);
	// ライトマップの読み込み(0番に読み込めば自動的に適用される）
	gsLoadLightmap(0, "Assets/Lightmap/Lightmap.txt");
	// 補助ライトの読み込み
	gsLoadAuxLight(0, "Assets/AuxLight/TitelAuxLight.txt");


	world_ = std::make_shared<World>();
	world_->add_actor(std::make_shared<TitleDragon>(world_, GSvector3{ 0.0f,3.0f,-5.0f }));
	world_->add_actor(std::make_shared<TitlePlayer>(world_, GSvector3{ 0.0f,0.0f,10.0f }));

	// フィールドクラスの追加
	world_->add_field(std::make_shared<Field>(Octree_TitelStage, Octree_TitelCollider));
	world_->add_camera(std::make_shared<TitleCamera>(world_, GSvector3{ 0.0f, 1.0f, 17.0f }, GSvector3{ 0.0f, 6.5, 0.0f }));
	// ライトクラスの追加
	world_->add_light(std::make_shared<Light>(world_));

	for (GSint i = 0; i < 8; ++i) {
		// 補助ライトの位置を取得
		GSvector3 position;
		gsGetAuxLightPosition(0, i, &position);
		// 補助ライトの位置に炎のエフェクトを出現させる
		gsPlayEffect(0, &position);
	}
}

// 更新
void TitleScene::update(float delta_time)
{
	world_->update(delta_time);
	if (is_fade_)return;
	// エンターキーを押したらシーン終了
	if (Input::is_a_push()) {
		gsPlaySE(Se_GameStart);
		is_end_ = true;     // シーン終了
	}
}

// 描画
void TitleScene::draw() const
{
	//レンダーターゲットを有効にする
	gsBeginRenderTarget(0);
	//レンダーターゲットのクリア
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	world_->draw();
	fog.draw();
	static const GSvector2 Pos{ 100.0f,-50.0f };
	gsDrawSprite2D(Texture_TitelName, &Pos, NULL, NULL, NULL, NULL, 0.0f);
	draw_fade();
	//レンダーターゲットを無効にする
	gsEndRenderTarget();

	// ポストエフェクトのパラメータ
	static GScolor color_{ 1.0f, 1.0f, 1.0f, 1.0f };
	static float   saturation_{ 0.8f };
	static float   luminance_{ 1.0f };
	static float   exposure_{ 0.8f };

	//シェーダーを有効にする
	GScolor col = color_ * luminance_;
	col.a = 1.0f;
	gsBeginShader(0);
	// 彩度の設定
	gsSetShaderParam1f("u_Saturation", saturation_);
	// テクスチャの設定
	gsSetShaderParamTexture("u_RenderTexture", 0);
	// 色調整
	gsSetShaderParam4f("u_Color", &col);
	// 露出
	gsSetShaderParam1f("u_Exposure", exposure_);
	// レンダーターゲット用のテクスチャをバインド
	gsBindRenderTargetTextureEx(0, 0, 0);
	// レンダーターゲットを描画
	gsDrawRenderTargetEx(0);
	// レンダーターゲット用のテクスチャの解除
	gsUnbindRenderTargetTextureEx(0, 0, 0);
	// シェーダーを無効にする
	gsEndShader();
	
}

// 終了しているか？
bool TitleScene::is_end() const {
	return is_end_;         // 終了フラグを返す
}

// 次のシーン名を返す
std::string TitleScene::next() const {
	return "GamePlayScene"; // 次のシーン名を返す
}

// 終了
void TitleScene::end() {
	// ワールドを消去
	world_->clear();
	//Tweenの削除
	Tween::clear();
	//BGMの削除
	gsStopBGM();
	//
	gsDeleteAuxLight(0);
	//
	gsDeleteLightmap(0);
}

//フェード処理
void TitleScene::draw_fade()const {
	GScolor Color{ 1.0f, 1.0f, 1.0f, alpha_ };
	gsDrawSprite2D(Texture_Fade, NULL, NULL, NULL, &Color, NULL, 0.0f);
}

