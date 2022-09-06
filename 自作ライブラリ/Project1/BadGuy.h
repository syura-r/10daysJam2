#pragma once
#include "Object.h"
class FBXModel;
class BadGuy :
    public Object
{
public:
	BadGuy(const Vector3& arg_pos);
	~BadGuy();
	void Initialize()override;
	void Update()override;
	void Draw() override;
	void DrawReady() override;
	void OnCollision(const CollisionInfo& info)override;
private:
	void CheckHit();

	//下向き加速
	float fallAcc = -0.02f * 0.4f;
	//下向き加速度の最大値
	const float fallVYMin = -0.5f;

	Vector3 StartPos;
	Vector3 prePos;
	//落下ベクトル
	XMVECTOR fallV;
	//現在向いてる方向
	Vector3 direction = {-1,0,0};
	//接地フラグ
	bool onGround = true;

	FBXModel* myModel = nullptr;

};

