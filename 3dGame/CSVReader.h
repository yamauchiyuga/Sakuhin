#ifndef CSV_READER_H_
#define CSV_READER_H_

#include <vector>
#include <string>

class CsvReader {
public:
    // �R���X�g���N�^
    CsvReader() = default;
    // �R���X�g���N�^
    CsvReader(const std::string& file_name);
    // �t�@�C���̓ǂݍ���
    void load(const std::string& file_name);
    // �f�[�^�̎擾(������Ŏ擾�j
    const std::string& get(int row, int column) const;
    // �f�[�^�̎擾�iint�^�ɕϊ����Ď擾�j
    int geti(int row, int column) const;
    // �f�[�^�̎擾�ifloat�^�ɕϊ����Ď擾�j
    float getf(int row, int column) const;
    // �s����Ԃ�
    int rows() const;
    // �񐔂�Ԃ�
    int columns(int row = 0) const;

private:
    // �P�s
    using Row = std::vector<std::string>;
    // �s�̔z��
    using Rows = std::vector<Row>;
    // CSV�f�[�^
    Rows rows_;
};

#endif