#include "Engine.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "SkillManager.h"


SpeedBoost::SpeedBoost()
{
	m_speedBoost = false;
	m_speedBoostTimer = 0;
	iconA = new AnimatedSprite({ 0,97,22,22 }, { 400.0f,724.0f,22.0f,22.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("skill"), 0, 1, 1, 1);
}

void SpeedBoost::Update(double* a, double* b, double* c, float* x, float* y)
{
	double * m_accelX = a, *m_mMaxY = b, *m_energy = c;
	

	if (EVMA::KeyPressed(SDL_SCANCODE_1) && *m_energy > 0)
	{
		m_speedBoost = true;
		*m_energy -= 5;	
		/*effect = new AnimatedSprite({ 0,0,96,96 }, { *x, *y,32.0f,32.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("lightEffect"), 0, 28, 28, 4);*/
	}
	if (m_speedBoost == true)
	{
		*m_mMaxY = 10;
		++m_speedBoostTimer;
		if (*m_accelX > 0)
		*m_accelX += 3.0;
		if (*m_accelX < 0)
			*m_accelX -= 3.0;
		iconA->Animate();
	}
	if (m_speedBoostTimer >= 30)
	{
		m_speedBoost = false;
		m_speedBoostTimer = 0;
		*m_mMaxY = 5;
		/*effect = nullptr;*/
		iconA->SetSrcP(0, 97);
	}
}

void SpeedBoost::Render()
{	
	/*if (effect != nullptr) {
		effect->Render();
		effect->Animate();
	}*/
	if (iconA != nullptr)
		iconA->Render();
}

invisibility::invisibility()
{
	iconB = new AnimatedSprite({ 0,119,22,22 }, { 423.0f,724.0f,22.0f,22.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("skill"), 0, 1, 1, 1);
}

void invisibility::Update(double* c)
{
	
	double* m_energy = c;
	if (EVMA::KeyPressed(SDL_SCANCODE_2) && *m_energy > 0)
	{
		Engine::Instance().setinvis(true);
		*m_energy -= 5;
		m_invis = true;
	}
	if (Engine::Instance().getinvis() == true)
	{
		++m_invisTimer;
		iconB->Animate();
	}
	if (m_invisTimer >= 200)
	{
		Engine::Instance().setinvis(false);
		m_invisTimer = 0;
		iconB->SetSrcP(0, 119);
		m_invis = true;
	}
}

void invisibility::Render()
{
	if (iconB != nullptr)
		iconB->Render();
}

doubleJump::doubleJump()
{
	iconC = new AnimatedSprite({ 0,141,22,22 }, { 446.0f,724.0f,22.0f,22.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("skill"), 0, 1, 1, 1);
}

void doubleJump::Update(double* c)
{
	double* m_energy = c;
	if (EVMA::KeyPressed(SDL_SCANCODE_3) && *m_energy > 0 && m_doubleJumpCoolDown == false)
	{
		*m_energy -= 5;
		m_doubleJump = true;
	}
	if (m_doubleJump == true)
	{
		iconC->Animate();
		++m_doubleJumpTimer;
	}
	if (m_doubleJumpTimer >= 200)
	{
		m_doubleJumpTimer = 0;
		iconC->SetSrcP(0, 119);
		m_doubleJump = false;
	}
	
}

void doubleJump::Render()
{
	if (iconC != nullptr)
		iconC->Render();
}
