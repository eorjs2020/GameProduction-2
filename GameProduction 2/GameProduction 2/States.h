#pragma once
#ifndef _STATES_H_
#define _STATES_H_
#define NUMPLATFORMS 5
#define ROWS 73
#define COLS 171
#define ROWS2 51
#define COLS2 419

#include <iostream>
#include <SDL.h>
#include <array>
#include <vector>
#include <map>
#include "Button.h"
#include "PlatformPlayer.h"
#include "PlatformEnemy.h"
#include "GrapplingHook.h"
#include "Label.h"
#include "Tile.h"
#include "Player.h"
#include "glm.hpp"
#include <array>
#include "Timer.h"
using namespace std;
class State // This is the abstract base class for all specific states.
{
protected:
	State() {}
public:
	virtual void Update() = 0; // Having at least one 'pure virtual' method like this, makes a class abtract.
	virtual void Render();     // Meaning we cannot create objects of the class.
	virtual void Enter() = 0;  // Virtual keyword means we can override in derived class.
	virtual void Exit() = 0;
	virtual void Resume();
};

class Level1State : public State
{
private:
	Sprite* m_goal;
	Sprite * m_interface, * m_battery;
	Label* m_timer, * m_energy;
std::string m_TimerNum, m_defualtTimer = "Timer: 0", m_updateTimer, m_defualtEnergy = "Energy: 0", 
		m_updateEnergy, m_energyNum;
	Player* m_pPlayer;
	GrapplingHook* m_hook;
	
	bool existHook = false, hookColl = false, m_stageEnd = false; //check if evnet happened to render object, check if object is colliding for player to move
	int m_pSFXVolume, m_pMusicVolume;
	int m_pSFXSetVol = 30, m_pMusicSetVol = 16;
	LTimer timer;
	bool m_batteryExist = true; 
	
public:
	Level1State();
	void Update();
	void CheckCollisionHook();
	void Render();
	void Enter();
	void Exit();
	void Resume();

};
class Level2State : public State
{
private:
	Player* m_pPlayer;
	GrapplingHook* m_hook;
public:
	Level2State();
	void Update();
	void Render();
	void Enter();
	void Exit();
	void Resume();
};
class TutorialState : public State
{
private:
	Sprite* explainKey;
	Sprite* m_goal;
	Sprite* m_interface;
	Button* m_MainMenu;
	Label* m_timer, * m_energy;
	Label* tuto1, *tuto2, *tuto3, *tuto4, *tuto5, *tuto6;
	Label* pressEnterL;
	Player* m_pPlayer;
	GrapplingHook* m_hook;
	bool explainPause = true;
	int pressEnter = 0;
	bool explain = true;
	bool existHook = false, hookColl = false, m_stageEnd = false; 
	int m_pSFXVolume, m_pMusicVolume;
	int m_pSFXSetVol = 30, m_pMusicSetVol = 16;
public:
	TutorialState();
	void Update();
	void CheckCollisionHook();
	void Render();
	void Enter();
	void Exit();
	void Resume();
};

class TitleState : public State
{
public:
	
	TitleState();
	void Update();
	void Render();
	void Enter();
	void Exit();
private:
	Button* m_playBtn;
	Button* m_quitBtn;
	Button* m_tuto;
	
		
};
class EndState : public State
{
public:
	EndState();
	void Update();
	void Render();
	void Enter();
	void Exit();
private:
	Button* m_playBtn;
};

#endif