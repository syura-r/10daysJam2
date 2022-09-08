#pragma once
#include "BaseEnemy.h"
class BadGuy :
    public BaseEnemy
{
public:
	BadGuy(const Vector3& arg_pos);
	~BadGuy();
	void Initialize()override;
	void Update()override;
private:
	void CheckHit();

	//����������
	float fallAcc = -0.02f * 0.4f;
	//�����������x�̍ő�l
	const float fallVYMin = -0.5f;

	//�����x�N�g��
	XMVECTOR fallV;
	//���݌����Ă����
	Vector3 direction = {-1,0,0};
	//�ڒn�t���O
	bool onGround = true;


};

