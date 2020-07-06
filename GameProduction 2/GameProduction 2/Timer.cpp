#include "Timer.h"
#include <iostream>


LTimer::LTimer()
{
	//Initialize the variables
	m_startTicks = 0;
	m_pausedTicks = 0;

	m_paused = false;
	m_started = false;
}

void LTimer::start()
{
	//Start the timer
	m_started = true;

	//Unpause the timer
	m_paused = false;

	//Get the current clock time
	m_startTicks = SDL_GetTicks();
	m_pausedTicks = 0;
}

void LTimer::stop()
{
	//Stop the timer
	m_started = false;

	//Unpause the timer
	m_paused = false;

	//Clear tick variables
	m_startTicks = 0;
	m_pausedTicks = 0;
}

void LTimer::pause()
{
	//If the timer is running and isn't already paused
	if (m_started && !m_paused)
	{
		//Pause the timer
		m_paused = true;

		//Calculate the paused ticks
		m_pausedTicks = SDL_GetTicks() - m_startTicks;
		m_startTicks = 0;
	}
}

void LTimer::unpause()
{
	//If the timer is running and paused
	if (m_started && m_paused)
	{
		//Unpause the timer
		m_paused = false;

		//Reset the starting ticks
		m_startTicks = SDL_GetTicks() - m_pausedTicks;

		//Reset the paused ticks
		m_pausedTicks = 0;
	}
}

Uint32 LTimer::getTicks()
{
	//The actual timer time
	Uint32 time = 0;

	//If the timer is running
	if (m_started)
	{
		//If the timer is paused
		if (m_paused)
		{
			//Return the number of ticks when the timer was paused
			time = m_pausedTicks;
		}
		else
		{
			//Return the current time minus the start time
			time = SDL_GetTicks() - m_startTicks;
		}
	}
	return time;
}

bool LTimer::isStarted()
{
	return m_started;
}

bool LTimer::isPaused()
{
	return m_paused && m_started;
}

std::string LTimer::getrunnningtime(LTimer a)
{
	m_seconds = a.getTicks() / 1000;
	
	if (m_seconds > 59)
		{
			m_minutes++;
			m_seconds = 0;
		}
	std::string m_a, m_b, m_c;
	m_a = std::to_string(m_seconds);
	m_b = std::to_string(m_minutes);
	m_c = m_b + ":" + m_a;
	return m_c;
}