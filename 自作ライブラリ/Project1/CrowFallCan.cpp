#include "CrowFallCan.h"
#include "FBXManager.h"
#include "BoxCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "Boss.h"
#include "ParticleEmitter.h"

CrowFallCan::CrowFallCan(const Vector3& pos, Boss* bossPtr)
{
	myModel = FBXManager::GetModel("can");
	name = typeid(*this).name();
	//ƒ‚ƒfƒ‹‚Ì¶¬
	Create(myModel);
	position = pos;
	scale = 0.2f;
	velocity.y = -0.05f;
	BoxCollider* boxCollider = new BoxCollider();
	boxCollider->SetObject(this);
	boxCollider->SetScale(scale);
	SetCollider(boxCollider);
	collider->SetAttribute(COLLISION_ATTR_ITEM);
	collider->SetMove(true);
	boss = bossPtr;
}

void CrowFallCan::Update()
{
	ParticleEmitter::CreateMagicEffect(position);

	rotation.z += rotVel;
	position += velocity;

	BoxCollider* boxCollider = dynamic_cast<BoxCollider*>(collider);
	assert(boxCollider);
	if (CollisionManager::GetInstance()->CheckHitBox(*boxCollider, COLLISION_ATTR_ENEMYS, boxCollider))
	{
		dead = true;
		boss->StartMagic();
	}
	Object::Update();
}
