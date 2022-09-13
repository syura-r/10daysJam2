#pragma once
#include "TransitionItem.h"

class TransitionManager
{
public:
	TransitionManager();
	~TransitionManager();

	void Initialize();
	void Update();
	void Draw();

	//�V�[���J�ڊJ�n
	void IsAction();
	bool GetIsAction();

	//�V�[����ς���^�C�~���O�Ő^��Ԃ�
	bool GetChangeTime();

	//�V�[�����؂�ւ���ăJ�����̈ʒu���ς���Ă��Y���������悤�ɂ���
	void CameraChaser();

private:
	bool isAction = false;
	//�V�[���J�ڏI��
	void IsActionEnd();

	//////////

	Vector3 parentPosition = {};
	Vector3 prevParentPosition = {};

	static const int itemsCount = 18;
	TransitionItem* items[itemsCount];
	//�܂΂�ɔz�u�A��������
	bool ItemMapping();

	Vector3 cameraTarget = {};
};

