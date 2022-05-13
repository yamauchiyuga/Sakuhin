#ifndef ASSETS_H_
#define ASSETS_H_

//メッシュデータ
enum {
    Mesh_Player,					//自機
    Mesh_Dragon,                     // 敵
    Mesh_Skybox                     // スカイボックス
};

//オクトー
enum {
    Octree_Koutei,                  //校庭
    Octree_KouteiCollider,           //校庭衝突判定
    Octree_Stage,
    Octree_Collider
};

//テクスチャデータ
enum {
    Texture_Title,          // タイトル
    Texture_Kendo,          // 剣道部長
    Texture_Karate,         // 空手部長
    Texture_Start,          // 開始メッセージ
    Texture_Number,         // 数字フォント
    Texture_Text,           // テキスト画像
    Texture_BlueBack,       // リザルト用背景
    Texture_Result1,        // リザルト用テキスト1
    Texture_Result2,        // リザルト用テキスト2
    Texture_Radar,          // レーダー画像
    Texture_RadarPoint,      // レーダーの点
    Texture_HP,             //HP表示の赤部分
    Texture_Frame,          //
    Texture_ST,

};

enum {
    Sound_TitleBGM,         // タイトルシーン用ＢＧＭ 
    Sound_PlayingBGM,       // ゲームシーン用のＢＧＭ 
    Sound_ResultBGM         // リザルト用ＢＧＭ 
};

enum {
    Se_GameStart,           // ゲーム開始時の効果音
    Se_PlayerAttack,        // 剣道部長 攻撃時効果音
    Se_PlayerDamage,        // 剣道部長 やられ効果音
    Se_EnemeyDamage,        // 空手部部長 やられ効果音
    Se_Timeout              // タイムアウトの効果音   
};

// シェーダー
enum {
    Shader_StandardMesh,            // メッシュ用シェーダー
    Shader_StandardSkinnedMesh,     // スキニングメッシュ用シェーダー
    Shader_StandardOctree,          // オクツリー用シェーダー
    Shader_ShadowMapMesh,           // メッシュ用シャドウマップシェーダー
    Shader_ShadowMapSkinnedMesh     // スキニングメッシュ用シャドウマップシェーダー
};
#endif // !ASSETS_H_
