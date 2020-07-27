#pragma once
#include "EventManager.h"
#include "Sprite.h"
#include "SDL.h"

class SkillManager
{
protected:
	SkillManager() {}
public:
	void virtual Update() = 0;
	void virtual Render() = 0;

};

//New skills below

class SpeedBoost : public SkillManager {
public:
	SpeedBoost();
	void Update() {}
	void Update(double* a, double* b, double* c, float* x, float* y);
	void Render();
	bool getspeedBoost() { return m_speedBoost; }
private:
	AnimatedSprite* effect, *iconA;
	bool m_speedBoost = false;
	int m_speedBoostTimer = 0;
};
	
class invisibility : public SkillManager {
public:
	invisibility();
	void Update() {}
	void Update(double* c);
	void Render();
	bool getinvis() { return m_invis; }

private:
	bool m_invis = false;
	int m_invisTimer = 0;
	AnimatedSprite* effect, * iconB;
};