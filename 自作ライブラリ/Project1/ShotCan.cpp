#include "ShotCan.h"
#include "FBXManager.h"
#include "BoxCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"

ShotCan::ShotCan(const Vector3& pos, const Vector3& shotVec)
{
	name = typeid(*this).name();
	//ƒ‚ƒfƒ‹‚Ì¶¬
	Create(FBXManager::GetModel("can"));
	position = pos;
	scale = 0.2f;
	rotV = std::rand() % 30 * 0.1f;
	velocity = shotVec *0.6f;
	aliveCount = 0;
	BoxCollider* boxCollider = new BoxCollider();
	boxCollider->SetObject(this);
	boxCollider->SetScale(scale );
	SetCollider(boxCollider);
	collider->SetAttribute(COLLISION_ATTR_BULLET);
	collider->SetMove(true);

}

void ShotCan::Update()
{
	aliveCount++;
	if (aliveCount > 30)
		dead = true;
	rotation.z += rotV;
	position += velocity;
	BoxCollider* boxCollider = dynamic_cast<BoxCollider*>(collider);
	assert(boxCollider);
	if (CollisionManager::GetInstance()->CheckHitBox(*boxCollider, COLLISION_ATTR_ENEMYS, boxCollider))
	{
		dead = true;
	}

	Object::Update();
}

void ShotCan::Draw()
{
	Object::CustomDraw(true, true);
}

void ShotCan::DrawReady()
{
	pipelineName = "FBX";

}
