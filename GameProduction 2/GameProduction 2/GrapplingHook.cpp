#include "GrapplingHook.h"
#include "Engine.h"
#include <algorithm>
#include <iostream>
#include "MathManager.h"
#include "EventManager.h"
#include "CollisionManager.h"
#include "DebugManager.h"

GrapplingHook::GrapplingHook(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, double dir, Player * a) 
	:Sprite(s, d, r, t)
{
	dx = dy = 0.0f;
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
	m_destinationX = m_Player->GetDstP()->x;
	m_destinationY = m_Player->GetDstP()->y;
	grap = true;
}


void GrapplingHook::Update() 
{
	if (m_Player->BGScrollY())
	{
		m_destinationY -= m_Player->GetVelY();
		m_dst.y -= m_Player->GetVelY();
	}
	if (m_Player->BGScorllX())
	{
		m_destinationX -= m_Player->GetVelX();
		m_dst.x -= m_Player->GetVelX();
	}

	
	if (EVMA::MouseHeld(1))
	{
		m_destinationX = EVMA::GetMousePos().x;
		m_destinationY = EVMA::GetMousePos().y;
		m_dst.x = m_destinationX;
		m_dst.y = m_destinationY;
		m_Exist = true;
	


	}

	if (EVMA::MouseHeld(3)&& m_Exist)
	{
		m_Player->SetGrav(0.0f);
	

		if (MAMA::Distance(m_Player->GetDstP()->x + m_Player->GetDstP()->w /2, m_dst.x + m_dst.w /2, m_Player->GetDstP()->y + m_Player->GetDstP()->h /2, m_dst.y + m_dst.h /2) <= 5)
		{
			dx = dy = 0.0;
			m_Player->Stop();
			m_Exist = false;
		}
			
		else 
		{
			double a = MAMA::AngleBetweenPoints((m_dst.y + m_dst.h / 2) - (m_Player->GetDstP()->y + m_Player->GetDstP()->h / 2),
				(m_dst.x + m_dst.w / 2) - (m_Player->GetDstP()->x + m_Player->GetDstP()->w / 2));
			MAMA::SetDeltas(a, dx, dy, 5.0, 5.0);
			m_Player->SetVel(dx, dy);
		}
		
	}

	else if (EVMA::MouseReleased(3))
	{
		dx = dy = 0;
		m_Player->SetGrav(6.0f);
	}
	
	
}

void GrapplingHook::Render()
{
	
	if (m_Exist)
	{
		SDL_RenderCopyF(m_pRend, m_pText, &m_src, &m_dst);
	}

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

void GrapplingHook::Collision(int stage)
{
	if (stage == 1)
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
	else if (stage == 2)
	{

		for (int i = 0; i < ROWS2; ++i)
		{
			for (int j = 0; j < COLS2; ++j)
			{

				if (Engine::Instance().GetLevel2()[i][j]->IsObstacle() && COMA::AABBCheck(*this->GetDstP(), *Engine::Instance().GetLevel2()[i][j]->GetDstP()))
				{
					if (this->GetDstP()->y + Engine::Instance().GetLevel2()[i][j]->GetDstP()->h - (float)this->GetVelY() <= (float)Engine::Instance().GetLevel2()[i][j]->GetDstP()->y)
					{ // Colliding top side of platform.
						this->Stop();
						this->SetY(Engine::Instance().GetLevel2()[i][j]->GetDstP()->y - this->GetDstP()->h);

					}
					else if (this->GetDstP()->y - (float)this->GetVelY() >= Engine::Instance().GetLevel2()[i][j]->GetDstP()->y + Engine::Instance().GetLevel2()[i][j]->GetDstP()->h)
					{ // Colliding bottom side of platform.
						this->Stop();
						this->SetY(Engine::Instance().GetLevel2()[i][j]->GetDstP()->y + Engine::Instance().GetLevel2()[i][j]->GetDstP()->h);
					}
					else if (this->GetDstP()->x + this->GetDstP()->w - this->GetVelX() <= Engine::Instance().GetLevel2()[i][j]->GetDstP()->x)
					{ // Collision from left.
						this->Stop(); // Stop the player from moving horizontally.
						this->SetX(Engine::Instance().GetLevel2()[i][j]->GetDstP()->x - this->GetDstP()->w);
					}
					else if (this->GetDstP()->x - (float)this->GetVelX() >= Engine::Instance().GetLevel2()[i][j]->GetDstP()->x + Engine::Instance().GetLevel2()[i][j]->GetDstP()->w)
					{ // Colliding right side of platform.
						this->Stop();
						this->SetX(Engine::Instance().GetLevel2()[i][j]->GetDstP()->x + Engine::Instance().GetLevel2()[i][j]->GetDstP()->w);
					}
				}

			}
		}
	}
}
