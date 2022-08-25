#include "EnemyGenerator.h"

#include "../Actor/Enemy/Dragon.h"
#include "../Actor/Enemy/Skeleton.h"
#include "../Actor/Enemy/Witch.h"

static const int CsvFase{ 0 };			// �t�F�[�Y�̗�
static const int CsvName{ 1 };		// ���O�Ɣ����ʒu�̗�
static const int CsvPosition{ 2 };		// �����ʒu�̗�
static const int CsvFasePoint{ 0 };	// �t�F�[�Y�|�C���g�̗�
static const int CsvFasePos{ 1 };		// ���������������

// �R���X�g���N�^
EnemyGenerator::EnemyGenerator(std::shared_ptr<IWorld> world, const std::string& data_name, const std::string& pos_name)
{
	world_ = world;
	name_ = "EnemeyGenerator";
	tag_ = "Generator";

	load_csv(data_name, pos_name);
	read_stage_data();
	read_fase_point();
}

// �X�V
void EnemyGenerator::update(float delta_time)
{
	if (stage_data_.empty() || fase_point_.empty()) return;

	float PlayerPositionZ = world_->find_actor("Player")->transform().position().z;
	if (fase_point_.front().second < PlayerPositionZ) return;

	while (!stage_data_.empty() && 
		stage_data_.front().first == fase_point_.front().first)
	{
		auto RowData = stage_data_.front().second;
		std::string EnemyName = RowData.name_;

		if (EnemyName == "Skeleton") generate<Skeleton>(world_, RowData.pos_);
		if (EnemyName == "Witch") generate<Witch>(world_, RowData.pos_);
		if (EnemyName == "Dragon") generate<Dragon>(world_, RowData.pos_);

		stage_data_.pop();
	}
	fase_point_.pop();
}

void EnemyGenerator::load_csv(const std::string& data_name, const std::string& pos_name)
{
	csv_generator_data_.load(data_name);
	csv_generator_pos_.load(pos_name);
}

void EnemyGenerator::read_stage_data()
{
	for (int i = 0; i < csv_generator_data_.rows(); ++i)
	{
		int Fase = csv_generator_data_.geti(i, CsvFase);
		std::string Name = csv_generator_data_.get(i, CsvName);
		GSvector3 Pos(
			csv_generator_data_.getf(i, CsvPosition),
			csv_generator_data_.getf(i, CsvPosition + 1),
			csv_generator_data_.getf(i, CsvPosition + 2));
		stage_data_.emplace(Fase, EnemyStruct(Name, Pos));
	}
}

void EnemyGenerator::read_fase_point()
{
	for (int i = 0; i < csv_generator_pos_.rows(); ++i)
	{
		fase_point_.emplace(
		csv_generator_pos_.geti(i, CsvFasePoint),
		csv_generator_pos_.geti(i, CsvFasePos));
	}
}