#ifndef ST_H_
#define ST_H_

class ST
{
public:
	//�R���X�g���N�^
	ST(float max_stamina);
	//�X�V
	void update(float delta_time);
	//�`��
	void draw()const;
	//�X�^�~�i����̏���
	bool consumption_stamina(const int  consumption_stamina);
	//���݂̃X�^�~�i��Ԃ�
	int get_stamina()const;
private:
	//���݂̃X�^�~�i
	float cullent_stamina_;
	//�ő�X�^�~�i
	const float max_stamina_;

};

#endif // !ST_H_
