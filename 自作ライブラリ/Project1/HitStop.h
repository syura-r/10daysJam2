#pragma once
class HitStop
{
public:
	static bool Stop();
	static void SetStopTime(const unsigned int arg_stopFrame)
	{
		stopFrameTime = arg_stopFrame;
		frameCounter = 0;
	}
private:
	static unsigned int frameCounter;
	static unsigned int stopFrameTime;
};

