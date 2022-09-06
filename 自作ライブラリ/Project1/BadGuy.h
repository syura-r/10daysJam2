#pragma once
#include "Object.h"
class FBXModel;
class BadGuy :
    public Object
{
public:
	BadGuy(const Vector3& arg_pos);
	~BadGuy();
	void Initialize()override;
	void Update()override;
	void Draw() override;
	void DrawReady() override;
	void OnCollision(const CollisionInfo& info)override;
private:
	void CheckHit();

	//����������
	float fallAcc = -0.02f * 0.4f;
	//�����������x�̍ő�l
	const float fallVYMin = -0.5f;

	Vector3 StartPos;
	Vector3 prePos;
	//�����x�N�g��
	XMVECTOR fallV;
	//���݌����Ă����
	Vector3 direction = {-1,0,0};
	//�ڒn�t���O
	bool onGround = true;

	FBXModel* myModel = nullptr;

};

