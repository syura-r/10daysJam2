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
	const Vector3& GetMoveDistance() { return position - prePos; }
private:
	void CheckHit();
	void JumpScaleCluc();
	void KnockBack();
	void Damage();
	//ジャンプ補正値の最小値
	const float MinVal = 0.4f;
	//ジャンプ補正値の最大値
	const float MaxVal = 0.8f;
	//何回ジャンプしたら最大になるのか
	const int MaxJumpCount = 100;
	//ジャンプ補正値の上昇値
	const float valVel = (MaxVal - MinVal) / MaxJumpCount;
	//ジャンプ補正値
	float val = MinVal;
	//ジャンプ時上向き初速
	float jumpVYFist = 0.5f * val;
		//下向き加速
	float fallAcc = -0.02f * val;
	//下向き加速度の最大値
	const float fallVYMin = -0.5f;

	bool a = false;

	Vector3 StartPos;
	Vector3 StartScale;
	Vector3 prePos;
	//落下ベクトル
	XMVECTOR fallV;
	//現在向いてる方向
	Vector3 direction;
	//接地フラグ
	bool onGround = true;
	//ジャンプフラグ
	bool jump = false;

	//ダメージフラグ
	bool damage = false;
	//ノックバックの際に方向入力を無効にするフラグ
	const float MaxNockBackVel = 0.06f;
	bool knockBack = false;
	float knockBackVel = MaxNockBackVel;
	int knockBackCounter;
	int damageCounter;
	//無敵時間
	const int MaxDamageTime = 120;
	//着地、ジャンプ前のスケール変化
	bool changeOnGroundScale = false;
	//ジャンプ開始のスケール変化
	bool changeJumpScale = false;
	int changeScaleCounter = 0;
	FBXModel* myModel = nullptr;
};

