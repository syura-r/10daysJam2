#include "BossHead.h"
#include "FBXManager.h"
#include "Boss.h"

BossHead::BossHead(const Vector3& pos, Boss* bossPtr)
{
	StartPos = pos;
	rotation.y = -135;
	scale = 1.2;
	//アニメーション用にモデルのポインタを格納
	myModel = FBXManager::GetModel("BossHead");
	name = typeid(*this).name();
	//モデルの生成
	Create(myModel);
	boss = bossPtr;
	position = StartPos;
	prePos = position;
	velocity.x = -0.1f;
	aceel.x = 0.007f;
	aceel.y = -0.004f;
	Object::Update();
}

void BossHead::Update()
{
	moveCounter++;
	if (moveCounter < 40)
		return;
	prePos = position;
	//rotation.x += 5;
	if (velocity.x < 0)
	{
		position.x += velocity.x;
		velocity.x += aceel.x;
	}
	else
	{
		position.y += velocity.y;
		velocity.y += aceel.y;
	}
	if (position.y < -12 && prePos.y >= -12 && !bound)
	{
		velocity.y = 0.05;
		aceel.y = -0.006f;
		velocity.z = -0.1f;
		aceel.z = 0.005f;
		bound = true;
	}
	if (velocity.z < 0)
	{
		position.z += velocity.z;
		velocity.z += aceel.z;
	}
	if (position.y < -16)
	{
		boss->Break();
		dead = true;
	}

	Object::Update();
}
