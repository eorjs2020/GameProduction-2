#include "GrapplingHook.h"
#include "Engine.h"
#include <algorithm>
#include <iostream>
#include "MathManager.h"
#include "EventManager.h"
#include "CollisionManager.h"

GrapplingHook::GrapplingHook(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, double dir, Player * a) 
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
	m_Player = a; 
}


void GrapplingHook::Update() 
{
	
	if (EVMA::MouseHeld(1))
	{
		if (m_Exist == false) {
			m_dst.x = m_Player->GetDstP()->x;
			m_dst.y = m_Player->GetDstP()->y;
			m_Exist = true;
			m_Player->SetGrav(0.0);
			m_destinationX = EVMA::GetMousePos().x;
			m_destinationY = EVMA::GetMousePos().y;
		}
		if (m_Exist == false) {
			double h = 1, o, a;
			double pA = MAMA::AngleBetweenPoints((m_dst.y - m_Player->GetDstP()->y),
				(m_dst.x - m_Player->GetDstP()->x));
			double pD = MAMA::Distance((m_Player->GetDstP()->x + m_Player->GetDstP()->w / 2.0f), (m_dst.x),
				(m_Player->GetDstP()->y + m_Player->GetDstP()->h / 2.0f), (m_dst.y));
			a = MAMA::SetDeltaX(pA, h);
			o = MAMA::SetDeltaY(pA, h);
		}
	}
	else if (EVMA::MouseReleased(1))
	{
		m_Exist = false;
	}
	
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
		if (MAMA::Distance((GetDstP()->x), (m_destinationX),
			(GetDstP()->y), (m_destinationY)) <= 1.0)
		{
			Stop();
		}
		else {
			double pA = MAMA::AngleBetweenPoints((m_destinationY - GetDstP()->y),
				(m_destinationX - GetDstP()->x));
			double pD = MAMA::Distance((GetDstP()->x + GetDstP()->w / 2.0f), (m_destinationX),
				(GetDstP()->y + GetDstP()->h / 2.0f), (m_destinationX));
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

void GrapplingHook::Collision()
{
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{

			if (Engine::Instance().GetLevel()[i][j]->IsObstacle() && COMA::AABBCheck(*this->GetDstP(), *Engine::Instance().GetLevel()[i][j]->GetDstP()))
			{
				if (this->GetDstP()->y + Engine::Instance().GetLevel()[i][j]->GetDstP()->h - (float)this->GetVelY() <= (float)Engine::Instance().GetLevel()[i][j]->GetDstP()->y)
				{ // Colliding top side of platform.
					this->Stop();
					this->SetY(Engine::Instance().GetLevel()[i][j]->GetDstP()->y - this->GetDstP()->h);
					
				}
				else if (this->GetDstP()->y - (float)this->GetVelY() >= Engine::Instance().GetLevel()[i][j]->GetDstP()->y + Engine::Instance().GetLevel()[i][j]->GetDstP()->h)
				{ // Colliding bottom side of platform.
					this->Stop();
					this->SetY(Engine::Instance().GetLevel()[i][j]->GetDstP()->y + Engine::Instance().GetLevel()[i][j]->GetDstP()->h);
				}
				else if (this->GetDstP()->x + this->GetDstP()->w - this->GetVelX() <= Engine::Instance().GetLevel()[i][j]->GetDstP()->x)
				{ // Collision from left.
					this->Stop(); // Stop the player from moving horizontally.
					this->SetX(Engine::Instance().GetLevel()[i][j]->GetDstP()->x - this->GetDstP()->w);
				}
				else if (this->GetDstP()->x - (float)this->GetVelX() >= Engine::Instance().GetLevel()[i][j]->GetDstP()->x + Engine::Instance().GetLevel()[i][j]->GetDstP()->w)
				{ // Colliding right side of platform.
					this->Stop();
					this->SetX(Engine::Instance().GetLevel()[i][j]->GetDstP()->x + Engine::Instance().GetLevel()[i][j]->GetDstP()->w);
				}
			}

		}
	}
}
