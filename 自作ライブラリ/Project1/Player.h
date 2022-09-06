#pragma once
#include <array>

#include "BoxCollider.h"
#include "DebugCamera.h"
#include "FBXModel.h"
#include "Object.h"
#include "ObjectManager.h"
#include "Timer.h"
#include "NumberSprite.h"

class StandardEnemy;
class EnergyItem;
class PanelItem;
class PanelCutLocus;

class Player : public Object
{
public:
	Player(const Vector3& arg_pos);
	~Player();
	void Initialize()override;
	void Update()override;
	void Draw() override;
	void DrawReady() override;
private:
	void CheckHit();
	void JumpScaleCluc();
	//�W�����v�␳�l�̍ŏ��l
	const float MinVal = 0.4f;
	//�W�����v�␳�l�̍ő�l
	const float MaxVal = 0.8f;
	//����W�����v������ő�ɂȂ�̂�
	const int MaxJumpCount = 100;
	//�W�����v�␳�l�̏㏸�l
	const float valVel = (MaxVal - MinVal) / MaxJumpCount;
	//�W�����v�␳�l
	float val = MinVal;
	//�W�����v�����������
	float jumpVYFist = 0.5f * val;
		//����������
	float fallAcc = -0.02f * val;
	//�����������x�̍ő�l
	const float fallVYMin = -0.5f;

	Vector3 StartPos;
	Vector3 prePos;
	//�����x�N�g��
	XMVECTOR fallV;
	//���݌����Ă����
	Vector3 direction;
	//�ڒn�t���O
	bool onGround = true;
	//�W�����v�t���O
	bool jump = false;

	//�ڒn�A�W�����v�O�̃X�P�[���ω�
	bool changeOnGroundScale = false;
	//�W�����v�J�n�̃X�P�[���ω�
	bool changeJumpScale = false;
	int changeScaleCounter = 0;
	FBXModel* myModel = nullptr;
};

