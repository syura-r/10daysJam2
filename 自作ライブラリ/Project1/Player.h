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
class HitPointBar;
class Sprite3D;
class Player : public Object
{
public:
	Player(const Vector3& arg_pos);
	~Player();
	void Initialize()override;
	void Update()override;
	void Draw() override;
	void DrawReady() override;
	bool GetGoal() { return bossFightReady; }
	void EndPosition();
	void BossDead() { 
		endMove = true; 
		fightBoss = false;
	};
	void CanOperate() { canOperate = true; }
	bool GetFightBoss() { return fightBoss; }
	void Cure();
private:
	void EndMove();
	void CheckHit();
	void JumpScaleCluc();
	void KnockBack();
	void Damage();
	void StumbleCluc();
	void Shot();
	void ShotMove();
	void RotCluc();
	void BossFightReady();
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

	//�c��̃W�����v�ł����
	int restJump = MaxJumpCount;

	bool endMove = false;

	//��]���x
	float rotVel;
	//���݂̉�]�p�x
	float nowRot;
	//�]�񂾏�Ԃ�
	bool stumble;
	//�]��ł���N���オ��܂ł̃J�E���^�[
	int stumbleCounter;
	float stumbleVelX;
	float stumbleAccelX;

	bool canOperate = false;

	bool fightBoss = false;

	const Vector3 EndPos = { 69,-7,0 };
	//���ˊp�x
	Vector3 shotDir;
	bool shotMoveFlag;
	Vector3 shotVel;
	const float shotReactionVal = 0.2f;
	int shotCounter = 0;
	bool canShot = true;

	bool a = false;
	bool downKeyFrame = false;

	bool bossFightReady;

	bool cannotMoveRot;

	Vector3 StartPos;
	Vector3 StartScale;
	//�����x�N�g��
	XMVECTOR fallV;
	//���݌����Ă����
	Vector3 direction;
	//�ڒn�t���O
	bool onGround = true;
	//�W�����v�t���O
	bool jump = false;

	//�_���[�W�t���O
	bool damage = false;
	//�m�b�N�o�b�N�̍ۂɕ������͂𖳌��ɂ���t���O
	const float MaxNockBackVel = 0.06f;
	bool knockBack = false;
	float knockBackVel = MaxNockBackVel;
	int knockBackCounter;
	int damageCounter;
	//���G����
	const int MaxDamageTime = 120;
	//���n�A�W�����v�O�̃X�P�[���ω�
	bool changeOnGroundScale = false;
	//�W�����v�J�n�̃X�P�[���ω�
	bool changeJumpScale = false;
	int changeScaleCounter = 0;
	FBXModel* myModel = nullptr;

	HitPointBar* cansBar = nullptr;

	Sprite3D* auxiliaryLines = nullptr;

};

