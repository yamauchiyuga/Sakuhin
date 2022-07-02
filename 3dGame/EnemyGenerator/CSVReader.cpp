#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

// �R���X�g���N�^
CsvReader::CsvReader(const std::string& file_name) 
{
    load(file_name);
}


// �t�@�C���̓ǂݍ���
void CsvReader::load(const std::string& file_name)
{
    // �t�@�C���I�[�v��
    std::ifstream file{ file_name };
    if (!file) throw std::runtime_error("CSV�t�@�C�����I�[�v���ł��܂���ł���");
    rows_.clear();                     // �Â��f�[�^������
    std::string line;                  // ���s�����܂ł�1�s���̕�����
    while (std::getline(file, line))// ���s��؂�łP�s���ǂݍ���
    { 
        std::stringstream ss{ line };  // �P�s���f�[�^��stringstream�Ɋi�[
        std::string value;             // �e�Z���̃f�[�^
        Row row;                       // �P�s���̃f�[�^      
        while (std::getline(ss, value, ',')) // �J���}��؂�œǂݍ���
        { 
            row.push_back(value);      // �e�Z���̃f�[�^���s�ɒǉ�
        }
        rows_.push_back(row);          // 1�s���̃f�[�^��ǉ�k
    }
}


// �f�[�^�̎擾(������Ŏ擾�j
const std::string& CsvReader::get(int row, int column) const
{
    return rows_[row][column];
}


// �f�[�^�̎擾�iint�^�ɕϊ����Ď擾�j
int CsvReader::geti(int row, int column) const 
{
    return std::stoi(get(row, column));
}

// �f�[�^�̎擾�ifloat�^�ɕϊ����Ď擾�j
float CsvReader::getf(int row, int column) const 
{
    return std::stof(get(row, column));
}


// �s����Ԃ�
int CsvReader::rows() const
{
    return (int)rows_.size();
}


// �񐔂�Ԃ�
int CsvReader::columns(int row) const
{
    return (int)rows_[row].size();
}

