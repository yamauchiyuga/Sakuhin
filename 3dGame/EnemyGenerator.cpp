#include "EnemyGenerator.h"
#include "World/IWorld.h"
#include "Actor/Enemy/Dragon.h"
#include "Actor/Enemy/Skeleton.h"
#include "Actor/Enemy/Witch.h"

const int CSvFase{ 0 };//フェーズの列
const int CsvName{ 1 };	// 名前の列
const int CsvPosition{ 2 }; // 座標の列
const int CsvFasePos{ 1 };//生成が発動する列

// コンストラクタ
EnemyGenerator::EnemyGenerator(std::shared_ptr<IWorld> world, const std::string& csv_stage_data, const std::string& csv_fase_pos)
{
	world_ = world;
	name_ = "EnemeyGenerator";
	tag_ = "Generator";
	csv_statge_data_.load(csv_stage_data);    // CSVファイルの読み込み
	csv_fase_pos_.load(csv_fase_pos);
	player_ = world_->find_actor("Player");
	next_fase_ = 0;

	for (int i = 0; i < csv_statge_data_.rows(); ++i) 
	{
		int TempFase = csv_statge_data_.geti(i, CSvFase);
		EnemyNP Temp;
		Temp.EnemyName = csv_statge_data_.get(i, CsvName);
		Temp.EnemyPos = GSvector3(csv_statge_data_.getf(i, CsvPosition), csv_statge_data_.getf(i, CsvPosition + 1), csv_statge_data_.getf(i, CsvPosition + 2));
		statge_data_.push(std::make_pair(TempFase, Temp));
	}

	for (int i = 0; i < csv_fase_pos_.rows(); ++i)
	{
		fase_pos_.push_back(csv_fase_pos_.geti(i, CsvFasePos));
	}
}

// 更新
void EnemyGenerator::update(float delta_time)
{
	if (statge_data_.empty())return;

	if (fase_pos_[next_fase_] > player_->transform().position().z) 
	{	
		while (statge_data_.front().first == next_fase_)
		{
			auto RowData = statge_data_.front();
			std::string EnemyName = RowData.second.EnemyName;
			if (EnemyName == "Skeleton")
			{
				world_->add_actor(std::make_unique<Skeketon>( world_,RowData.second.EnemyPos ));
			}
			if (EnemyName == "Witch") 
			{
				world_->add_actor(std::make_unique<Witch>( world_,RowData.second.EnemyPos ));
			}
			if (EnemyName == "Dragon") 
			{
				world_->add_actor(std::make_unique <Dragon>( world_,RowData.second.EnemyPos ));
			}

			statge_data_.pop();
			if (statge_data_.empty())return;

		}
		next_fase_++;
	}



}

// 生成
void EnemyGenerator::generate() {

}