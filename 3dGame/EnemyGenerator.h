#ifndef ENEMY_GENERATOR_H_ 
#define ENEMY_GENERATOR_H_

#include "CSVReader.h"
#include "Actor/Actor.h"
#include<utility>
#include<queue>
#include<string>
#include<memory>

struct EnemyNP 
{
	std::string EnemyName;
	GSvector3 EnemyPos;
}; 

class EnemyGenerator :public Actor 
{
public:
	// コンストラクタ
	EnemyGenerator(std::shared_ptr<IWorld> world, const std::string& csv_stage_data, const std::string& csv_fase_pos);
	// 更新
	virtual void update(float delta_time) override;
private:
	// 敵の生成
	void generate();
private:
	// 出現データ
	CsvReader   csv_statge_data_;
	//フェーズごとの生成位置
	CsvReader   csv_fase_pos_;
	//ステージデータ
	std::queue<std::pair<int, EnemyNP>> statge_data_;
	//
	std::vector<int> fase_pos_;
	//プレイヤー取得
	std::shared_ptr<Actor> player_;
	//次のフェーズ
	int next_fase_;
};

#endif
