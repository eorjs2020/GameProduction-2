#include "Player.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "Engine.h"
#include "SkillManager.h"
#define SPEED 2

Player::Player(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf)
	:AnimatedSprite(s, d, r, t, sstart, smin, smax, nf), m_state(idle), m_dir(0) {

	m_grounded = false;
	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	m_maxVelX = 5.0;
	m_maxVelY = JUMPFORCE;
	m_grav = GRAV;
	m_drag = 0.88;
	m_a = &m_accelX;
	m_aMaxY = &m_maxVelX;
}

void Player::Update()
{
	// Do X axis first.
	m_velX += m_accelX;
	m_velX *= (m_grounded ? m_drag : 1);
	m_velX = std::min(std::max(m_velX, -(m_maxVelX)), (m_maxVelX));
	m_dst.x += (int)m_velX; // Had to cast it to int to get crisp collision with side of platform.
	// Now do Y axis.
	m_velY += m_accelY + m_grav; // Adjust gravity to get slower jump.
	m_velY = std::min(std::max(m_velY, -(m_maxVelY)), (m_grav *2));
	m_dst.y += (int)m_velY; // If you run into issues with vertical collision, you can also cast to int.
	m_accelX = m_accelY = 0;
	if (IsGrounded() == false)
	{
		m_grav = GRAV;
	}

	if (EVMA::KeyHeld(SDL_SCANCODE_A))
		m_accelX -= 1;
	else if (EVMA::KeyHeld(SDL_SCANCODE_D))
		m_accelX += 1;

	if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && IsGrounded())
	{

		SetAccelY(-JUMPFORCE); // Sets the jump force.
		SetGrounded(false);
	}
	Speed(m_a, m_aMaxY);



	switch (m_state)
	{
	case idle:
		if (EVMA::KeyHeld(SDL_SCANCODE_A) || EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			SetState(running);
		}
		if (EVMA::KeyHeld(SDL_SCANCODE_SPACE))
		{
			SetState(jump);
		}

		break;
	case running:
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
		if (IsGrounded())
		{
			SetState(idle);
			break;
		}

		break;
	}
	
	Animate();
}

void Player::Render()
{
	SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), GetDstP(), m_angle, 0, static_cast<SDL_RendererFlip>(m_dir));
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
	else if(m_state == running)// Only other is running for now...
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
		m_spriteMax = 11;
	}
}

void Player::Collision()
{
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{

			if (Engine::Instance().GetLevel()[i][j]->IsObstacle() && COMA::AABBCheck(*this->GetDstP(), *Engine::Instance().GetLevel()[i][j]->GetDstP()))
			{
				if (this->GetDstP()->y + Engine::Instance().GetLevel()[i][j]->GetDstP()->h - (float)this->GetVelY() <= (float)Engine::Instance().GetLevel()[i][j]->GetDstP()->y)
				{ // Colliding top side of platform.
					this->SetGrounded(true);
					this->StopY();
					this->SetY(Engine::Instance().GetLevel()[i][j]->GetDstP()->y - this->GetDstP()->h);
					if (IsGrounded() == true)
					{
						m_grav = 0;
					}
				}
				else if (this->GetDstP()->y - (float)this->GetVelY() >= Engine::Instance().GetLevel()[i][j]->GetDstP()->y + Engine::Instance().GetLevel()[i][j]->GetDstP()->h)
				{ // Colliding bottom side of platform.
					this->StopY();
					this->SetY(Engine::Instance().GetLevel()[i][j]->GetDstP()->y + Engine::Instance().GetLevel()[i][j]->GetDstP()->h);
				}
				else if (this->GetDstP()->x + this->GetDstP()->w - this->GetVelX() <= Engine::Instance().GetLevel()[i][j]->GetDstP()->x)
				{ // Collision from left.
					this->StopX(); // Stop the player from moving horizontally.
					this->SetX(Engine::Instance().GetLevel()[i][j]->GetDstP()->x - this->GetDstP()->w);
				}
				else if (this->GetDstP()->x - (float)this->GetVelX() >= Engine::Instance().GetLevel()[i][j]->GetDstP()->x + Engine::Instance().GetLevel()[i][j]->GetDstP()->w)
				{ // Colliding right side of platform.
					this->StopX();
					this->SetX(Engine::Instance().GetLevel()[i][j]->GetDstP()->x + Engine::Instance().GetLevel()[i][j]->GetDstP()->w);
				}
			}

		}
	}
}

void Player::Stop() // If you want a dead stop both axes.
{
	StopX();
	StopY();
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

void Player::AddAccelX(double a)
{
	m_accelX = m_accelX + a;
}

void Player::SetMaxVel(double a)
{
	m_maxVelX = a;
}
