#pragma once
#ifndef _POLICE_ENEMY_
#define _POLICE_ENEMY_
#include "Sprite.h"
#include "Player.h"
#include "glm.hpp"
#include <vector>
#include "Bullet.h"
class FireDrone : public AnimatedSprite
{
public:
	FireDrone(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf, std::vector<Bullet*>* bVec);
	void Update(float AccelX, float AccelY, bool x, bool bgscroll, Player* p);
	void Render();
	double GetVelX();
	double GetVelY();
	bool IsGrounded();

private:
	enum state { idle, fire } m_State;
	void StopX();
	void StopY();
	void SetAccelX(double a);
	void SetAccelY(double a);
	void SetGrounded(bool g);
	void SetX(float y);
	void SetY(float y);
	void AI(SDL_Rect* a);
	void LOS(Player* p);
	void LOSCheck(Player* p);
	void BulletCollision(Player* p);
private:
	int m_bulletTimer,
		m_timerMax;
	int chasingTimer, searchingDelay, m_slowCooldown = 0;
	bool m_dir;
	bool m_grounded, m_bullNull;
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
	bool m_bgScrollX = false, m_bgScrollY = false, m_moving = false, m_playerslow = false, HasLineofSight = false;
	std::vector<Bullet*>* m_pBulletVec;
public:
	SDL_FRect m_pSBox;
	glm::vec2 m_ePos;
	
};
#endif
