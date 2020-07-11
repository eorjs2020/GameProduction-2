#pragma once
#include "Sprite.h"
#include "vec2.hpp"
#include "Player.h"
#ifndef _ENEMY_H_
#define _ENEMY_H_
class Enemy :public AnimatedSprite
{
public:
	Enemy (SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf, int boundery);
	void Update(float AccelX,float AccelY, bool x, bool bgscroll, Player* p);
	void Render();
	double GetVelX();
	double GetVelY();
	bool IsGrounded();
	
private:
	enum state{ idle, seeking, arrive} m_State;
	void Stop();
	void StopX();
	void StopY();
	void SetAccelX(double a);
	void SetAccelY(double a);
	void SetGrounded(bool g);
	void SetX(float y);
	void SetY(float y);
	void AI(SDL_Rect* a);
private:
	SDL_FRect m_pSBox;
	int chasingTimer, searchingDelay, m_slowCooldown = 0;
	bool m_dir;
	bool m_grounded;
	double m_accelX,
		m_accelY,
		m_velX,
		m_maxVelX,
		m_velY,
		m_maxVelY,
		m_drag,
		m_grav,
		dx, dy, boundary;
	bool searchingDir = false, m_invis = false;
	bool m_bgScrollX = false, m_bgScrollY = false, m_moving = false, m_playerslow = false;
public:
	glm::vec2 m_ePos;
	void setInvis(bool a);
};

#endif
