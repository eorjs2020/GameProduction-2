#pragma once
#ifndef _GRAPPLINGHOOK_H_
#define _GRAPPLINGHOOK_H_

#include "Sprite.h"
#include "Player.h"

class GrapplingHook : public Sprite
{
public:
	GrapplingHook(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, double dir, Player * a);
	void Update();
	void Render();
	void Stop();
	double GetVelX();
	double GetVelY();
	void SetAccelX(double a);
	void SetAccelY(double a);
	void SetX(float y);
	void SetY(float y);
	void SetExist(bool y);
	bool GetExist();
	void Collision(int stage = 1);
private:
	bool m_grounded, m_Exist = false, m_Collision;
	double m_lifetime, 
		m_accelX, 
		m_accelY, 
		direction,
		m_velX,
		m_maxVelX,
		m_velY,
		m_maxVelY,
		m_drag,
		m_grav;
	Player* m_Player;

};
#endif
