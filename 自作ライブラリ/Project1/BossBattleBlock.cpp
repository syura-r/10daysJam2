#include "BossBattleBlock.h"
#include "BoxCollider.h"
#include "CollisionAttribute.h"
#include "DrawMode.h"
#include "OBJLoader.h"
#include "Boss.h"


BossBattleBlock::BossBattleBlock(const Vector3& position, Boss* bossPtr)
{
	this->position = position;
	boss = bossPtr;
	name = typeid(*this).name();

	Create(OBJLoader::GetModel("box"));
	//コライダーの追加
	BoxCollider* collider = new BoxCollider;
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	collider->SetScale(scale / 2);
	collider->SetMove(false);
	SetCollider(collider);

}

void BossBattleBlock::Update()
{
	if (boss->IsDead())
		dead = true;
	Object::Update();
}
