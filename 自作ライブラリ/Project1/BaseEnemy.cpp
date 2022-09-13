#include "BaseEnemy.h"
#include "CollisionAttribute.h"
#include"BaseCollider.h"
Player* BaseEnemy::player = nullptr;

void BaseEnemy::OnCollision(const CollisionInfo& info)
{
	if (info.collider->GetAttribute() == COLLISION_ATTR_ALLIES)
	{
		Vector3 rejectVec = info.reject;
		rejectVec.Normalize();
		if (rejectVec.Length() <= 0)
			return;
		if (abs(rejectVec.y) > abs(rejectVec.x))
		{
			dead = true;
		}
	}
	if (info.collider->GetAttribute() == COLLISION_ATTR_BULLET)
	{
		dead = true;
	}
}


void BaseEnemy::Draw()
{
	Object::CustomDraw(true, true);
}

void BaseEnemy::DrawReady()
{
	pipelineName = "FBX";
}
