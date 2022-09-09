#pragma once
#include "Sprite.h"
#include "Vector.h"

class GameEndSelect
{
public:
	GameEndSelect();
	~GameEndSelect();

	void Initialize();
	void Update();
	void Draw();

	void IsActive();
	bool GetIsActive() { return activeFlag; }
	bool GetRestart() { return flag_restart; }
	bool GetToTitle() { return flag_toTitle; }

private:
	//�I������I�ԏ���
	void Select();
	//����{�^������������̏���
	void Decision();

	//�`���X�V�������s����
	bool activeFlag = false;

	enum SelectState
	{
		Restart = 0,
		ToTitle = 1,
	};
	SelectState selectState;
	//�I�����̌�
	const int selectMax = 2;

	//�Q�[����ʂ��Â�
	Sprite* sp_back = nullptr;
	Vector2 pos_back = {};

	//�I�������₷��
	Sprite* sp_base = nullptr;
	Vector2 pos_base = {};
	float alpha_base = 1.0f;
	bool isUP_alphaChange = false;

	//�I����1�ɕK�v�ȕϐ�
	struct SelectSprite_inGameEndSelect
	{
		SelectSprite_inGameEndSelect();
		~SelectSprite_inGameEndSelect();
		void Initialize(const std::string& texName, const float posY);
		void Update();
		void Draw();

		Sprite* sprite = nullptr;
		std::string texName = "";
		Vector2 pos = {};
	};


	//��蒼��
	SelectSprite_inGameEndSelect* restart = nullptr;
	bool flag_restart = false;

	//�^�C�g���ɂ��ǂ�
	SelectSprite_inGameEndSelect* toTitle = nullptr;
	bool flag_toTitle = false;
};

