#include "GrapplingHook.h"
#include "Engine.h"
#include <algorithm>
#include <iostream>
#include "MathManager.h"
#include "EventManager.h"

GrapplingHook::GrapplingHook(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, double dir) 
	:Sprite(s, d, r, t)
{
	direction = dir;
	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	m_maxVelX = 10.0;
	m_maxVelY = 10.0;
	m_grav = 0;
	m_drag = 0;
	m_grounded = false;
	m_Exist = false;
	m_Collision = false;
}

void GrapplingHook::Update()
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
	if (m_Exist == true)
	{
		double h = 1, o, a;
		if (MAMA::Distance((GetDstP()->x), (EVMA::GetMousePos().x),
			(GetDstP()->y), (EVMA::GetMousePos().x)) <= 1.0)
		{
			Stop();
		}
		else {
			double pA = MAMA::AngleBetweenPoints((EVMA::GetMousePos().y - GetDstP()->y),
				(EVMA::GetMousePos().x - GetDstP()->x));
			double pD = MAMA::Distance((GetDstP()->x + GetDstP()->w / 2.0f), (EVMA::GetMousePos().x),
				(GetDstP()->y + GetDstP()->h / 2.0f), (EVMA::GetMousePos().x));
			a = MAMA::SetDeltaX(pA, h);
			o = MAMA::SetDeltaY(pA, h);
			SetAccelX(a);
			SetAccelY(o);
		}
	}


}

void GrapplingHook::Render()
{
	SDL_RenderCopyF(m_pRend, m_pText, &m_src, &m_dst);
}

void GrapplingHook::Stop()
{
	m_accelX = 0.0;
	m_accelY = 0.0;
	m_velX = 0.0;
	m_velY = 0.0;
}

double GrapplingHook::GetVelX() { return m_velX; }
double GrapplingHook::GetVelY() { return m_velY; }
void GrapplingHook::SetAccelX(double a) { m_accelX = a; }
void GrapplingHook::SetAccelY(double a) { m_accelY = a; }
void GrapplingHook::SetX(float y) { m_dst.x = y; }
void GrapplingHook::SetY(float y) { m_dst.y = y; }

void GrapplingHook::SetExist(bool y)
{
	m_Exist = y;
}

bool GrapplingHook::GetExist()
{
	return m_Exist;
}
