#pragma once
#include<DirectXMath.h>

class CircleShadow
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public://バッファ用データ
	struct ConstBuff
	{
		XMVECTOR dir;
		XMFLOAT3 casterpos;
		float distanceCasterLight;
		XMFLOAT3 atten;
		float pad;//パディング
		XMFLOAT2 factorAngleCos;
		unsigned int active;
		float pad2;//パディング
	};
public://メンバ関数
	inline void SetDir(const XMVECTOR& dir) { this->dir = DirectX::XMVector3Normalize(dir); }
	inline const XMVECTOR& GetDir() { return dir; }

	inline void SetCasterPos(const XMFLOAT3& pos) {
		this->casterPos = pos;
	}
	inline const Vector3& GetCasterPos() { return casterPos; }

	inline void SetDistanceCasterLight(float  distanceCasterLight) {
		this->distanceCasterLight = distanceCasterLight;
	}
	inline float GetDistanceCasterLight() { return distanceCasterLight; }

	inline void SetAtten(const Vector3& atten) {
		this->atten = atten;
	}
	inline const Vector3& GetAtten() { return atten; }

	inline void SetFactorAngle(const XMFLOAT2& factorAngle)
	{
		this->factorAngleCos.x = cosf(DirectX::XMConvertToRadians(factorAngle.x));
		this->factorAngleCos.y = cosf(DirectX::XMConvertToRadians(factorAngle.y));
	}
	inline const XMFLOAT2 GetFactorAngleCos() { return factorAngleCos; }

	inline void SetActive(bool active) { this->active = active; }
	inline bool IsActive() { return active; }

private://メンバ変数
	XMVECTOR dir = { 1,0,0,0 };//方向(単位ベクトル)
	float distanceCasterLight = 100.0f;//キャスターとライトの距離
	Vector3 casterPos = { 0,0,0 };//キャスター座標(ワールド座標系)
	Vector3 atten = { 0.5f,0.6f,0.0f };//ライトの距離減衰係数
	XMFLOAT2 factorAngleCos = { 0.2f,0.5f };//ライト減衰角度(開始角度、終了角度)
	bool active = false;



};