#include "Player.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "Engine.h"


Player::Player(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf)
	:AnimatedSprite(s, d, r, t, sstart, smin, smax, nf), m_state(idle), m_dir(0) {

	m_grounded = false;
	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	m_maxVelX = 5.0;
	m_maxVelY = 60;
	m_grav = GRAV;
	m_drag = 0.88;
	m_a = &m_accelX;
	m_aMaxY = &m_maxVelX;
	speedDowncount = 0;
	m_energy = 0; 
	m_en = &m_energy;
	m_invis = new invisibility();
	m_speedBoost = new SpeedBoost();
	m_doubleJump = new doubleJump();
	m_barrier = new Barrier();
	slowHazardCheck = false;
}

void Player::Update(int stage)
{
	
	m_doubleJump->Update(m_en);
	
	m_pbarSkill = m_barrier->getbar();
	if (EVMA::KeyHeld(SDL_SCANCODE_A))
	{		
		m_accelX -= 1;				
	}
	else if (EVMA::KeyHeld(SDL_SCANCODE_D))
	{
		m_accelX += 1;
	}
	if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_doubleJump->getTotalJumps() < 2 && (IsGrounded() || m_doubleJump->getdoubleJump() == true))
	{
		SetAccelY(0);
		AddAccelY(-20);
		if(m_doubleJump->getTotalJumps() == 1)
			AddAccelY(-15);
		SetGrav(2);
		m_doubleJump->addTotalJumps(1);
		SetGrounded(false);
		
	}
	
	m_dX = &this->GetDstP()->x;
	m_dY = &this->GetDstP()->y;
	m_speedBoost->Update(m_a, m_aMaxY, m_en, m_dX, m_dY);
	m_invis->Update(m_en);
	

	//std::cout << Engine::Instance().getinvis();

	m_bgScrollX = m_bgScrollY = false;
	if (stage == 1)
	{
		if (GetVelX() > 0 && GetDstP()->x > WIDTH * 0.7f)
		{
			if (Engine::Instance().GetLevel()[0][COLS - 1]->GetDstP()->x > WIDTH - 32)
			{
				m_bgScrollX = true;
				HandleCamera((float)GetVelX(), true);
			}
		}
		else if (GetVelX() < 0 && GetDstP()->x < WIDTH * 0.3f)
		{
			if (Engine::Instance().GetLevel()[0][0]->GetDstP()->x < 0)
			{
				m_bgScrollX = true;
				HandleCamera((float)GetVelX(), true);
			}
		}
		if (GetVelY() > 0 && GetDstP()->y > HEIGHT * 0.7f)
		{
			if (Engine::Instance().GetLevel()[ROWS - 1][0]->GetDstP()->y > HEIGHT - 32)
			{
				m_bgScrollY = true;
				HandleCamera((float)GetVelY());
			}
		}
		else if (GetVelY() < 0 && GetDstP()->y < HEIGHT * 0.3f)
		{
			if (Engine::Instance().GetLevel()[0][0]->GetDstP()->y < 0)
			{
				m_bgScrollY = true;
				HandleCamera((float)GetVelY());
			}
		}
	}
	else if (stage == 2)
	{
		if (GetVelX() > 0 && GetDstP()->x > WIDTH * 0.7f)
		{
			if (Engine::Instance().GetLevel2()[0][COLS2 - 1]->GetDstP()->x > WIDTH - 32)
			{
				m_bgScrollX = true;
				HandleCamera((float)GetVelX(), true, stage);
			}
		}
		else if (GetVelX() < 0 && GetDstP()->x < WIDTH * 0.3f)
		{
			if (Engine::Instance().GetLevel2()[0][0]->GetDstP()->x < 0)
			{
				m_bgScrollX = true;
				HandleCamera((float)GetVelX(), true, stage);
			}
		}
		if (GetVelY() > 0 && GetDstP()->y > HEIGHT * 0.7f)
		{
			if (Engine::Instance().GetLevel2()[ROWS2 - 1][0]->GetDstP()->y > HEIGHT - 32)
			{
				m_bgScrollY = true;
				HandleCamera((float)GetVelY(), false, stage);
			}
		}
		else if (GetVelY() < 0 && GetDstP()->y < HEIGHT * 0.3f)
		{
			if (Engine::Instance().GetLevel2()[0][0]->GetDstP()->y < 0)
			{
				m_bgScrollY = true;
				HandleCamera((float)GetVelY(), false, stage);
			}
		}
	}
	
	Collision();
	//std::cout << GetVelY() << std::endl;
	switch (m_state)
	{
	case idle:
		if (GetVelY() < 0)
		{
			SetState(jump);
		}
		if (GetVelY() > 0)
		{
			SetState(fall);
		}
		if (EVMA::KeyHeld(SDL_SCANCODE_A) || EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			SetState(running);
		}
		break;
	case running:
		if(m_speedBoost->getspeedBoost() == true)
			SetState(boost);
		if (m_invis->getinvis() == true)
			SetState(invis);
		if (EVMA::KeyReleased(SDL_SCANCODE_A) || EVMA::KeyReleased(SDL_SCANCODE_D))
		{
			SetState(idle);
			break; // Skip movement parsing below.
		}
		if (EVMA::KeyHeld(SDL_SCANCODE_A))
		{
			m_dir = 1;
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			m_dir = 0;

		}
		break;
	case jump:
		if (GetVelY() == 0)
		{
			SetState(idle);
			break;
		}
		if (GetVelY() > 0)
		{
			SetState(fall);
			break;
		}

		break;
	case fall:
		if (GetVelY() == 0)
		{
			SetState(idle);
			break;
		}
		if (GetVelY() < 0)
		{
			SetState(jump);
			break;
		}

		break;
	case boost:
		if (EVMA::KeyReleased(SDL_SCANCODE_A) || EVMA::KeyReleased(SDL_SCANCODE_D))
		{
			SetState(idle);
			break; // Skip movement parsing below.
		}
		if (EVMA::KeyHeld(SDL_SCANCODE_A))
		{
			m_dir = 1;
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			m_dir = 0;

		}
		break;
	case invis:
		if (EVMA::KeyReleased(SDL_SCANCODE_A) || EVMA::KeyReleased(SDL_SCANCODE_D))
		{
			SetState(idle);
			break; // Skip movement parsing below.
		}
		if (EVMA::KeyHeld(SDL_SCANCODE_A))
		{
			m_dir = 1;
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			m_dir = 0;

		}
		break;
	}
	
	Animate();
	bool *f = &m_dir;
	double* h = &m_velX;
	m_barrier->Update(m_en, m_dX, m_dY, f, h);
	slowHazard();
	UpdateAxis(m_bgScrollX, m_bgScrollY);
}

void Player::Render()
{	
	SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), GetDstP(), m_angle, 0, static_cast<SDL_RendererFlip>(m_dir));
	m_speedBoost->Render();
	m_invis->Render();
	m_doubleJump->Render();
	m_barrier->Render();
}

void Player::SetState(int s)
{
	m_state = static_cast<state>(s);
	m_frame = 0;
	if (m_state == idle)
	{
		m_pText = TEMA::GetTexture("playerIdle");
		m_sprite = m_spriteMin = 0;
		m_spriteMax = 4;
	}
	else if (m_state == running)// Only other is running for now...
	{
		m_pText = TEMA::GetTexture("playerWalk");
		m_sprite = m_spriteMin = 0;
		m_spriteMax = 5;
	}
	else if (m_state == jump)
	{
		m_pText = TEMA::GetTexture("playerJump");
		m_sprite = 0;
		m_spriteMin = 0;
		m_spriteMax = 3;
	}
	else if (m_state == fall)
	{
		m_pText = TEMA::GetTexture("playerfall");
		m_sprite = 0;
		m_spriteMin = 0;
		m_spriteMax = 3;
	}
	else if (m_state == boost)
	{
		m_pText = TEMA::GetTexture("playerboost");
		m_sprite = m_spriteMin = 0;
		m_spriteMax = 5;

	}
	else if (m_state == invis)
	{
		m_pText = TEMA::GetTexture("playerinvis");
		m_sprite = m_spriteMin = 0;
		m_spriteMax = 5;
	}
}


void Player::Collision()
{
	for (unsigned i = 0; i < Engine::Instance().GetPlatform().size(); i++) // For each platform.
	{
		//Platform collision
		if (COMA::AABBCheck(*GetDstP(), *Engine::Instance().GetPlatform()[i]->GetDstP()))
		{			
			if (GetDstP()->y + GetDstP()->h - (float)GetVelY() <= Engine::Instance().GetPlatform()[i]->GetDstP()->y)
			{ // Colliding top side of platform.
				SetGrounded(true);
				m_doubleJump->setTotalJumps(0);
				SetGrav(4);
				StopY();
				SetY(Engine::Instance().GetPlatform()[i]->GetDstP()->y - GetDstP()->h);
			}
			else if (GetDstP()->y - (float)GetVelY() >= Engine::Instance().GetPlatform()[i]->GetDstP()->y + 
				Engine::Instance().GetPlatform()[i]->GetDstP()->h)
			{ // Colliding bottom side of platform.
				StopY();
				SetY(Engine::Instance().GetPlatform()[i]->GetDstP()->y + Engine::Instance().GetPlatform()[i]->GetDstP()->h);
			}
			else if (GetDstP()->x + GetDstP()->w - GetVelX() <= Engine::Instance().GetPlatform()[i]->GetDstP()->x)
			{ // Collision from left.
				StopX(); // Stop the player from moving horizontally.
				SetX(Engine::Instance().GetPlatform()[i]->GetDstP()->x - GetDstP()->w);
			}
			else if (GetDstP()->x - (float)GetVelX() >= Engine::Instance().GetPlatform()[i]->GetDstP()->x + Engine::Instance().GetPlatform()[i]->GetDstP()->w)
			{ // Colliding right side of platform.
				StopX();
				SetX(Engine::Instance().GetPlatform()[i]->GetDstP()->x + Engine::Instance().GetPlatform()[i]->GetDstP()->w);
			}
		}
	}

	for (unsigned i = 0; i < Engine::Instance().GetHazard().size(); i++) // For each platform.
	{
		//Hazard collision 
		if (COMA::AABBCheck(*GetDstP(), *Engine::Instance().GetHazard()[i]->GetDstP()))
		{
			if (slowHazardCheck == false)
				slowHazardCheck = true;
		}
	}
}
void Player::HandleCamera(float scroll, bool x, int stage)
{ 
	if (stage == 1)
	{		
		for (int row = 0; row < ROWS; row++)
		{
			for (int col = 0; col < COLS; col++)
			{
				if (x)
					Engine::Instance().GetLevel()[row][col]->GetDstP()->x -= (int)scroll;
				else
					Engine::Instance().GetLevel()[row][col]->GetDstP()->y -= (int)scroll;
			}

		}

		for (auto i = 0; i < Engine::Instance().GetEnemy().size(); ++i)
		{
			if (x)
			{
				Engine::Instance().GetEnemy()[i]->GetDstP()->x -= (int)scroll;
				Engine::Instance().GetEnemy()[i]->m_ePos.x -= (int)scroll;
				Engine::Instance().GetEnemy()[i]->m_pSBox.x -= (int)scroll;
			}
			else
			{
				Engine::Instance().GetEnemy()[i]->GetDstP()->y -= (int)scroll;
				Engine::Instance().GetEnemy()[i]->m_ePos.y -= (int)scroll;
				Engine::Instance().GetEnemy()[i]->m_pSBox.y -= (int)scroll;
			}

		}
		

	}
	else if (stage == 2)
	{
		for (int row = 0; row < ROWS2; row++)
		{
			for (int col = 0; col < COLS2; col++)
			{
				if (x)
					Engine::Instance().GetLevel2()[row][col]->GetDstP()->x -= (int)scroll;
				else
					Engine::Instance().GetLevel2()[row][col]->GetDstP()->y -= (int)scroll;
			}
		}
		for (auto i = 0; i < Engine::Instance().GetEnemy().size(); ++i)
		{
			if (x)
			{
				Engine::Instance().GetEnemy()[i]->GetDstP()->x -= (int)scroll;
				Engine::Instance().GetEnemy()[i]->m_ePos.x -= (int)scroll;
				Engine::Instance().GetEnemy()[i]->m_pSBox.x -= (int)scroll;
			}
			else
			{
				Engine::Instance().GetEnemy()[i]->GetDstP()->y -= (int)scroll;
				Engine::Instance().GetEnemy()[i]->m_ePos.y -= (int)scroll;
				Engine::Instance().GetEnemy()[i]->m_pSBox.y -= (int)scroll;
			}

		}
	}
}
void Player::UpdateAxis(bool sX, bool sY)
{
	// Do X axis first.
	m_velX += m_accelX;
	m_velX *= (m_grounded ? m_drag : 1);
	m_velX = std::min(std::max(m_velX, -(m_maxVelX)), (m_maxVelX));
	if (!sX)
		m_dst.x += (int)m_velX; // Had to cast it to int to get crisp collision with side of platform.
	// Now do Y axis.
	m_velY += m_accelY + m_grav; // Adjust gravity to get slower jump.
	m_velY = std::min(std::max(m_velY, -(m_maxVelY)), (m_grav * 5));
	if (!sY)
		m_dst.y += (int)m_velY; // To remove aliasing, I made cast it to an int too.
	m_accelX = m_accelY = 0.0;
}
void Player::Stop() // If you want a dead stop both axes.
{
	StopX();
	StopY();
}
void Player::slowHazard()
{
	if (slowHazardCheck == true)
	{
		this->SetMaxVel(1);
		++slowHazardtimer;
		if (slowHazardtimer >= 50)
		{
			this->SetMaxVel(5);
			slowHazardCheck = false;
			slowHazardtimer = 0;
		}
	}
}
void Player::StopX() { m_velX = 0.0; }
void Player::StopY() { m_velY = 0.0; }
void Player::SetAccelX(double a) { m_accelX = a; }
void Player::SetAccelY(double a) { m_accelY = a; }



bool Player::IsGrounded() { return m_grounded; }
void Player::SetGrounded(bool g) { m_grounded = g; }
double Player::GetVelX() { return m_velX; }
double Player::GetVelY() { return m_velY; }
void Player::SetX(float y) { m_dst.x = y; }
void Player::SetY(float y) { m_dst.y = y; }
void Player::SetVel(double a, double b) { m_velX = a; m_velY = b; }
void Player::AddAccelX(double a)
{
	m_accelX += a;
}
void Player::AddAccelY(double a)
{
	m_accelY += a;
}
void Player::SetMaxVel(double a)
{
	m_maxVelX = a;
}

void Player::setEnergy(int a)
{
	m_energy += a;
}
void Player::SetGrav(double y)
{
	m_grav = y;
}