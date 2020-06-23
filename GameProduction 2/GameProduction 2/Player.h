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
	bool IsGrounded() { return m_grounded; }
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
	void SetState(int s);
	void StopX() { m_velX = 0.0; }
	void StopY() { m_velY = 0.0; }
	void SetAccelX(double a);
	void SetAccelY(double a);
	
};

#endif
