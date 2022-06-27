#ifndef ASSETS_H_
#define ASSETS_H_

//メッシュデータ
enum {
    Mesh_Player,
    Mesh_Dragon,
    Mesh_Skeleton,
    Mesh_Witch,
    Mesh_Skybox
};

//オクトー
enum {       
    Octree_Stage,
    Octree_Collider
};

//テクスチャデータ
enum {
    Texture_Titel,
    Texture_Fade,
    Texture_GameOver,
    Texture_GameClear,
    Texture_HP,   
    Texture_Frame,
    Texture_ST,

};

enum {
    Sound_TitleBGM,
    Sound_PlayingBGM,
    Sound_ResultBGM
};

//SE
enum {
    Se_GameStart,
    Se_GameClear,
    Se_GameOver,
    Se_PlayerAttack,        
    Se_PlayerDamage, 
    Se_PlayerRun,
    Se_PlayerBlock,
    Se_EnemyDamage,
    Se_DragonAttack1,
    Se_DragonAttack2,
    Se_DragonDeath,
    Se_DragonFire,
    Se_DragonFoot,
    Se_DragonLanding,	//ドラゴンの着地SE
    Se_DragonSpitFire,
    Se_Skeleton_Attack,
    Se_WitchAttack
};

// シェーダー
enum {
    Shader_StandardMesh,            // メッシュ用シェーダー
    Shader_StandardSkinnedMesh,     // スキニングメッシュ用シェーダー
    Shader_StandardOctree,          // オクツリー用シェーダー
    Shader_ShadowMapMesh,           // メッシュ用シャドウマップシェーダー
    Shader_ShadowMapSkinnedMesh     // スキニングメッシュ用シャドウマップシェーダー
};
//エフェクト
enum {
    Effect_Fier
};
#endif // !ASSETS_H_
