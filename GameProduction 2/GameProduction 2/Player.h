#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_
#define GRAV 6.0
#define JUMPFORCE 60.0

#include "Sprite.h"


class Player : public AnimatedSprite
{
public:
	Player(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf);
	void Update();
	void Render();
	double GetVelX();
	double GetVelY();
private:
	enum state { idle, running, jump } m_state;
	bool m_dir;
	bool m_grounded;
	double m_accelX,
		m_accelY,
		m_velX,
		m_maxVelX,
		m_velY,
		m_maxVelY,
		m_drag,
		m_grav;
private:
	void Collision();
	void SetState(int s);
	void Stop();
	void StopX();
	void StopY();
	void SetAccelX(double a);
	void SetAccelY(double a);
	bool IsGrounded();
	void SetGrounded(bool g);
	
	void SetX(float y);
	void SetY(float y);
	
};

#endif
