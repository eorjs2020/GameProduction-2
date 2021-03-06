#pragma once
#ifndef _STATES_H_
#define _STATES_H_
#define NUMPLATFORMS 5
#define ddROWS 73
#define COLS 171
#define ROWS2 51
#define COLS2 419

#include <iostream>
#include <SDL.h>
#include <array>
#include <vector>
#include <map>
#include "Button.h"
#include "GrapplingHook.h"
#include "Label.h"
#include "Tile.h"
#include "Player.h"
#include "glm.hpp"
#include <array>
#include "Timer.h"
#include "FireDrone.h"
#include "Bullet.h"

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
	Sprite * m_interface, * m_battery[38];
	Label* m_timer, * m_energy;
	std::string m_TimerNum, m_defualtTimer = "Timer: 0", m_updateTimer, m_defualtEnergy = "Energy: 0", 
		m_updateEnergy, m_energyNum;
	Player* m_pPlayer;
	GrapplingHook* m_hook;
	Button* m_quit, * m_mainMenu, * m_resume, * m_pause;
	bool existHook = false, hookColl = false, m_stageEnd = false; //check if evnet happened to render object, check if object is colliding for player to move
	int m_pSFXVolume, m_pMusicVolume;
	int m_pSFXSetVol = 30, m_pMusicSetVol = 16, m_batteryArraySize = 38,
		m_batteryX[38] = {2, 4, 6, 21, 21, 21, 17, 12, 16, 24, 24, 24, 24, 23, 23, 23, 23, 24, 29,
			42, 42, 42, 42, 42, 42, 49, 46, 60, 33, 33, 33, 33, 43, 43, 42, 42, 68, 46}, 
		m_batteryY[38] = {21, 71, 132, 96, 97, 98, 116, 62, 98, 10, 11, 12, 13, 10, 11, 12, 13, 53, 35,
			37, 38, 39, 40, 41, 42, 3,  28, 90, 77, 78, 79, 80, 146, 147,146, 147, 140, 110};
	LTimer timer;
	int bulletTimer = 0, m_pNumBulletHit;
	bool bulletslow = false;
	bool m_batteryExist = true;
	bool m_bullNull;
	std::vector<FireDrone*> fDrone;
	std::vector<Bullet*> m_vEBullets;
	double m_destinationX, m_destinationY;
	
public:
	Level1State();
	void Update();
	void CheckCollisionHook();
	void Render();
	void Enter();
	void Exit();
	void Resume();
	void BulletCollision();
	bool LOS(int n);
	

};
class Level2State : public State
{
private:
	Player* m_pPlayer;
	GrapplingHook* m_hook;
	std::vector<FireDrone*> fDrone;
	std::vector<Bullet*> m_vEBullets;
	bool m_bullNull;
	double m_destinationX, m_destinationY;
	Sprite* m_battery[28];
	Sprite* m_goal;
	Button* m_quit, * m_mainMenu, * m_resume, * m_Pause;
	Label* m_timer, * m_energy;
	LTimer timer;
	std::string m_TimerNum, m_defualtTimer = "Timer: 0", m_updateTimer, m_defualtEnergy = "Energy: 0",
		m_updateEnergy, m_energyNum;
	bool existHook = false, hookColl = false, m_stageEnd = false;
	int m_pSFXVolume, m_pMusicVolume,
		m_batteryX[28] = { 48, 48, 43, 43, 42, 37, 28, 20, 20, 20, 17, 20, 
			23, 19, 19, 28, 28, 21, 10, 8, 22, 22, 22, 22, 22, 22, 22, 22},
		m_batteryY[28] = { 4, 5, 49, 50, 89, 92, 128, 101, 102, 103, 126, 164, 
			223, 257, 257, 304, 336, 343, 342, 382, 339, 339, 339, 339, 339, 339, 339, 339 };
	int m_pSFXSetVol = 30, m_pMusicSetVol = 16;
	int bulletTimer = 0, m_pNumBulletHit;
	bool bulletslow = false;
public:
	Level2State();
	void Update();
	void Render();
	void Enter();
	void Exit();
	void Resume();
	void BulletCollision();
	bool LOS(int n);
};
class TutorialState : public State
{
private:
	Sprite* explainKey, * m_battery[4];
	Sprite* m_goal;
	Sprite* m_interface;
	Button* m_quit, * m_mainMenu, * m_resume, * m_Pause;
	Label* m_timer, * m_energy;
	Label* tuto1, *tuto2, * tuto2b, *tuto3, *tuto4, * tuto4b, *tuto5, * tuto5b, *tuto6, 
		* tuto6b, *tuto5c, *tuto7, * tuto7b, *tuto8, *tuto8b;
	Label* m_input;
	Player* m_pPlayer;
	GrapplingHook* m_hook;
	LTimer timer;
	std::string m_TimerNum, m_defualtTimer = "Timer: 0", m_updateTimer, m_defualtEnergy = "Energy: 0",
		m_updateEnergy, m_energyNum;
	bool explainPause = true;
	int pressEnter = 0;
	bool explain = true;
	bool existHook = false, hookColl = false, m_stageEnd = false; 
	int m_pSFXVolume, m_pMusicVolume,
		m_batteryX[4] = { 13, 13, 13, 13},
		m_batteryY[4] = { 25, 25, 26, 26};
	int m_pSFXSetVol = 30, m_pMusicSetVol = 16;
	double m_destinationX, m_destinationY;

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
class ScoreState : public State
{
public:
	ScoreState();
	void Update();
	void Render();
	void Enter();
	void Exit();
private:
	Button* m_nextLevelBtn;
	Label* m_score;
	int totalScore, timeScore, energyScore, updatingtimer;
	string tS = "00000";
};

#endif