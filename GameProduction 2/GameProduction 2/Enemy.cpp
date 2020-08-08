#include "Enemy.h"
#include <algorithm>
#include "MathManager.h"
#include "EventManager.h"
#include "CollisionManager.h"
#include "Engine.h"
#include "TextureManager.h"

Enemy::Enemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf, int boundary)
	:AnimatedSprite(s, d, r, t, sstart, smin, smax, nf), m_dir(0), m_State(idle)
{		
	this->boundary = boundary;
	m_grounded = false;
	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	m_maxVelX = 5.0;
	m_drag = 0.88;
	m_ePos.x = m_dst.x;
	m_ePos.y = m_dst.y;
	chasingTimer = 0;
	searchingDelay = 0;
	m_pSBox = { m_dst.x + m_dst.w, m_dst.y + 15, 160.0f, 80.0f };
	m_pBoxSrc = { 0, 63, 300, 200 };
}

void Enemy::Update(float AccelX, float AccelY, bool x, bool y, Player* p)
{
	// Do X axis first.
	m_velX += m_accelX;
	m_velX *= (m_grounded ? m_drag : 1);
	m_velX = std::min(std::max(m_velX, -(m_maxVelX)), (m_maxVelX));
	
	m_dst.x += (int)m_velX; // Had to cast it to int to get crisp collision with side of platform.
	// Now do Y axis.
	m_velY += m_accelY + m_grav; // Adjust gravity to get slower jump.
	m_velY = std::min(std::max(m_velY, -(m_maxVelY)), (m_maxVelY));
	m_dst.y += (int)m_velY; // To remove aliasing, I made cast it to an int too.
	m_accelX = m_accelY = 0.0;
	if (m_dir == 0)
	{
		m_pSBox.x = (int)m_dst.x + (int)m_dst.w;
		m_pSBox.y = m_dst.y +15;
	}

	if (m_dir == 1)
	{
		m_pSBox.x = m_dst.x - (m_pSBox.w);
		m_pSBox.y = m_dst.y + 15;
	}

	if (x)
	{
		m_ePos.x -= AccelX;
		m_dst.x -= AccelX;
		m_pSBox.x -= AccelX;
	}
	if (y)
	{
		m_ePos.y -= AccelY;
		m_dst.y -= AccelY;
		m_pSBox.y -= AccelX;
	}
	/*if (EVMA::KeyHeld(SDL_SCANCODE_2))
		m_State = seeking;
	else if (EVMA::KeyHeld(SDL_SCANCODE_3))
		m_State = idle;*/
	switch (m_State)
	{
	case idle:
	{	dx = dy = 0.0;
	
		if (COMA::AABBCheck(m_pSBox,*p->GetDstP()))
		{
			++searchingDelay;
		}
		if (searchingDelay >= 30 && Engine::Instance().getinvis() == false)
		{
			searchingDelay = 0;
			m_State = seeking;
		}
		if (!m_moving)
		{

			m_dir = 0;
			m_dst.x += 2;
	
			if (m_dst.x >= m_ePos.x + boundary)
			{
			m_moving = true;
			}

		}
		else if (m_moving)
		{

			m_dir = 1;
			m_dst.x -= 2.0f;

			if (m_dst.x <= m_ePos.x)
			{
				m_moving = false;
			}

		}
		break;
	}
	case seeking:
	{ // {} Needed because of local var.
		SDL_Rect temp1 = { MAMA::ConvertFRect2Rect(*GetDstP()) };

		SDL_Rect temp2 = { p->GetDstP()->x, p->GetDstP()->y, p->GetDstP()->w / 2.0f, p->GetDstP()->h / 2.0f };
		SDL_Rect temp3 = { p->GetDstP()->x, p->GetDstP()->y, p->GetDstP()->w, p->GetDstP()->h };
		if (m_dst.x > p->GetDstP()->x)
			m_dir = 1;
		else
			m_dir = 0;

		double a = MAMA::AngleBetweenPoints((p->GetDstP()->y + p->GetDstP()->h / 2) - (GetDstP()->y + GetDstP()->h / 2),
			(p->GetDstP()->x + p->GetDstP()->w / 2) - (GetDstP()->x + GetDstP()->w / 2));
		MAMA::SetDeltas(a, dx, dy, 2.0, 2.0);
		GetDstP()->x += (int)round(dx);
		GetDstP()->y += (int)round(dy);

		if (SDL_HasIntersection(&temp2, &temp1)) {
			m_State = arrive;
			if (m_slowCooldown == 0 && m_playerslow == false)
				m_playerslow = true;
		}
		if (chasingTimer == 150)
		{
			chasingTimer = 0;
			m_State = arrive;
		}
		++chasingTimer; 
		
		break;
	}
	case arrive:
	{	
		if (m_dst.x > m_ePos.x)
			m_dir = 1;
		else
			m_dir = 0;
		
		if (MAMA::Distance(GetDstP()->x + GetDstP()->w / 2, m_ePos.x, GetDstP()->y + GetDstP()->h / 2, m_ePos.y) <= 5)
		{
			dx = dy = 0.0;
			m_State = idle;
		}
		double a = MAMA::AngleBetweenPoints((m_ePos.y) - (GetDstP()->y + GetDstP()->h / 2),
			(m_ePos.x) - (GetDstP()->x + GetDstP()->w / 2));
		MAMA::SetDeltas(a, dx, dy, 2.0, 2.0);
		Collision();
		if (!sideCol)
		{
			GetDstP()->x += (int)round(dx);
			
		}
		
		if (!updownCol)
		{
			GetDstP()->y += (int)round(dy);
			
		}
		sideCol = false;
		updownCol = false;
	}
	}
	if (m_playerslow == true && m_slowCooldown < 40) {
			p->SetMaxVel(2);
			++m_slowCooldown;
		}
	if (m_slowCooldown >= 40) {
			m_slowCooldown = 0;
			p->SetMaxVel(5);
			m_playerslow = false;
		}

	Animate();
}

void Enemy::Render()
{
	SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), GetDstP(), m_angle, 0, static_cast<SDL_RendererFlip>(m_dir));
	if (m_State == idle)
	{
		//SDL_SetRenderDrawColor(m_pRend, 192, 64, 0, 255);
		//SDL_RenderFillRectF(m_pRend, &m_pSBox);
		SDL_RenderCopyExF(m_pRend, TEMA::GetTexture("light"), &m_pBoxSrc, &m_pSBox, m_angle, 0, static_cast<SDL_RendererFlip>(m_dir));
	}
}
void Enemy::AI(SDL_Rect* a)
{
	SDL_Rect* temp = a;
	double dist = MAMA::Distance(m_dst.x + m_dst.w / 2.0f, temp->x + temp->w / 2.0f, m_dst.y + m_dst.h / 2.0f, temp->y + temp->h / 2.0f);


}

void Enemy::Collision()
{
	for (unsigned i = 0; i < Engine::Instance().GetPlatform().size(); i++) // For each platform.
	{

		if (COMA::AABBCheck(*GetDstP(), *Engine::Instance().GetPlatform()[i]->GetDstP()))
		{
			if (GetDstP()->y + GetDstP()->h - (float)GetVelY() <= Engine::Instance().GetPlatform()[i]->GetDstP()->y)
			{ // Colliding top side of platform.
				updownCol = true;
				SetY(Engine::Instance().GetPlatform()[i]->GetDstP()->y - GetDstP()->h);
			}
			else if (GetDstP()->y - (float)GetVelY() >= Engine::Instance().GetPlatform()[i]->GetDstP()->y +
				Engine::Instance().GetPlatform()[i]->GetDstP()->h)
			{ // Colliding bottom side of platform.
				sideCol = true;
				SetY(Engine::Instance().GetPlatform()[i]->GetDstP()->y + Engine::Instance().GetPlatform()[i]->GetDstP()->h);
			}
			else if (GetDstP()->x + GetDstP()->w - GetVelX() <= Engine::Instance().GetPlatform()[i]->GetDstP()->x)
			{ // Collision from left.
				updownCol = true;
				SetX(Engine::Instance().GetPlatform()[i]->GetDstP()->x - GetDstP()->w);
			}
			else if (GetDstP()->x - (float)GetVelX() >= Engine::Instance().GetPlatform()[i]->GetDstP()->x + Engine::Instance().GetPlatform()[i]->GetDstP()->w)
			{ // Colliding right side of platform.
				sideCol = true;
				SetX(Engine::Instance().GetPlatform()[i]->GetDstP()->x + Engine::Instance().GetPlatform()[i]->GetDstP()->w);
			}
		}
	}
}

void Enemy::setInvis(bool a)
{
	m_invis = a;
}

void Enemy::StopX() { m_velX = 0.0; }
void Enemy::StopY() { m_velY = 0.0; }
void Enemy::SetAccelX(double a) { m_accelX = a; }
void Enemy::SetAccelY(double a) { m_accelY = a; }
bool Enemy::IsGrounded() { return m_grounded; }
void Enemy::SetGrounded(bool g) { m_grounded = g; }
double Enemy::GetVelX() { return m_velX; }
double Enemy::GetVelY() { return m_velY; }
void Enemy::SetX(float y) { m_dst.x = y; }
void Enemy::SetY(float y) { m_dst.y = y; }




