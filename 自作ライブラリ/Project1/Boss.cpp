#include "Boss.h"
#include "FbxLoader.h"
#include "FBXModel.h"
#include "BoxCollider.h"
#include "CollisionAttribute.h"
#include "FBXManager.h"
#include "PtrDelete.h"
#include "Easing.h"
#include "RaycastHit.h"

#include "CollisionManager.h"
#include "ParticleEmitter.h"
#include "InGameCamera.h"
#include "Player.h"

Boss::Boss()
{
	mainModel = FbxLoader::GetInstance()->LoadModelFromFile("Tinpira");
	mainModel->AddAnimation("stand", 0, 0);
	mainModel->AddAnimation("walk", 1, 120);
	mainModel->AddAnimation("attack", 121, 180);
	headModel = FBXManager::GetModel("BossHead");
	bodyModel = FBXManager::GetModel("BossBody");
	position = { 7,-5,0 };
	name = typeid(*this).name();
	scale = 0.3f;
	rotation.y = -135;
	//モデルの生成
	Create(mainModel);
	BoxCollider* boxCollider = new BoxCollider();
	boxCollider->SetObject(this);
	boxCollider->SetOffset({ 0,0.65f,0,0 });
	boxCollider->SetScale(scale * Vector3{ 0.6f,2.25f,0.6f });
	SetCollider(boxCollider);
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
	collider->SetMove(true);
	name = typeid(*this).name();
	Initialize();
	Object::Update();
	actionState = ActionState::appear;
	magic = true;
	colorChangeCounter = 0;
	appearCounter = 0;
	camera = dynamic_cast<InGameCamera*>(Object3D::GetCamera());
}

Boss::~Boss()
{
	PtrDelete(mainModel);
}
void Boss::Initialize()
{
	actionState = ActionState::appear;
	appearState = AppearState::masgic;
	magic = true;
	earthquake = false;
	colorChangeCounter = 0;
	appearCounter = 0;
	attackCounter = 0;
	onGround = true;
	wallHit = false;
	rightRush = false;
	//scale = { 0.3f };
	//rotation.y = -90;
	prePos = position;
}

void Boss::Update()
{
	switch (actionState)
	{
	case ActionState::await:
		break;
	case ActionState::appear:
		Appear();
		break;
	case ActionState::attack:
		Attack();
		break;
	case ActionState::death:
		break;
	case ActionState::move:
		Move();
		break;

	default:
		break;
	}
	CheckHit();
	Object::Update();
}

void Boss::Draw()
{
	Object::CustomDraw(true, true);
}

void Boss::DrawReady()
{
	pipelineName = "FBX";
}

void Boss::OnCollision(const CollisionInfo& info)
{
}

void Boss::StartApper()
{
}

void Boss::StartMagic()
{
	magic = true;
}

void Boss::EndMagic()
{
}

void Boss::Move()
{
}

void Boss::Appear()
{
	const float bigScale = 1.2f;
	switch (appearState)
	{
	case AppearState::masgic:
	{
		ChangeColor();
		break;
	}
	case AppearState::scaleUp:
	{
		appearCounter++;
		if (appearCounter <= 90)
		{
			scale = Easing::EaseInOutBack(0.3f, bigScale, 90, appearCounter);
			BoxCollider* boxCollider = dynamic_cast<BoxCollider*>(collider);
			assert(boxCollider);
			boxCollider->SetOffset({ 0,0.65f * scale.x / 0.3f,0,0 });
			boxCollider->SetScale(scale * Vector3{ 0.6f,2.25f,0.6f });
			if (appearCounter == 90)
			{
				preColor = color;
				magic = false;
			}
		}
		if (appearCounter >= 90)
		{
			color.x = Easing::EaseInQuint(preColor.x, 1, 10, appearCounter - 90);
			color.y = Easing::EaseInQuint(preColor.y, 1, 10, appearCounter - 90);
			color.z = Easing::EaseInQuint(preColor.z, 1, 10, appearCounter - 90);
			if (appearCounter == 100)
			{
				color = { 1,1,1,1 };
				appearState = AppearState::jump;
				appearCounter = 0;
			}
		}
		break;
	}
	case AppearState::jump:
	{
		appearCounter++;
		const int EaingTime = 20;
		if (appearCounter <= EaingTime)
		{
			scale.x = Easing::EaseOutExpo(bigScale, bigScale * 2, EaingTime, appearCounter);
			scale.z = Easing::EaseOutExpo(bigScale, bigScale * 2, EaingTime, appearCounter);
			scale.y = Easing::EaseOutExpo(bigScale, bigScale * 0.5f, EaingTime, appearCounter);

		}
		else
		{
			scale.x = Easing::EaseOutExpo(bigScale * 2, bigScale, EaingTime, appearCounter - EaingTime);
			scale.z = Easing::EaseOutExpo(bigScale * 2, bigScale, EaingTime, appearCounter - EaingTime);
			scale.y = Easing::EaseOutExpo(bigScale * 0.5f, bigScale, EaingTime, appearCounter - EaingTime);

		}
		if (appearCounter >= EaingTime * 1.5f)
		{
			//ジャンプ時上向き初速
			auto jumpVYFist = 0.5f * 0.4f;
			//下向き加速
			fallAcc = -0.02f * 0.4f;

			fallV = { 0,jumpVYFist,0,0 };
			ParticleEmitter::CreateShock(position + Vector3{ 0, -0.5f, 0 });
			onGround = false;

			appearState = AppearState::earthquake;
		}
		break;
	}
	case AppearState::earthquake:
	{
		if (!earthquake)
		{
			const int EaingTime = 20;

			if (appearCounter >= EaingTime * 1.5f && appearCounter <= EaingTime * 2)
			{
				scale.x = Easing::EaseOutExpo(bigScale * 2, bigScale, EaingTime, appearCounter - EaingTime);
				scale.z = Easing::EaseOutExpo(bigScale * 2, bigScale, EaingTime, appearCounter - EaingTime);
				scale.y = Easing::EaseOutExpo(bigScale * 0.5f, bigScale, EaingTime, appearCounter - EaingTime);
				appearCounter++;
			}
			else
			{
				appearCounter = 0;
			}
			break;
		}
		appearCounter++;
		if (appearCounter > 40)
		{
			actionState = ActionState::attack;
			attackState = AttackState::rush;
			//mainModel->PlayAnimation("walk", true, 3, false);
			//if (appearCounter % 10 == 0)
			//{
			//	assert(camera);
			//	camera->SetShake(5, 0.05f);

			//}
		}
		

		break;
	}
	default:
		break;
	}

}

void Boss::Attack()
{
	switch (attackState)
	{
	case AttackState::rush:
	{
		attackCounter++;
		if (!wallHit)
		{
			mainModel->PlayAnimation("walk", true, 3, false);
			if (attackCounter % 10 == 0)
			{
				assert(camera);
				camera->SetShake(5, 0.03f);

			}
			if (attackCounter > 90)
			{
				if (!rightRush)
					position.x -= 0.2f;
				else
					position.x += 0.2f;
			}
		}
		else
		{

			onGround = false;
			if (attackCounter <= 15)
			{
				mainModel->PlayAnimation("stand", true, 1, false);
				rotation.x = Easing::EaseInOutBack(0, -90, 15, attackCounter);

			}
			if (attackCounter > 135 && attackCounter < 255)
			{
				mainModel->PlayAnimation("walk", true, 3, false);
			}
			else if (attackCounter >= 255 && attackCounter < 275)
			{
				if (attackCounter == 255)
				{
					//ジャンプ時上向き初速
					const float jumpVYFist = 0.08f;
					//下向き加速
					fallAcc = -jumpVYFist / 20;

					fallV = { 0,jumpVYFist,0,0 };

				}
				rotation.x = Easing::EaseInOutBack(-90, 0, 20, attackCounter - 255);
				if (!rightRush)
				{
					rotation.y = Easing::EaseInOutBack(-90, -270 + 45, 20, attackCounter - 255);
				}
				else
				{
					rotation.y = Easing::EaseInOutBack(-90-180, -135, 20, attackCounter - 255);
				}
			}
			if (attackCounter == 315)
			{
				wallHit = false;
				attackCounter = 0;
				if (!rightRush)
				{
					rightRush = true;
				}
				else
				{
					attackState = AttackState::jump;
					rightRush = false;
				}
			}
		}
		break;
	}
	case AttackState::beam:
	{
		break;
	}
	case AttackState::boomerang:
	{
		break;
	}
	case AttackState::jump:
	{
		const float bigScale = 1.2f;
		attackCounter++;
		const int EaingTime = 40;
		if (attackCounter <= EaingTime)
		{
			scale.x = Easing::EaseOutExpo(bigScale, bigScale * 2, EaingTime, attackCounter);
			scale.z = Easing::EaseOutExpo(bigScale, bigScale * 2, EaingTime, attackCounter);
			scale.y = Easing::EaseOutExpo(bigScale, bigScale * 0.5f, EaingTime, attackCounter);

		}
		else if(attackCounter <= EaingTime * 2)
		{
			scale.x = Easing::EaseOutExpo(bigScale * 2, bigScale, EaingTime, attackCounter - EaingTime);
			scale.z = Easing::EaseOutExpo(bigScale * 2, bigScale, EaingTime, attackCounter - EaingTime);
			scale.y = Easing::EaseOutExpo(bigScale * 0.5f, bigScale, EaingTime, attackCounter - EaingTime);

		}
		if (attackCounter == EaingTime * 1.5f)
		{
			//ジャンプ時上向き初速
			auto jumpVYFist = 0.5f * 4.0f;
			//下向き加速
			fallAcc = -0.02f * 4.0f;

			fallV = { 0,jumpVYFist,0,0 };
			ParticleEmitter::CreateShock(position + Vector3{ 0, -0.5f, 0 });
			onGround = false;
		}
		if (attackCounter > EaingTime * 2 && attackCounter < EaingTime * 2 + 180)
		{
			fallV = {};
			fallAcc = 0;

			if (attackCounter < EaingTime * 2 + 90)
				position.x = player->GetPosition().x;

		}
		if (attackCounter == EaingTime * 2 + 180)
		{
			fallAcc = -0.02f * 4.0f;
			rotation.y = -180;
			onGround = false;
		}

		if (attackCounter > EaingTime * 2 + 180)
		{
			if (onGround)
			{
				attackCounter = 0;
				if (jumpCounter < 2)
				{
					jumpCounter++;
				}
				else
				{
					attackState = AttackState::beam;
					rotation.y = -135;
					jumpCounter = 0;
				}
			}
		}
		break;
	}

	default:
		break;
	}
}

void Boss::ChangeColor()
{
	//  R   G   B
// 230  0   18
// 243 152  0
// 255 241  0
//  0  153  68
//  0  104 183
//  29  32 136
// 146  7  131
	if (colorChangeCounter < 10)
	{
		//白→赤 // 230  0   18
		color.x = Easing::EaseInQuint(1, 230.0f / 255.0f, 10, colorChangeCounter);
		color.y = Easing::EaseInQuint(1, 0, 10, colorChangeCounter);
		color.z = Easing::EaseInQuint(1, 18.0f / 255.0f, 10, colorChangeCounter);
	}
	else if (colorChangeCounter < 20)
	{
		//赤→橙 // 243 152  0
		color.x = Easing::EaseInQuint(230.0f / 255.0f, 243.0f / 255.0f, 10, colorChangeCounter - 10);
		color.y = Easing::EaseInQuint(0, 152.0f / 255.0f, 10, colorChangeCounter - 10);
		color.z = Easing::EaseInQuint(18.0f / 255.0f, 0, 10, colorChangeCounter - 10);
	}
	else if (colorChangeCounter < 30)
	{
		//橙→黄	// 255 241  0
		color.x = Easing::EaseInQuint(243.0f / 255.0f, 255.0f / 255.0f, 10, colorChangeCounter - 20);
		color.y = Easing::EaseInQuint(152.0f / 255.0f, 241.0f / 255.0f, 10, colorChangeCounter - 20);
		color.z = Easing::EaseInQuint(0.0f / 255.0f, 0.0f / 255.0f, 10, colorChangeCounter - 20);
	}
	else if (colorChangeCounter < 40)
	{
		//黄→緑 //  0  153  68
		color.x = Easing::EaseInQuint(255.0f / 255.0f, 0.0f / 255.0f, 10, colorChangeCounter - 30);
		color.y = Easing::EaseInQuint(241.0f / 255.0f, 153.0f / 255.0f, 10, colorChangeCounter - 30);
		color.z = Easing::EaseInQuint(0.0f / 255.0f, 68.0f / 255.0f, 10, colorChangeCounter - 30);
	}
	else if (colorChangeCounter < 50)
	{
		//緑→青 //  0  104 183
		color.x = Easing::EaseInQuint(0.0f / 255.0f, 0.0f / 255.0f, 10, colorChangeCounter - 40);
		color.y = Easing::EaseInQuint(153.0f / 255.0f, 104.0f / 255.0f, 10, colorChangeCounter - 40);
		color.z = Easing::EaseInQuint(68.0f / 255.0f, 183.0f / 255.0f, 10, colorChangeCounter - 40);
	}
	else if (colorChangeCounter < 60)
	{
		//青→シアン // 29  32 136
		color.x = Easing::EaseInQuint(0.0f / 255.0f, 29.0f / 255.0f, 10, colorChangeCounter - 50);
		color.y = Easing::EaseInQuint(104.0f / 255.0f, 32.0f / 255.0f, 10, colorChangeCounter - 50);
		color.z = Easing::EaseInQuint(183.0f / 255.0f, 136.0f / 255.0f, 10, colorChangeCounter - 50);
	}
	else if (colorChangeCounter < 70)
	{
		//シアン→紫 // 146  7  131
		color.x = Easing::EaseInQuint(29.0f / 255.0f, 146.0f / 255.0f, 10, colorChangeCounter - 60);
		color.y = Easing::EaseInQuint(32.0f / 255.0f, 7.0f / 255.0f, 10, colorChangeCounter - 60);
		color.z = Easing::EaseInQuint(136.0f / 255.0f, 131.0f / 255.0f, 10, colorChangeCounter - 60);
	}
	else if (colorChangeCounter < 80)
	{
		//紫→赤 // 230  0   18
		color.x = Easing::EaseInQuint(146.0f / 255.0f, 230.0f / 255.0f, 10, colorChangeCounter - 70);
		color.y = Easing::EaseInQuint(7.0f / 255.0f, 0.0f / 255.0f, 10, colorChangeCounter - 70);
		color.z = Easing::EaseInQuint(131.0f / 255.0f, 18.0f / 255.0f, 10, colorChangeCounter - 70);
	}

	colorChangeCounter++;
	if (colorChangeCounter >= 80)
	{
		colorChangeCounter = 10;
		appearState = AppearState::scaleUp;
	}

}

void Boss::CheckHit()
{
	if (!onGround)
	{
		//加速
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		//移動
		position.x += fallV.m128_f32[0];
		position.y += fallV.m128_f32[1];
		position.z += fallV.m128_f32[2];
	}

	//ボックスコライダーを取得
	BoxCollider* boxCollider = dynamic_cast<BoxCollider*>(collider);
	assert(boxCollider);

	//コライダー更新
	Object::Update();


	float dounDirSize = boxCollider->GetScale().y;
	if(rotation.x == -90)
		dounDirSize = boxCollider->GetScale().z;
	//ボックスの上端から球の下端までのレイキャスト用レイを準備
	Ray downRay;
	downRay.start = boxCollider->center;
	downRay.start.m128_f32[1] += dounDirSize;
	downRay.dir = { 0,-1,0,0 };
	RaycastHit downRayCastHit;

	//接地状態
	if (onGround)
	{
		//スムーズに坂を下る為の吸着距離
		const float absDistance = 0.2f;
		//接地を維持
		if (CollisionManager::GetInstance()->Raycast(downRay, boxCollider, COLLISION_ATTR_LANDSHAPE,
			&downRayCastHit, dounDirSize * 2.0f + absDistance))
		{
			onGround = true;
			position.y -= (downRayCastHit.distance - dounDirSize * 2.0f);
			//行列更新など
			Object::Update();
		}
		//地面がないので落下
		else {
			onGround = false;
			fallV = {};
		}
	}
	//落下状態
	else if (fallV.m128_f32[1] <= 0.0f)
	{
		if (attackState == AttackState::jump && position.y > 0 && actionState == ActionState::attack)
		{

		}
		else
		{
			if (CollisionManager::GetInstance()->Raycast(downRay, boxCollider, COLLISION_ATTR_LANDSHAPE,
				&downRayCastHit, dounDirSize * 2.0f))
			{
				//着地
				onGround = true;
				position.y -= (downRayCastHit.distance - dounDirSize * 2.0f);
				//行列更新など
				Object::Update();
				if (appearState == AppearState::earthquake && !earthquake)
				{
					earthquake = true;
					assert(camera);
					camera->SetShake(20, 0.1f);
					ParticleEmitter::CreateBossAppearShock(position + Vector3{ 0,0.65f * scale.y / 0.3f,0 });
				}
				//if (fallV.m128_f32[1] <= fallVYMin + 0.3f)
				//	ParticleEmitter::CreateJumpDust(position);

			}
		}
	}
	class PlayerQueryCallBack :public QueryCallback
	{
	public:
		PlayerQueryCallBack(Box* box) :box(box) {};

		//衝突時のコールバック関数
		bool OnQueryHit(const QueryHit& info)override
		{
			box->center += info.reject;
			move += info.reject;
			collider = info.collider;
			//}
			return true;
		}
		//クエリーに使用するボックス
		Box* box = nullptr;
		//衝突相手のコライダー
		BaseCollider* collider = nullptr;
		//排斥による移動量(累積値)
		DirectX::XMVECTOR move = {};
	};

	//クエリーコールバックの関数オブジェクト
	PlayerQueryCallBack callback(boxCollider);
	//callback.OnQueryHit(info);
	//地形との交差を全検索
	CollisionManager::GetInstance()->QueryBox(*boxCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	Vector3 rejectVec = callback.move;
	rejectVec.Normalize();
	//交差による排斥文を動かす
	if (attackState == AttackState::jump && position.y > 0 && actionState == ActionState::attack)
	{

	}
	else
	{

		position.x += callback.move.m128_f32[0];
		position.y += callback.move.m128_f32[1];
		position.z += callback.move.m128_f32[2];

		if (callback.move.m128_f32[1] < 0 && fallV.m128_f32[1]>0)
			fallV.m128_f32[1] = 0;
	}
	if (abs(callback.move.m128_f32[0]) > 0.01f)
	{
		if (attackState == AttackState::rush && actionState == ActionState::attack)
		{
			wallHit = true;
			camera->SetShake(15, 0.3f);
			attackCounter = 0;
			if(!rightRush)
				rotation = { 0,-90,0 };
			else
				rotation = { 0,-90 - 180,0 };
		}
		//velocity *= -1;
		//rotation.y *= -1;
	}

}
