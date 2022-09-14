#include "BossHair.h"
#include "FBXManager.h"
#include "BoxCollider.h"
#include "CollisionAttribute.h"
#include "Audio.h"

BossHair::BossHair(const Vector3& pos, const int arg_deadTime)
{
	Create(FBXManager::GetModel("BossHair"));

	position = pos;
	scale = 1.2f;
	BoxCollider* boxCollider = new BoxCollider();
	boxCollider->SetObject(this);
	boxCollider->SetScale(scale * 0.9f);
	SetCollider(boxCollider);
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
	collider->SetMove(true);

	velocity.y = 0.2f;
	aceel.y = -velocity.y / (arg_deadTime / 4);
	velocity.x = -0.5f;
	aceel.x = -velocity.x / (arg_deadTime / 2);
	deadCounter = 0;
	deadTime = arg_deadTime;
}
void BossHair::Update()
{
	Audio::PlaySE("boomerang", 0.1f * Audio::volume_se,true);
	rotation.z += rotVel;
	position += velocity;
	velocity += aceel;
	if (deadCounter == deadTime/2)
		aceel.y *= -1.0f;
	deadCounter++;
	if (deadCounter > deadTime)
	{
		dead = true;
	}
	Object::Update();
}

void BossHair::Draw()
{
	Object::CustomDraw(true, true);
}

void BossHair::DrawReady()
{
	pipelineName = "FBX";
}
