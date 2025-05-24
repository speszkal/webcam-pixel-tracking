#include "../../dependencies.h"

Timer::Timer()
{
    startTick = 0;
    pauseTick = 0;
    
    isStarted = false;
    isPaused = false;
}

Timer::~Timer()
{
    
}

void Timer::start()
{
    startTick = SDL_GetTicks64();
    pauseTick = 0;

    isStarted = true;
    isPaused = false;
}

void Timer::stop()
{
    startTick = 0;
    pauseTick = 0;

    isStarted = false;
    isPaused = false;
}

void Timer::pause()
{
    if(isStarted && !isPaused)
    {
        pauseTick = SDL_GetTicks64() - startTick;

        startTick = 0;

        isPaused = true;
    }
}

void Timer::unpause()
{
    if(isStarted && isPaused)
    {
        startTick = SDL_GetTicks64() - pauseTick;

        pauseTick = 0;

        isPaused = false;
    }
}

Uint64 Timer::getTicks()
{
    Uint64 time = 0;
    
    if(isStarted) isPaused ? time=pauseTick : time=SDL_GetTicks64()-startTick;

    return time;
}