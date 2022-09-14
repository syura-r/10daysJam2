#include "BadGuy.h"
#include"FBXModel.h"
#include"FbxLoader.h"
#include"BoxCollider.h"
#include"CollisionAttribute.h"
#include "RaycastHit.h"
#include"CollisionManager.h"
#include"Player.h"
#include "PtrDelete.h"
#include "FBXManager.h"

BadGuy::BadGuy(const Vector3& arg_pos) 
{
	StartPos = arg_pos;
	//�A�j���[�V�����p�Ƀ��f���̃|�C���^���i�[
	myModel = FBXManager::GetModel("badGuy");

	name = typeid(*this).name();
	scale = 0.3f;
	//���f���̐���
	Create(myModel);
	BoxCollider* boxCollider = new BoxCollider();
	boxCollider->SetObject(this);
	boxCollider->SetOffset({ 0,0.60f,0,0 });
	boxCollider->SetScale(scale* Vector3{0.6f,2.25f,0.6f});
	SetCollider(boxCollider);
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
	collider->SetMove(true);
	name = typeid(*this).name();
	Initialize();
	Object::Update();

}

BadGuy::~BadGuy()
{
	//PtrDelete(myModel);
}

void BadGuy::Initialize()
{
	onGround = true;
	//scale = { 0.3f };
	position = StartPos;
	rotation.y = -90;
	prePos = position;
	direction = { 1,0,0 };
	velocity.x = -0.02f;
}

void BadGuy::Update()
{
	if (player == nullptr) return;
	if (Vector3(player->GetPosition() - position).Length() > 15)
		return;
	position.x += velocity.x;
		//��������
	if (!onGround)
	{
		//����
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		//�ړ�
		position.x += fallV.m128_f32[0];
		position.y += fallV.m128_f32[1];
		position.z += fallV.m128_f32[2];
	}
	if (position.y < -10)
		dead = true;
	CheckHit();
	myModel->PlayAnimation("walk", true, 2, true);

	Object::Update();
}

void BadGuy::CheckHit()
{
	//�{�b�N�X�R���C�_�[���擾
	BoxCollider* boxCollider = dynamic_cast<BoxCollider*>(collider);
	assert(boxCollider);

	//�R���C�_�[�X�V
	Object::Update();

	//�{�b�N�X�̏�[���狅�̉��[�܂ł̃��C�L���X�g�p���C������
	Ray downRay;
	downRay.start = boxCollider->center;
	downRay.start.m128_f32[1] += boxCollider->GetScale().y;
	downRay.dir = { 0,-1,0,0 };
	RaycastHit downRayCastHit;

	//�ڒn���
	if (onGround)
	{
		//�X���[�Y�ɍ������ׂ̋z������
		const float absDistance = 0.2f;
		//�ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(downRay, boxCollider, COLLISION_ATTR_LANDSHAPE,
			&downRayCastHit, boxCollider->GetScale().y * 2.0f + absDistance))
		{
			onGround = true;
			position.y -= (downRayCastHit.distance - boxCollider->GetScale().y * 2.0f);
			//�s��X�V�Ȃ�
			Object::Update();
		}
		//�n�ʂ��Ȃ��̂ŗ���
		else {
			onGround = false;
			fallV = {};
		}
	}
	//�������
	else if (fallV.m128_f32[1] <= 0.0f)
	{
		if (CollisionManager::GetInstance()->Raycast(downRay, boxCollider, COLLISION_ATTR_LANDSHAPE,
			&downRayCastHit, boxCollider->GetScale().y * 2.0f))
		{
			//���n
			onGround = true;
			position.y -= (downRayCastHit.distance - boxCollider->GetScale().y * 2.0f);
			//�s��X�V�Ȃ�
			Object::Update();

			//if (fallV.m128_f32[1] <= fallVYMin + 0.3f)
			//	ParticleEmitter::CreateJumpDust(position);

		}
	}
	class PlayerQueryCallBack :public QueryCallback
	{
	public:
		PlayerQueryCallBack(Box* box) :box(box) {};

		//�Փˎ��̃R�[���o�b�N�֐�
		bool OnQueryHit(const QueryHit& info)override
		{
			box->center += info.reject;
			move += info.reject;
			collider = info.collider;
			//}
			return true;
		}
		//�N�G���[�Ɏg�p����{�b�N�X
		Box* box = nullptr;
		//�Փˑ���̃R���C�_�[
		BaseCollider* collider = nullptr;
		//�r�˂ɂ��ړ���(�ݐϒl)
		DirectX::XMVECTOR move = {};
	};

	//�N�G���[�R�[���o�b�N�̊֐��I�u�W�F�N�g
	PlayerQueryCallBack callback(boxCollider);
	//callback.OnQueryHit(info);
	//�n�`�Ƃ̌�����S����
	CollisionManager::GetInstance()->QueryBox(*boxCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	Vector3 rejectVec = callback.move;
	rejectVec.Normalize();
	//�����ɂ��r�˕��𓮂���
	position.x += callback.move.m128_f32[0];
	position.y += callback.move.m128_f32[1];
	position.z += callback.move.m128_f32[2];

	if (callback.move.m128_f32[1] < 0 && fallV.m128_f32[1]>0)
		fallV.m128_f32[1] = 0;
	if (abs(callback.move.m128_f32[0]) > 0.01f)
	{
		velocity *= -1;
		rotation.y *= -1;
	}
}
