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
#include "BossHair.h"
#include "HitPointBar.h"
#include "BossCrow.h"
#include "BossHead.h"

#include "Audio.h"
Boss::Boss()
{
	mainModel = FBXManager::GetModel("Boss");
	noMohikanModel = FBXManager::GetModel("BossNoHair");
	mohikanModel = FBXManager::GetModel("BossHair");
	headModel = FBXManager::GetModel("BossHead");
	bodyModel = FBXManager::GetModel("BossBody");
	StartPos = { StartPosX,-5,0 };
	position = StartPos;
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
	hpBar = new HitPointBar();
	Initialize();
	Object::Update();
	actionState = ActionState::appear;
	colorChangeCounter = 0;
	appearCounter = 0;
	camera = dynamic_cast<InGameCamera*>(Object3D::GetCamera());
	lockOnObj = new Object();
	lockOnObj->Create(FBXManager::GetModel("LockOn"));
	lockOnObj->SetColor({ 0.6f,0,0,1 });
	lockOnObj->SetScale(2.0f);
	flashTex = new Sprite();
	HRESULT result = DirectXLib::GetInstance()->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBuffData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	destruction = 0.0f;
	scaleFactor = 1.0f;
	positionFactor = 2.0f;
	rotationFactor = 0.79f;
	tessellation = 1;
	onEasing = false;
	StartApper();
}

Boss::~Boss()
{
	PtrDelete(lockOnObj);
	PtrDelete(hpBar);
	PtrDelete(flashTex);

}
void Boss::Initialize()
{
	playBreakAnimation = false;
	drawFlash = false;
	flashAlpha = 0;
	flashCounter = 0;
	actionState = ActionState::appear;
	appearState = AppearState::masgic;
	magic = false;
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
	hpBar->Initialize(MaxHP);
}

void Boss::Update()
{
	if (Input::TriggerKey(DIK_0))
	{
		actionState = ActionState::death;
		drawFlash = true;
	}
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
		Death();
		break;
	case ActionState::move:
		Move();
		break;

	default:
		break;
	}
	CheckHit();
	Damage();
	Object::Update();
	hpBar->Update(hp);
}

void Boss::Draw()
{
	if (attackState == AttackState::jump && actionState == ActionState::attack && attackCounter > 40 * 2 && !onGround)
	{
		lockOnObj->Object::CustomDraw(true);
	}
	if (breakModelDraw)
	{
		DirectXLib::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
		DirectXLib::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, constBuff->GetGPUVirtualAddress());

	}
	Object::CustomDraw(true, true);
}

void Boss::DrawReady()
{
	if (drawFlash)
	{
		flashTex->DrawSprite("white1x1", { 960,540 }, 0, { 1920,1080 }, { 1,1,1,flashAlpha });
	}
	ConstBuffData* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
	constMap->_Destruction = destruction;
	constMap->_ScaleFactor = scaleFactor;
	constMap->_PositionFactor = positionFactor;
	constMap->_RotationFactor = rotationFactor;
	constMap->_Tessellation = tessellation;
	constMap->_OnEasing = onEasing;


	constBuff->Unmap(0, nullptr);
	if(!breakModelDraw)
	pipelineName = "FBX";
	else
		pipelineName = "FBXPolygonBreak";

	hpBar->Draw_boss();

}

void Boss::OnCollision(const CollisionInfo& info)
{
	if (damage) return;
	if (info.collider->GetAttribute() == COLLISION_ATTR_ALLIES)
	{
		Vector3 rejectVec = info.reject;
		rejectVec.Normalize();
		if (rejectVec.Length() <= 0)
			return;
		if (abs(rejectVec.y) > abs(rejectVec.x))
		{
			hp -= DamageVal;
			damage = true;
			color.w = 0;
			Audio::PlaySE("trample", 0.4f * Audio::volume_se);
		}
	}
	if (info.collider->GetAttribute() == COLLISION_ATTR_BULLET)
	{
		hp -= DamageVal;
		damage = true;
		color.w = 0;
		Audio::PlaySE("trample", 0.4f * Audio::volume_se);
	}

	if (hp <= 0)
	{
		drawFlash = true;
		actionState = ActionState::death;
	}
}

void Boss::StartApper()
{
	ObjectManager::GetInstance()->Add(new BossCrow(this));
}

void Boss::StartMagic()
{
	magic = true;
	ParticleEmitter::CreateGetEffect(position + Vector3{0, 0.65f, 0});
}

void Boss::Break()
{
	Audio::PlaySE("Boss_dead", Audio::volume_se * 0.5f);
	playBreakAnimation = true;
	camera->SetShake(120, 0.05f);
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
			ChangeColor();
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
			attackState = AttackState::boomerang;
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
				Audio::PlaySE("footsteps", 0.25f * Audio::volume_se);

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
			else if (attackCounter >= 255 && attackCounter < 295)
			{
				if (attackCounter == 255)
				{
					//ジャンプ時上向き初速
					const float jumpVYFist = 0.08f;
					//下向き加速
					fallAcc = -jumpVYFist / 20;

					fallV = { 0,jumpVYFist,0,0 };

				}
				rotation.x = Easing::EaseInOutBack(-90, 0, 40, attackCounter - 255);
				if (!rightRush)
				{
					rotation.y = Easing::EaseInOutBack(-90, -270 + 45, 40, attackCounter - 255);
				}
				else
				{
					rotation.y = Easing::EaseInOutBack(-90 - 180, -135, 40, attackCounter - 255);
				}
			}
			if (attackCounter == 335)
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
		attackCounter++;
		if (attackCounter < 114)
		{
			mainModel->PlayAnimation("attackReady", true, 1, false);
			if (attackCounter % 15 == 0)
			{
				assert(camera);
				camera->SetShake(5, 0.03f);
				Audio::PlaySE("footsteps", 0.25f * Audio::volume_se);

			}
		}
		else if (attackCounter <= 163)
		{
			mainModel->PlayAnimation("attack", false, 1, false);
			if (attackCounter == 163)
			{
				BossHair* bossHair = new BossHair(position + Vector3{ -0.1f, 1.15f , 0 }*scale.x / 0.3f, 120);
				ObjectManager::GetInstance()->Add(bossHair);
				object->SetModel(noMohikanModel);
			}
		}
		else if (attackCounter <= 173)
		{
			noMohikanModel->PlayAnimation("attack", false, 1, false);
		}
		else if (attackCounter >= 294 && attackCounter <= 300)
		{
			noMohikanModel->PlayAnimation("attackEnd", false, 1, false);
			if (attackCounter == 300)
			{
				object->SetModel(mainModel);
			}
		}
		else if (attackCounter >= 301 && attackCounter <= 317)
		{
			mainModel->PlayAnimation("attackEnd", false, 1, false);
		}
		else if (attackCounter == 318)
		{
			attackCounter = 0;
			attackState = AttackState::rush;
		}
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

			if (attackCounter < EaingTime * 2 + 120)
			{
				if (jumpCounter == 3)
					position.x = StartPosX;
				else
				position.x = player->GetPosition().x;
				lockOnObj->SetPosition({ position.x ,-7.5f,0});
				lockOnObj->Update();
			}

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
				camera->SetShake(20, 0.3f);
				Audio::PlaySE("earthquake", 0.4f * Audio::volume_se);
				if (jumpCounter < 3)
				{
					jumpCounter++;
				}
				else
				{
					attackState = AttackState::boomerang;
					rotation.y = -135;
					jumpCounter = 0;
					attackCounter = 0;
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
	if (!magic) return;
	if(appearState == AppearState::masgic)
	ParticleEmitter::CreateMagicEffect(position + Vector3{0, 0.65f, 0});
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
					Audio::PlaySE("earthquake", 0.4f * Audio::volume_se);
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
			Audio::PlaySE("earthquake", 0.4f * Audio::volume_se);
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

void Boss::Damage()
{
	if (!damage) return;
	damageCounter++;
	if (damageCounter == 5)
	{
		color.w = 1.0f;
	}
	if (damageCounter >= 15)
	{
		damageCounter = 0;
		damage = false;
	}

}

void Boss::Death()
{
	if (flashCounter < 81)
		flashCounter++;
	if (flashCounter <= 40)
		flashAlpha = Easing::EaseInSine(0, 1, 40, flashCounter);
	else if (flashCounter <= 80)
		flashAlpha = Easing::EaseOutSine(1, 0, 40, flashCounter - 40);
	if (flashCounter == 80)
	{
		drawFlash = false;
	}
	if (flashCounter == 40)
	{
		player->EndPosition();
		position = StartPos + Vector3{ 0,-2.475f,0 };
		rotation = { 0,-135,0 };
		scale = 1.2f;
		BoxCollider* boxCollider = dynamic_cast<BoxCollider*>(collider);
		assert(boxCollider);
		boxCollider->SetOffset({ 0,0.65f * scale.x / 0.3f,0,0 });
		boxCollider->SetScale(scale * Vector3{ 0.6f,2.25f,0.6f });
		object->SetModel(bodyModel);
		Object::Update();
		breakModelDraw = true;
		ObjectManager::GetInstance()->Add(new BossHead(position , this));

	}
	if (playBreakAnimation)
	{
		destruction += breakSpeed;
		if (destruction == 0.7f)
		{
			ParticleEmitter::CreateBossAppearShock(position + Vector3{ 0,0.65f * scale.y / 0.3f,0 });
		}
		if (destruction >= 1.0f)
		{
			player->BossDead();
			dead = true;
		}
		return;
	}


}
