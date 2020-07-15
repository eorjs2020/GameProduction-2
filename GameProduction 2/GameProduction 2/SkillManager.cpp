#include "Engine.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "SkillManager.h"


SpeedBoost::SpeedBoost()
{
	m_speedBoost = false;
	m_speedBoostTimer = 0;
}

void SpeedBoost::Update(double* a, double* b, double* c, float* x, float* y)
{
	double * m_accelX = a, *m_mMaxY = b, *m_energy = c;

	if (EVMA::KeyPressed(SDL_SCANCODE_1) && *m_energy > 0)
	{
		m_speedBoost = true;
		*m_energy -= 5;	
		effect = new AnimatedSprite({ 0,0,96,96 }, { *x, *y,32.0f,32.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("lightEffect"), 0, 28, 28, 4);
	}
	if (m_speedBoost == true)
	{
		*m_mMaxY = 10;
		++m_speedBoostTimer;
		if (*m_accelX > 0)
		*m_accelX += 3.0;
		if (*m_accelX < 0)
			*m_accelX -= 3.0;
	}
	if (m_speedBoostTimer >= 30)
	{
		m_speedBoost = false;
		m_speedBoostTimer = 0;
		*m_mMaxY = 5;
		effect = nullptr;
	}
}

void SpeedBoost::Render()
{	
	if (effect != nullptr) {
		effect->Render();
		effect->Animate();
	}
}

invisibility::invisibility()
{
}

void invisibility::Update(double* c)
{
	double* m_energy = c;
	if (EVMA::KeyPressed(SDL_SCANCODE_2) && *m_energy > 0)
	{
		Engine::Instance().setinvis(true);
		*m_energy -= 5;
	}
	if (Engine::Instance().getinvis() == true)
	{
		++m_invisTimer;
	}
	if (m_invisTimer >= 200)
	{
		Engine::Instance().setinvis(false);
		m_invisTimer = 0;
	}
}

void invisibility::Render()
{

}
