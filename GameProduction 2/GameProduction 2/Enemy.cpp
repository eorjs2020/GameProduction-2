#include "Enemy.h"
#include <algorithm>
#include "MathManager.h"
#include "EventManager.h"
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
}

void Enemy::Update(float AccelX, float AccelY, bool x, bool y, Sprite* p)
{

	if (x)
	{
		m_ePos.x -= AccelX;
		m_dst.x -= AccelX;
	}
	if (y)
	{
		m_ePos.y -= AccelY;
		m_dst.y -= AccelY;
	}
	if (EVMA::KeyHeld(SDL_SCANCODE_2))
		m_State = seeking;
	else if (EVMA::KeyHeld(SDL_SCANCODE_3))
		m_State = idle;
	switch (m_State)
	{
	case idle:
		dx = dy = 0.0;
		if (p->GetDstP()->x + p->GetDstP()->w / 2 < m_ePos.x + boundary && p->GetDstP()->x + p->GetDstP()->w / 2 >= m_ePos.x &&
			p->GetDstP()->y + p->GetDstP()->h / 2 < m_ePos.y + 100 && p->GetDstP()->y + p->GetDstP()->h / 2 >= m_ePos.y - 100)
			++searchingDelay;
		if (searchingDelay == 30)
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
		MAMA::SetDeltas(a, dx, dy, 4.0, 4.0);
		GetDstP()->x += (int)round(dx); // Our rects are integers! Boo! This is a job for SuperFRect!
		GetDstP()->y += (int)round(dy);

		if (SDL_HasIntersection(&temp2, &temp1))
		{
			
			m_State = arrive;
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
		GetDstP()->x += (int)round(dx); // Our rects are integers! Boo! This is a job for SuperFRect!
		GetDstP()->y += (int)round(dy);

		
	}
	

	}

	Animate();
}

void Enemy::Render()
{
	SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), GetDstP(), m_angle, 0, static_cast<SDL_RendererFlip>(m_dir));
}
void Enemy::AI(SDL_Rect* a)
{
	SDL_Rect* temp = a;
	double dist = MAMA::Distance(m_dst.x + m_dst.w / 2.0f, temp->x + temp->w / 2.0f, m_dst.y + m_dst.h / 2.0f, temp->y + temp->h / 2.0f);


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




