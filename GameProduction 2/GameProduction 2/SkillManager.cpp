#include "Engine.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "SkillManager.h"

void Skill::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}

Invis::Invis()
{
}

Invis::~Invis()
{
}

int Invis::Update()
{
	return 0;
}

void Invis::Render()
{
}

void Invis::Execute()
{
}

SpeedBoost::SpeedBoost()
{
}

SpeedBoost::~SpeedBoost()
{
}

int SpeedBoost::Update()
{
	return 0;
}

void SpeedBoost::Render()
{
}

void SpeedBoost::Execute()
{
}
