#ifndef ASSETS_H_
#define ASSETS_H_

//メッシュデータ
enum {
    Mesh_Player,
    Mesh_Dragon,
    Mesh_Skeleton,
    Mesh_Witch,
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
    Texture_Lock
};
//BGM
enum {
    Sound_Titel,
    Sound_Wind,
};

//SE
enum {
    Se_GameStart,
    Se_GameClear,
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
    Se_DragonLanding,	
    Se_DragonSpitFire,
    Se_DragonExplosion,
    Se_SkeletonDetate,
    Se_WitchDetate
};

// シェーダー
enum {
    Shader_StandardMesh,           
    Shader_StandardSkinnedMesh,     
    Shader_StandardOctree,          
    Shader_ShadowMapMesh,           
    Shader_ShadowMapSkinnedMesh     
};
//エフェクト
enum {
    Effect_TorchFlame,
    Effect_FireBall,
    Effect_HitSpark,
    Effect_Blood,
    Effect_Explosion,
    Effect_Smoke
};
#endif // !ASSETS_H_
