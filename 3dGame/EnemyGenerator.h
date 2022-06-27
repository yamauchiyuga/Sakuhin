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
	// �R���X�g���N�^
	EnemyGenerator(std::shared_ptr<IWorld> world, const std::string& csv_stage_data, const std::string& csv_fase_pos);
	// �X�V
	virtual void update(float delta_time) override;
private:
	// �G�̐���
	void generate();
private:
	// �o���f�[�^
	CsvReader   csv_statge_data_;
	//�t�F�[�Y���Ƃ̐����ʒu
	CsvReader   csv_fase_pos_;
	//�X�e�[�W�f�[�^
	std::queue<std::pair<int, EnemyNP>> statge_data_;
	//
	std::vector<int> fase_pos_;
	//�v���C���[�擾
	std::shared_ptr<Actor> player_;
	//���̃t�F�[�Y
	int next_fase_;
};

#endif
