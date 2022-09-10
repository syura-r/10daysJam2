#include "HitStop.h"
unsigned int HitStop::frameCounter = 0;
unsigned int HitStop::stopFrameTime = 0;

bool HitStop::Stop()
{
    if (frameCounter >= stopFrameTime)
        return false;
    frameCounter++;
    return true;
}
