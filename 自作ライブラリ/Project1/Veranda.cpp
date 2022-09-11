#include "Veranda.h"
#include "BoxCollider.h"
#include "CollisionAttribute.h"
#include "DrawMode.h"
#include "FBXManager.h"

Veranda::Veranda(const Vector3& position, const Vector3& scale, const Vector3& rotation)
{
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;

	name = typeid(*this).name();

	Create(FBXManager::GetModel("Veranda"));
	//コライダーの追加
	BoxCollider* collider = new BoxCollider;
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	collider->SetScale(scale * Vector3{ 10,2.5f,5 });
	collider->SetMove(false);
	SetCollider(collider);
}

void Veranda::Draw()
{
#ifdef _DEBUG
	//if (DrawMode::GetDrawImGui())
	//{
	//	float rotate[3] = {};
	//	memcpy(rotate, &rotation, sizeof(float) * 3);
	//	ImGui::Begin("Box");
	//	ImGui::SliderFloat3("rotateXYZ", rotate, 0, 360);
	//	ImGui::End();
	//	memcpy(&rotation, rotate, sizeof(float) * 3);
	//	Update();
	//}
#endif

	Object::CustomDraw(true, true);
	//CustomDraw(false, true);
}
void Veranda::DrawReady()
{
	if (Object3D::GetDrawShadow())
	{
		pipelineName = "FBXShadowMap";
	}
	else
	{
		pipelineName = "FBX";
	}
}

