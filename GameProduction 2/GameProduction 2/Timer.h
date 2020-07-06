#pragma once
#include<ctime>
#include<cstdlib>
#include "Engine.h"

//code for Ltimer mainly from lazyfoo.net/tutorials/SDL/23_advanced_timers/index.php

class LTimer
{
public:
    std::string getrunnningtime();
    //Initializes variables
    LTimer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    Uint32 getTicks();

    //Checks the status of the timer
    bool isStarted();
    bool isPaused();

private:
    //The clock time when the timer started
    Uint32 m_startTicks;

    //The ticks stored when the timer was paused
    Uint32 m_pausedTicks;

    //The timer status
    bool m_paused;
    bool m_started;
    int m_seconds, m_minutes;
};

