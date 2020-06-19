#pragma once
#include "EventManager.h"
#include "Sprite.h"
#include "SDL.h"

class Skill
{
protected:
	Skill() {}

public:
	virtual int Update() = 0; 
	virtual void Render();
	virtual enum control {};
	virtual void Execute() = 0;
};

class Invis :public Skill 
{
private:
public:
	Invis();
	~Invis();
	int Update();
	void Render();
	enum control {};
	void Execute();
};


class SpeedBoost :public Skill
{
private:
public:
	SpeedBoost();
	~SpeedBoost();
	int Update();
	void Render();
	enum control {};
	void Execute();
};
