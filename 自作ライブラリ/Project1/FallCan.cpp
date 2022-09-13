#include "FallCan.h"
#include "Easing.h"
#include"FBXModel.h"
#include "FBXManager.h"
FallCan::FallCan(const Vector3& pos)
{
	myModel = FBXManager::GetModel("can");
	name = typeid(*this).name();
	//ƒ‚ƒfƒ‹‚Ì¶¬
	Create(myModel);
	position = pos;
	scale = 0.1f;
	rotV *= std::rand() % 30 * 0.01f;
	velocity.x = (std::rand() % 200 - 100) * 0.001f;
	velocity.y = (std::rand() % 90) * 0.001f + 0.2f;
	velocity.z = -0.02f;
	aliveCount = 0;
}

void FallCan::Update()
{
	aliveCount++;
	//if (aliveCount > 15)
	//	color.w = Easing::EaseInQuint(1, 0, 15, aliveCount - 15);
	if (aliveCount > 30)
		dead = true;

	position += velocity;
	velocity.y -= fallV;
	Object::Update();
}

void FallCan::Draw()
{
	Object::CustomDraw(true, true);
}

void FallCan::DrawReady()
{
	pipelineName = "FBX";
}
