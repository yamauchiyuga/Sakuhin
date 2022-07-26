#ifndef ASSETS_H_
#define ASSETS_H_

//メッシュデータ
enum {
    Mesh_Player,
    Mesh_TitlePlayer,
    Mesh_Dragon,
    Mesh_TitleDragon,
    Mesh_Skeleton,
    Mesh_Witch,
};

//オクトー
enum {       
    Octree_Stage,
    Octree_Collider,
    Octree_TitelStage,
    Octree_TitelCollider
};

//テクスチャデータ
enum {
    Texture_Titel,
    Texture_TitelName,
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
    Se_PlayerDrop,
    Se_EnemyDamage,
    Se_DragonAttack1,
    Se_DragonAttack2,
    Se_DragonDeath,
    Se_DragonFire,
    Se_DragonFoot,
    Se_DragonLanding,	
    Se_DragonSpitFire,
    Se_DragonExplosion,
    Se_SkeletonAttack,
    Se_SkeletonDamage,
    Se_SkeletonDetate,
    Se_SkeletonGeneration,
    Se_WitchAttack,
    Se_WitchDamage,
    Se_WitchThunder,
    Se_WitchDetate,
    Se_SpitFire
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
    Effect_Blood,
    Effect_Explosion,
    Effect_Smoke,
    Effect_AppearanceBlood,
    Effect_Thunder,
    Effect_Attack,
    Effect_Attack1
};
#endif // !ASSETS_H_
