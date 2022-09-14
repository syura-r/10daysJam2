#include "Goal.h"
#include "OBJLoader.h"
#include "BoxCollider.h"
#include "CollisionAttribute.h"

Goal::Goal(const Vector3& position, const Vector3& scale)
{
	this->position = position;
	this->scale = scale;

	name = typeid(*this).name();

	Create(OBJLoader::GetModel("box"));
	//コライダーの追加
	BoxCollider* collider = new BoxCollider;
	collider->SetAttribute(COLLISION_ATTR_GOAL);
	collider->SetScale(scale / 2);
	collider->SetMove(false);
	SetCollider(collider);
}
