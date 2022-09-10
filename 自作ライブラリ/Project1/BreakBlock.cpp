#include "BreakBlock.h"
#include "OBJLoader.h"
#include "BoxCollider.h"
#include "CollisionAttribute.h"
#include "HitStop.h"

BreakBlock::BreakBlock(const Vector3& position, const Vector3& scale)
{
	this->position = position;
	this->scale = scale;

	name = typeid(*this).name();

	Create(OBJLoader::GetModel("box"));
	//コライダーの追加
	BoxCollider* collider = new BoxCollider;
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	collider->SetScale(scale / 2);
	collider->SetMove(false);
	SetCollider(collider);
}

void BreakBlock::OnCollision(const CollisionInfo& info)
{
	if (info.collider->GetAttribute() == COLLISION_ATTR_ALLIES)
	{
		Vector3 rejectVec = info.reject;
		rejectVec.Normalize();
		if (rejectVec.Length() <= 0)
			return;
		if (-rejectVec.y > abs(rejectVec.x) && info.object->GetMoveDistance().y > 0)
		{
			dead = true;
			HitStop::SetStopTime(5);
		}
	}

}
