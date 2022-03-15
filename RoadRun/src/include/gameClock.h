#pragma once

#include "SDL2/SDL.h"

struct gameClock
{
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;

    double start = 0;
    double current = 0;

    long long int tick = 0;

    double fixedTick = 0;

    double d_multiply = 1;

    void update()
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = (((double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency())) / 1000) * d_multiply;

        current = SDL_GetTicks() - start;

        tick += (NOW - LAST);
        fixedTick += (NOW - LAST) * deltaTime;
    }

    void resetCounter()
    {
        start = SDL_GetTicks();
    }

    void set_deltaTime_count(double multiplyer)
    {
        d_multiply = multiplyer;
    }
};