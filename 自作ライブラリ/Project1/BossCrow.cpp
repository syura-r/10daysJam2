#include "BossCrow.h"
#include "Audio.h"
#include "Boss.h"
#include "FBXManager.h"
#include "ObjectManager.h"
#include "CrowFallCan.h"

BossCrow::BossCrow(Boss* bossPtr)
{
	StartPos = { bossPtr->GetPosition().x + 6 ,2,0 };
	scale = 0.3f;
	color = { 1,0,0,1 };
	//アニメーション用にモデルのポインタを格納
	myModel = FBXManager::GetModel("crow");
	name = typeid(*this).name();
	//モデルの生成
	Create(myModel);
	Initialize();
	boss = bossPtr;
}

BossCrow::~BossCrow()
{
}

void BossCrow::Initialize()
{
	position = StartPos;
	prePos = position;
	isSeLoop = true;
	seIntervalCount = 0;
}

void BossCrow::Update()
{
	CryVoiceLoop();
	prePos = position;
	position.x -= 0.1f;
	myModel->PlayAnimation("move", true, 1, true);
	if (position.x <= boss->GetPosition().x && prePos.x > boss->GetPosition().x)
	{
		ObjectManager::GetInstance()->Add(new CrowFallCan(position, boss));
	}
	if (position.x < StartPos.x - 25)
		dead = true;
	Object::Update();
}

void BossCrow::CryVoiceLoop()
{
	if (!isSeLoop)
		return;

	const int seInterval = 120;

	if (seIntervalCount <= 0)
	{
		Audio::PlaySE("crow", 0.1f * Audio::volume_se);
		seIntervalCount = seInterval;
	}

	seIntervalCount--;

}
