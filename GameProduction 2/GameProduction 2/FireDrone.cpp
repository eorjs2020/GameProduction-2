#include "FireDrone.h"
#include "CollisionManager.h"
#include "Engine.h"
#include "DebugManager.h"
#include "Bullet.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Utility.h"
FireDrone::FireDrone(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf, std::vector<Bullet*>* bVec) : 
	AnimatedSprite(s, d, r, t, sstart, smin, smax, nf), m_dir(0), m_State(idle), m_bulletTimer(0), m_timerMax(nf), m_pBulletVec(bVec)
{
	
	m_grounded = false;
	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	m_maxVelX = 5.0;
	m_drag = 0.88;
	m_ePos.x = m_dst.x;
	m_ePos.y = m_dst.y;
	chasingTimer = 0;
	searchingDelay = 0;

}

void FireDrone::Update(float AccelX, float AccelY, bool x, bool y, Player* p)
{
	
	m_velX += m_accelX;
	m_velX *= (m_grounded ? m_drag : 1);
	m_velX = std::min(std::max(m_velX, -(m_maxVelX)), (m_maxVelX));
	m_dst.x += (int)m_velX; // Had to cast it to int to get crisp collision with side of platform.
	m_velY += m_accelY + m_grav; // Adjust gravity to get slower jump.
	m_velY = std::min(std::max(m_velY, -(m_maxVelY)), (m_grav * 5));

	m_dst.y += (int)m_velY; // To remove aliasing, I made cast it to an int too.
	m_accelX = m_accelY = 0.0;
	
	
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
		m_pSBox.y -= AccelY;
	}
	switch (m_State)
	{
	case idle:
	{

		if (MAMA::Distance(GetDstP()->x + GetDstP()->w / 2, p->GetDstP()->x + p->GetDstP()->w /2, GetDstP()->y + GetDstP()->h / 2, p->GetDstP()->y + p->GetDstP()->h /2) <= 500)
		{
			m_State = fire;
		}



		break;
	}
	case fire:
	{
		
		
		if (MAMA::Distance(GetDstP()->x + GetDstP()->w / 2, p->GetDstP()->x + p->GetDstP()->w / 2, GetDstP()->y + GetDstP()->h / 2, p->GetDstP()->y + p->GetDstP()->h / 2) <= 500)
		{
			glm::vec2 temp = { p->GetDstP()->x, p->GetDstP()->y };
			if (m_bulletTimer++ == m_timerMax + 50)
			{
				std::cout << "fire" << std::endl;
				m_bulletTimer = 0;
				m_pBulletVec->push_back(new Bullet({ 0, 0, 160, 160 }, { GetDstP()->x + GetDstP()->w / 2, GetDstP()->y + GetDstP()->h / 2, 42, 42 },
					m_pRend, TEMA::GetTexture("dronebullet"), 6, temp));
				
				
			}
		}
		else if(MAMA::Distance(GetDstP()->x + GetDstP()->w / 2, p->GetDstP()->x + 
			p->GetDstP()->w / 2, GetDstP()->y + GetDstP()->h / 2, p->GetDstP()->y + p->GetDstP()->h / 2) >= 500)
			m_State = idle;

		break;
	}
	}
	Animate();
}

void FireDrone::Render()
{
	SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), GetDstP(), m_angle, 0, static_cast<SDL_RendererFlip>(m_dir));
	
}

void FireDrone::StopX() { m_velX = 0.0; }
void FireDrone::StopY() { m_velY = 0.0; }
void FireDrone::SetAccelX(double a) { m_accelX = a; }
void FireDrone::SetAccelY(double a) { m_accelY = a; }
bool FireDrone::IsGrounded() { return m_grounded; }
void FireDrone::SetGrounded(bool g) { m_grounded = g; }
double FireDrone::GetVelX() { return m_velX; }
double FireDrone::GetVelY() { return m_velY; }
void FireDrone::SetX(float y) { m_dst.x = y; }
void FireDrone::SetY(float y) { m_dst.y = y; }

void FireDrone::LOS(Player* p)
{
	LOSCheck(p);
	auto LOSColour = (HasLineofSight) ? glm::vec4(255.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 255.0f, 0.0f, 1.0f);
	
	DEMA::DrawLine({ int(p->GetDstP()->x + p->GetDstP()->w / 2), int(p->GetDstP()->y + p->GetDstP()->h / 2) },
		{ int(GetDstP()->x + GetDstP()->w / 2), int(GetDstP()->y + GetDstP()->h / 2) },
		{ Uint8(LOSColour.r), Uint8(LOSColour.g), Uint8(LOSColour.b), Uint8(LOSColour.a) });
	
}

void FireDrone::LOSCheck(Player* p)
{
	int temp = 0;
	for (auto i = 0; i < Engine::Instance().GetPlatform().size(); ++i)
	{
		if (COMA::LOSCheck(p, this, Engine::Instance().GetPlatform()[i]))
			++temp;
	}
	if (temp == 0)
		HasLineofSight = true;
}

void FireDrone::BulletCollision(Player* p)
{
	/*for (int i = 0; i < (int)m_pBulletVec.size(); i++)
	{
		m_pBulletVec[i]->Update();
		SDL_Rect b = { m_pBulletVec[i]->GetDstP()->x, m_pBulletVec[i]->GetDstP()->y,
			m_pBulletVec[i]->GetDstP()->w, m_pBulletVec[i]->GetDstP()->h };
		
		
		SDL_Rect e = { p->GetDstP()->x, p->GetDstP()->y, 32, 32 };
		if (SDL_HasIntersection(&b, &e))
		{
			
			delete m_pBulletVec[i];
			m_pBulletVec[i] = nullptr;
			m_bullNull = true;
			break;
		}
		
	}
	if (m_bullNull) CleanVector<Bullet*>(m_pBulletVec, m_bullNull);
	for (int i = 0; i < (int)m_pBulletVec.size(); i++)
	{
		SDL_Rect b = { m_pBulletVec[i]->GetDstP()->x, m_pBulletVec[i]->GetDstP()->y,
			m_pBulletVec[i]->GetDstP()->w, m_pBulletVec[i]->GetDstP()->h };
		for (int j = 0; j < Engine::Instance().GetPlatform().size(); j++)
		{
			if (Engine::Instance().GetPlatform()[j] == nullptr) continue;
			SDL_Rect e = { Engine::Instance().GetPlatform()[j]->GetDstP()->x, Engine::Instance().GetPlatform()[j]->GetDstP()->y, 32, 32 };
			if (SDL_HasIntersection(&b, &e))
			{
				delete m_pBulletVec[i];
				m_pBulletVec[i] = nullptr;
				m_bullNull = true;
				break;
			}
		}
	}
	if (m_bullNull) CleanVector<Bullet*>(m_pBulletVec, m_bullNull);*/
}
