#include "Player.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "TextureManager.h"
#define SPEED 2

Player::Player(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf)
	:AnimatedSprite(s, d, r, t, sstart, smin, smax, nf), m_state(idle), m_dir(0) {

	m_grounded = false;
	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	m_maxVelX = 10.0;
	m_maxVelY = JUMPFORCE;
	m_grav = GRAV;
	m_drag = 0.88;


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
	m_velY = std::min(std::max(m_velY, -(m_maxVelY)), (m_grav * 3));
	m_dst.y += m_velY; // If you run into issues with vertical collision, you can also cast to int.
	m_accelX = m_accelY = 0.0;
	
	
	switch (m_state)
	{
	case idle:
		if (EVMA::KeyHeld(SDL_SCANCODE_A) || EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			SetState(running);
		}
		if (EVMA::KeyHeld(SDL_SCANCODE_SPACE))
			SetState(jump);
		break;
	case running:
		if (EVMA::KeyReleased(SDL_SCANCODE_A) || EVMA::KeyReleased(SDL_SCANCODE_D))
		{
			SetState(idle);
			break; // Skip movement parsing below.
		}
		if (EVMA::KeyHeld(SDL_SCANCODE_A))
		{
			if (m_dst.x > 0 && !COMA::PlayerCollision({ (int)m_dst.x, (int)m_dst.y, (int)32, (int)32 }, -1, 0))
			{
				this->SetAccelX(-1.0);
				m_dir = 1;
			}
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			if (m_dst.x < 1024 - 32 && !COMA::PlayerCollision({ (int)m_dst.x, (int)m_dst.y, (int)32, (int)32 }, 1, 0))
			{
				this->SetAccelX(1.0);
				m_dir = 0;
			}
			
		}
		break;
	case jump:
		if (EVMA::KeyHeld(SDL_SCANCODE_SPACE))
		m_accelX = -JUMPFORCE; // Sets the jump force.
		m_grounded = false;
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
		m_spriteMax = 5;
	}
	else if(m_state == running)// Only other is running for now...
	{
		m_pText = TEMA::GetTexture("playerWalk");
		m_sprite = m_spriteMin = 0;
		m_spriteMax = 6;
	}
	else if (m_state == jump)
	{
		m_pText = TEMA::GetTexture("playerJump");
		m_sprite = 0;
		m_spriteMin = 0;
		m_spriteMax = 11;
	}
}

void Player::SetAccelX(double a)
{
	m_dst.x = a; 
}

void Player::SetAccelY(double a)
{
	m_dst.y = a;
}

