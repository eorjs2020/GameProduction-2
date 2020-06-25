#pragma once
#ifndef _STATES_H_
#define _STATES_H_
#define NUMPLATFORMS 5
#define ROWS 73
#define COLS 171

#include <iostream>
#include <SDL.h>
#include <array>
#include <map>
#include "Button.h"
#include "PlatformPlayer.h"
#include "PlatformEnemy.h"
#include "GrapplingHook.h"
#include "Label.h"
#include "Tile.h"
#include "Player.h"

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

class GameState : public State
{
private:


	
	Player* m_pPlayer;
	bool existHook = false, hookColl = false; //check if evnet happened to render object, check if object is colliding for player to move
	int m_pSFXVolume, m_pMusicVolume;
	int m_pSFXSetVol = 30, m_pMusicSetVol = 16;
public:
	GameState();
	void Update();
	void CheckCollision();
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