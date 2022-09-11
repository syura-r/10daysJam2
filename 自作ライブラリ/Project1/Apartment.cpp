#include "Apartment.h"
#include "FBXManager.h"

Apartment::Apartment(const Vector3& arg_pos)
{
	position = arg_pos;
	scale = { 0.35f,0.3f,0.1f };
	rotation = { 0,0,0 };


	name = typeid(*this).name();

	Create(FBXManager::GetModel("Apartment"));
	pipelineName = "FBX";

}

void Apartment::Draw()
{
	Object::CustomDraw(true, true);
}
void Apartment::DrawReady()
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

