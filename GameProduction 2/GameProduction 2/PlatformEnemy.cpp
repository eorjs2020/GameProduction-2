#include "PlatformEnemy.h"
#include "Engine.h"
#include <algorithm>
#include <iostream>

PlatformEnemy::PlatformEnemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t)
	:Sprite(s, d, r, t)
{
	m_grounded = false;
	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	m_maxVelX = 10.0;
	m_maxVelY = JUMPFORCE;
	m_grav = GRAV;
	m_drag = 0.88;
}

void PlatformEnemy::Update()
{
	// Do X axis first.
	m_velX += m_accelX;
	m_velX *= (m_grounded ? m_drag : 1);
	m_velX = std::min(std::max(m_velX, -(m_maxVelX)), (m_maxVelX));
	m_dst.x += (int)m_velX; // Had to cast it to int to get crisp collision with side of platform.
	// Now do Y axis.
	m_velY += m_accelY + m_grav; // Adjust gravity to get slower jump.
	m_velY = std::min(std::max(m_velY, -(m_maxVelY)), (m_grav * 3));
	m_dst.y += m_velY; // If you run into issues with vertical collision, you can also cast to int.
	m_accelX = m_accelY = 0.0;
}

void PlatformEnemy::Render()
{
	//m_src.x = m_src.w * (int)m_state;
	SDL_RenderCopyF(m_pRend, m_pText, &m_src, &m_dst);

	/*SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 255, 255, 255);
	SDL_RenderFillRectF(Engine::Instance().GetRenderer(), GetDstP());*/
}

void PlatformEnemy::Stop() // If you want a dead stop both axes.
{
	StopX();
	StopY();
}
void PlatformEnemy::StopX() { m_velX = 0.0; }
void PlatformEnemy::StopY() { m_velY = 0.0; }

void PlatformEnemy::SetAccelX(double a) { m_accelX = a; }
void PlatformEnemy::SetAccelY(double a) { m_accelY = a; }
bool PlatformEnemy::IsGrounded() { return m_grounded; }
void PlatformEnemy::SetGrounded(bool g) { m_grounded = g; }
double PlatformEnemy::GetVelX() { return m_velX; }
double PlatformEnemy::GetVelY() { return m_velY; }
void PlatformEnemy::SetX(float y) { m_dst.x = y; }
void PlatformEnemy::SetY(float y) { m_dst.y = y; }

void PlatformEnemy::SetGrav(double y)
{
	m_grav = y;
}
