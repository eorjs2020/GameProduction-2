#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_
#define GRAV 4.0
#define JUMPFORCE 30.0

#include "Sprite.h"


class Player : public AnimatedSprite
{
public:
	Player(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf);
	void Update();
	void Render();
	double GetVelX();
	double GetVelY();
	void Collision();
	void SetGrav(double y);
	bool BGScorllX() { return m_bgScrollX; }
	bool BGScrollY() { return m_bgScrollY; }
	void AddAccelX(double a);	
	void SetMaxVel(double a);
private:
	int speedDowncount;
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
	bool m_bgScrollX = false, m_bgScrollY = false;
	double* m_a, * m_aMaxY;
private:
	void HandleCamera(float scroll, bool x = false);
	void UpdateAxis(bool ax, bool ay);
	bool IsGrounded();
	void Stop();
	void StopX();
	void StopY();
	void SetAccelX(double a);
	void SetAccelY(double a);
	void SetGrounded(bool g);
	void SetX(float y);
	void SetY(float y);
	void SetState(int s);
	

	
	
};

#endif
