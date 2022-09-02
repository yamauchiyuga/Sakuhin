#ifndef ENEMY_GENERATOR_H_ 
#define ENEMY_GENERATOR_H_

#include "CSVReader.h"
#include "../Actor/Actor.h"
#include "../World/IWorld.h"
#include<utility>
#include<queue>
#include<string>
#include<memory>
#include <map>

struct EnemyStruct
{
	EnemyStruct(const std::string& name, const GSvector3& pos) :
		name_{name}, pos_{pos}{}
	const std::string name_;
	const GSvector3 pos_;
};

class EnemyGenerator :public Actor 
{
public:
	// �R���X�g���N�^
	EnemyGenerator(std::shared_ptr<IWorld> world, const std::string& data_name, const std::string& pos_name);
	// �X�V
	virtual void update(float delta_time) override;
private:
	// �G�̐���
	void load_csv(const std::string& data_name, const std::string& pos_name);
	void read_stage_data();
	void read_phase_point();

	template<typename T>
	inline void generate(std::shared_ptr<IWorld> world, const GSvector3& pos)
	{
		world->add_actor(std::make_unique<T>(world, pos));
	}

private:
	// �o���f�[�^CSV
	CsvReader   csv_generator_data_;
	//�t�F�[�Y���Ƃ̐����ʒuCSV
	CsvReader   csv_generator_pos_;

	// �X�e�[�W�f�[�^
	std::queue<std::pair<int, EnemyStruct>> stage_data_;
	// �t�F�[�Y�f�[�^
	std::queue<std::pair<int, int>> phase_point_;
};

#endif
