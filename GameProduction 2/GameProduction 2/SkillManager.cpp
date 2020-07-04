#include "Engine.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "SkillManager.h"


bool m_speedBoost = false;
int m_speedBoostTimer = 0;

void Speed(double* a, double* b)
{
	double * m_accelX = a, *m_mMaxY = b;
	

	if (EVMA::KeyPressed(SDL_SCANCODE_1))
	{
		m_speedBoost = true;
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
	}
}