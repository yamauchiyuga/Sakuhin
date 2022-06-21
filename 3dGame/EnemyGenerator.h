#ifndef ENEMY_GENERATOR_H_ 
#define ENEMY_GENERATOR_H_

#include "CSVReader.h"
#include "Actor/Actor.h"

#include<utility>
#include<queue>
#include<string>

struct EnemyNP {
	std::string EnemyName;
	GSvector3 EnemyPos;
}; 

class EnemyGenerator :public Actor {
public:
	// �R���X�g���N�^
	EnemyGenerator(IWorld* world, const std::string& csv_stage_data, const std::string& csv_fase_pos);
	// �X�V
	virtual void update(float delta_time) override;
private:
	// �G�̐���
	void generate();
private:
	// �o���f�[�^
	CsvReader   csv_statge_data_;
	//
	CsvReader   csv_fase_pos_;
	//
	std::queue<std::pair<int, EnemyNP>> statge_data_;
	//
	std::vector<int> fase_pos_;
	//
	Actor* player_;
	//
	int next_fase_;
};

#endif
