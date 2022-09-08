#include "InGameTimer.h"
#include "Vector.h"

InGameTimer::InGameTimer()
{
	timer = new Timer(TimerPerformance::Up);

	stopwatch_sprite = new Sprite();
	seconds_sprite = new NumberSprite(seconds);
}

InGameTimer::~InGameTimer()
{
	delete timer;
	delete stopwatch_sprite;
	delete seconds_sprite;
}

void InGameTimer::Initialize()
{
	timer->Initialize();
	seconds = 0.0f;
}

void InGameTimer::Update()
{
	//�o�ߎ��ԁi�b�j
	seconds = timer->GetRealTime(TimerPerformance::Up);

	timer->Update();
}

void InGameTimer::Draw()
{
	//�摜�T�C�Y�i����1���j
	const Vector2 numberTexSize = { 47.0f, 86.0f };
	//�A�C�R���摜�T�C�Y
	const float stopwatchTexSizeX = 340.0f;
	const float stopwatchScale = 0.25f;

	//��ʃT�C�Y
	const Vector2 windowSize = { 1920.0f, 1080.0f };
	//�A�C�R�����W
	Vector2 position = { (windowSize.x / 2) - (stopwatchTexSizeX * stopwatchScale), windowSize.y / 8};
	//�A�C�R���`��
	stopwatch_sprite->DrawSprite("stopwatch", position, 0, { stopwatchScale,stopwatchScale }, { 1,1,1,1 }, { 0.0f,0.5f }, "NoAlphaToCoverageSprite");

	//�A�C�R���̑傫�������炷
	position.x += (stopwatchTexSizeX * stopwatchScale) + (numberTexSize.x / 2.0f);
	//�����`��
	seconds_sprite->Draw(std::to_string((int)seconds).size(), "number", position, { 1,1 }, { 1,1,1,1 }, { 0.0f,0.5f }, "NoAlphaToCoverageSprite");
}
