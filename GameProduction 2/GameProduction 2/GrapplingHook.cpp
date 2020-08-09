#include "GrapplingHook.h"
#include "Engine.h"
#include <algorithm>
#include <iostream>
#include "MathManager.h"
#include "EventManager.h"
#include "CollisionManager.h"
#include "DebugManager.h"
#include "TextureManager.h"

GrapplingHook::GrapplingHook(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, double dir, Player * a, double mousePosX, double mousePosY)
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
	m_dst.x = m_Player->GetDstP()->x;
	m_dst.y = m_Player->GetDstP()->y;
	endpointX = mousePosX;
	endpointY = mousePosY;
	m_destinationX = m_Player->GetDstP()->x;
	m_destinationY = m_Player->GetDstP()->y;
	shoot = false;
	m_line = new Sprite({ 0,20,189,30 }, { m_Player->GetDstP()->x, m_Player->GetDstP()->y, 100, 3 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("hook"));
	point.x = 0;
	point.y = 0;
	m_line->SetPoint(&point);
	m_Exist = true;
	shoot = true;
	m_dst.x = m_Player->GetDstP()->x;
	m_dst.y = m_Player->GetDstP()->y;
	anglecheck = true;
}


void GrapplingHook::Update()
{
	//std::cout << endpointX << std::endl << endpointY << std::endl;
	
	// Do X axis first.
	m_velX += m_accelX;
	m_velX *= (m_grounded ? m_drag : 1);
	m_velX = std::min(std::max(m_velX, -(m_maxVelX)), (m_maxVelX));
	m_dst.x += (int)m_velX; // Had to cast it to int to get crisp collision with side of platform.
	// Now do Y axis.
	m_velY += m_accelY + m_grav; // Adjust gravity to get slower jump.
	m_velY = std::min(std::max(m_velY, -(m_maxVelY)), (m_grav * 5));
	m_dst.y += (int)m_velY; // To remove aliasing, I made cast it to an int too.
	m_accelX = m_accelY = 0.0;

	if (m_Player->BGScrollY())
	{
		m_dst.y -= m_Player->GetVelY();
	}
	if (m_Player->BGScorllX())
	{
		m_dst.x -= m_Player->GetVelX();
	}

	/*if (EVMA::MouseReleased(1))
	{*/
		/*m_destinationX = EVMA::GetMousePos().x;
		m_destinationY = EVMA::GetMousePos().y;*/
		
		

	//}

	//if (shoot)
	//{
	//	//std::cout << "a" << std::endl;
	//	if (MAMA::Distance(m_dst.x + m_dst.w / 2, endpointX, m_dst.y + m_dst.h / 2, endpointY) == 0)
	//	{
	//		dx = dy = 0.0;
	//		shoot = false;
	//	}

	//	else
	//	{
	if (anglecheck == true ) {
		double a = MAMA::AngleBetweenPoints((endpointY)-(m_dst.y + m_dst.h / 2),
			(endpointX)-(m_dst.x + m_dst.w / 2));
		MAMA::SetDeltas(a, dx, dy, 10.0, 10.0);
		anglecheck = false;
	}
	if (shoot) {
		m_velX = (int)round(dx);
		m_velY = (int)round(dy);
	}
	//	}
	//}
	if (EVMA::MouseHeld(1) && shoot == false)
	{
		m_Player->SetGrav(0.0f);
		double a = MAMA::AngleBetweenPoints((m_dst.y + m_dst.h / 2) - (m_Player->GetDstP()->y + m_Player->GetDstP()->h / 2),
			(m_dst.x + m_dst.w / 2) - (m_Player->GetDstP()->x + m_Player->GetDstP()->w / 2));
		MAMA::SetDeltas(a, dx, dy, 4.0, 4.0);
		m_Player->SetVel((int)round(dx), (int)round(dy));
		
	}
	if (MAMA::Distance(m_Player->GetDstP()->x + m_Player->GetDstP()->w / 2, m_dst.x + m_dst.w / 2, m_Player->GetDstP()->y + m_Player->GetDstP()->h / 2, m_dst.y + m_dst.h / 2) <= 0 && shoot == false)
	{
		m_dst.x = m_Player->GetDstP()->x;
		m_dst.y = m_Player->GetDstP()->y;
		dx = dy = 0.0;
		m_Player->Stop();
		m_Exist = false;
	}
		

	if (EVMA::MouseReleased(1) && m_Exist)
	{
		dx = dy = 0;
		m_Player->SetGrav(4.0f);
		this->SetExist(false);
	}
	double m_lineAngle;
	 m_lineAngle = MAMA::AngleBetweenPoints((m_dst.y + m_dst.h / 2) - (m_Player->GetDstP()->y + m_Player->GetDstP()->h / 2),
		(m_dst.x + m_dst.w / 2) - (m_Player->GetDstP()->x + m_Player->GetDstP()->w / 2));
	/*if(m_Player->GetDstP()->x - this->GetDstP()->x)*/
	m_line->SetAngle(((m_lineAngle * 180 / 3.14) + 180));
	
	m_line->SetDstP(this->GetDstP()->x, this->GetDstP()->y);
	
	/*if ( -90 < m_lineAngle && m_lineAngle < -180 )
		m_line->SetDstP(m_Player->GetDstP()->x - 32 * cos(m_lineAngle), m_Player->GetDstP()->y + 15 * 1 - cos(m_lineAngle));
	if (0 < m_lineAngle && m_lineAngle < -90)
		m_line->SetDstP(m_Player->GetDstP()->x + 32 * cos(m_lineAngle), m_Player->GetDstP()->y + 15 * 1 - cos(m_lineAngle));*/


	//std::cout << (m_lineAngle * 180 / 3.14) << std::endl;
	m_line->setSize(sqrt(pow(abs(this->m_dst.x - m_Player->GetDstP()->x), 2) + pow(abs(this->m_dst.y - m_Player->GetDstP()->y), 2)), 3);
	
	if (m_line->GetDstP()->w > 600)
		this->SetExist(false);
	/*if (shoot == false && m_dst.x == endpointX && m_dst.y == endpointY)
		this->SetExist(false);*/

	
	//std::cout << m_line->GetDstP()->y << std::endl;
	//m_line->SetDstP(m_Player->GetDstP()->x, m_Player->GetDstP()->y);
	
	//m_line->SetDstP( m_Player->GetDstP()->x + 32 - (m_line->GetDstP()->w) / 2 * cos(m_lineAngle), m_Player->GetDstP()->y + 15 - sqrt(pow( (m_line->GetDstP()->w)/2, 2) * 2 - 2 * (m_line->GetDstP()->w)  * cos(m_lineAngle)));
	//m_line->SetDstP(m_Player->GetDstP()->x + 32 - (m_line->GetDstP()->w) / (2 * cos(m_lineAngle)), m_Player->GetDstP()->y + 15 - (m_line->GetDstP()->w) / (2 * sin(m_lineAngle))) ;


	}

void GrapplingHook::Render()
{	
	SDL_RenderCopyF(m_pRend, m_pText, &m_src, &m_dst);
	m_line->Render();
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
						dx = dy = 0.0;
						shoot = false;

					}
					else if (this->GetDstP()->y - (float)this->GetVelY() >= Engine::Instance().GetLevel()[i][j]->GetDstP()->y + Engine::Instance().GetLevel()[i][j]->GetDstP()->h)
					{ // Colliding bottom side of platform.
						this->Stop();
						dx = dy = 0.0;
						shoot = false;
					}
					else if (this->GetDstP()->x + this->GetDstP()->w - this->GetVelX() <= Engine::Instance().GetLevel()[i][j]->GetDstP()->x)
					{ // Collision from left.
						this->Stop(); // Stop the player from moving horizontally.
						dx = dy = 0.0;
						shoot = false;
					}
					else if (this->GetDstP()->x - (float)this->GetVelX() >= Engine::Instance().GetLevel()[i][j]->GetDstP()->x + Engine::Instance().GetLevel()[i][j]->GetDstP()->w)
					{ // Colliding right side of platform.
						this->Stop();
						dx = dy = 0.0;
						shoot = false;
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
						dx = dy = 0.0;
						shoot = false;

					}
					else if (this->GetDstP()->y - (float)this->GetVelY() >= Engine::Instance().GetLevel2()[i][j]->GetDstP()->y + Engine::Instance().GetLevel2()[i][j]->GetDstP()->h)
					{ // Colliding bottom side of platform.
						this->Stop();
						dx = dy = 0.0;
						shoot = false;
					}
					else if (this->GetDstP()->x + this->GetDstP()->w - this->GetVelX() <= Engine::Instance().GetLevel2()[i][j]->GetDstP()->x)
					{ // Collision from left.
						this->Stop();
						dx = dy = 0.0;
						shoot = false;
					}
					else if (this->GetDstP()->x - (float)this->GetVelX() >= Engine::Instance().GetLevel2()[i][j]->GetDstP()->x + Engine::Instance().GetLevel2()[i][j]->GetDstP()->w)
					{ // Colliding right side of platform.
						this->Stop();
						dx = dy = 0.0;
						shoot = false;
					}
				}

			}
		}
	}
}
