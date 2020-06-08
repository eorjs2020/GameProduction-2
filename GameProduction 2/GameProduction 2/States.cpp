#include "States.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "StateManager.h" // Make sure this is NOT in "States.h" or circular reference.
#include "TextureManager.h"
#include "Engine.h"
#include "Button.h"
#include <iostream>

// Begin State. CTRL+M+H and CTRL+M+U to turn on/off collapsed code.
void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}
void State::Resume() {}
// End State.

// Begin GameState.
GameState::GameState() {}

void GameState::Enter()
{
	
	std::cout << "Entering GameState..." << std::endl;
	m_pPlayer = new PlatformPlayer({ 10,-2,120,120 }, { 512.0f,468.0f,80,80 }, 
		Engine::Instance().GetRenderer(), TEMA::GetTexture("player"));
	m_pEnemy = new PlatformEnemy({ 0,0,128,128 }, { 19.0f,19.0f,80,80 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("Enemy"));
	m_quitBtn = new QuitButton({ 0,0,480,140 }, { 0.0f,0.0f,240.0f,70.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("quit"));
	m_pPlatforms[0] = new SDL_FRect({ -100.0f,668.0f,1224.0f,100.0f });
	m_pPlatforms[1] = new SDL_FRect({ 200.0f,468.0f,100.0f,20.0f });
	m_pPlatforms[2] = new SDL_FRect({ 724.0f,468.0f,100.0f,20.0f });
	m_pPlatforms[3] = new SDL_FRect({ 462.0f,368.0f,100.0f,20.0f });
	m_pPlatforms[4] = new SDL_FRect({ 462.0f,648.0f,100.0f,20.0f });
	SOMA::Load("Aud/power.wav", "beep", SOUND_SFX);
	SOMA::Load("Aud/Space_Loop1.wav", "BGM", SOUND_MUSIC);
	SOMA::Load("Aud/jump.wav", "jump", SOUND_SFX);
	SOMA::PlayMusic("BGM");
	
}

void GameState::Update()
{
	m_pMusicVolume = m_pMusicSetVol;
	m_pSFXVolume = m_pSFXSetVol;
	SOMA::SetSoundVolume(m_pSFXVolume);
	SOMA::SetMusicVolume(m_pMusicVolume);
	//SOMA::PlayMusic("BGM");
	//Grappling Hook input
	if (EVMA::MouseHeld(1))
	{
		if (existHook == false)
		{
			float px = m_pPlayer->GetDstP()->x, py = m_pPlayer->GetDstP()->y;
			m_pHook = new GrapplingHook({ 10,-2,10,10 }, { px, py,30,30 },
				Engine::Instance().GetRenderer(), TEMA::GetTexture("fireball"), 0.00);
			m_pHook->SetExist(true);
			existHook = true;
		}
		if (hookColl == true)
		{
			double h = 1, o, a;
			m_pPlayer->SetGrav(0.0);
			double pA = MAMA::AngleBetweenPoints((m_pHook->GetDstP()->y - m_pPlayer->GetDstP()->y),
				(m_pHook->GetDstP()->x - m_pPlayer->GetDstP()->x));
			double pD = MAMA::Distance((m_pPlayer->GetDstP()->x + m_pPlayer->GetDstP()->w / 2.0f), (m_pHook->GetDstP()->x),
				(m_pPlayer->GetDstP()->y + m_pPlayer->GetDstP()->h / 2.0f), (m_pHook->GetDstP()->y));
			a = MAMA::SetDeltaX(pA, h);
			o = MAMA::SetDeltaY(pA, h);
			m_pPlayer->SetAccelX(a);
			m_pPlayer->SetAccelY(o);
			m_pPlayer->SetAccelX(a);
			m_pPlayer->SetAccelY(o);
		}
	}
	else if (EVMA::MouseReleased(1))
		{
			//delete m_pHook;
			m_pPlayer->SetGrav(6.0);
			m_pPlayer->Stop();
			existHook = false;
			hookColl = false;
		}
	// Get input.
	if (EVMA::KeyHeld(SDL_SCANCODE_A))
		m_pPlayer->SetAccelX(-1.0);
	else if (EVMA::KeyHeld(SDL_SCANCODE_D))
		m_pPlayer->SetAccelX(1.0);
	else if (EVMA::KeyHeld(SDL_SCANCODE_X))
	{
		State::Resume();
		STMA::PushState(new EndState);
	}
	else if (EVMA::KeyHeld(SDL_SCANCODE_PAGEUP))
	{
		++m_pMusicSetVol;
		++m_pSFXVolume;
	}
	else if (EVMA::KeyHeld(SDL_SCANCODE_PAGEDOWN))
	{
		--m_pMusicSetVol;
		--m_pSFXVolume;
	}
	if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_pPlayer->IsGrounded())
	{
		SOMA::PlaySound("jump");
		m_pPlayer->SetAccelY(-JUMPFORCE); // Sets the jump force.
		m_pPlayer->SetGrounded(false);
	}
	if (m_quitBtn->Update() == 1)
		return;
	// Wrap the player on screen.
	if (m_pPlayer->GetDstP()->x < -51.0) m_pPlayer->SetX(1024.0);
	else if (m_pPlayer->GetDstP()->x > 1024.0) m_pPlayer->SetX(-50.0);
	// Wrap the Enemy on screen.
	if (m_pEnemy->GetDstP()->x < -51.0) m_pEnemy->SetX(1024.0);
	else if (m_pEnemy->GetDstP()->x > 1024.0) m_pEnemy->SetX(-50.0);
	// Do the rest.
	m_pPlayer->Update();
	m_pEnemy->Update();
	CheckCollision();
	if (existHook == true)
	{
		m_pHook->Update();
		CheckCollisionHook();
	}	
}

void GameState::CheckCollision()
{
	for (int i = 0; i < NUMPLATFORMS; i++) // For each platform.
	{
		if (COMA::AABBCheck(*m_pPlayer->GetDstP(), *m_pPlatforms[i]))
		{
			if (m_pPlayer->GetDstP()->y + m_pPlayer->GetDstP()->h - (float)m_pPlayer->GetVelY() <= m_pPlatforms[i]->y)
			{ // Colliding top side of platform.
				m_pPlayer->SetGrounded(true);
				m_pPlayer->StopY();
				m_pPlayer->SetY(m_pPlatforms[i]->y - m_pPlayer->GetDstP()->h);
			}
			else if (m_pPlayer->GetDstP()->y - (float)m_pPlayer->GetVelY() >= m_pPlatforms[i]->y + m_pPlatforms[i]->h)
			{ // Colliding bottom side of platform.
				m_pPlayer->StopY();
				m_pPlayer->SetY(m_pPlatforms[i]->y + m_pPlatforms[i]->h);
			}
			else if (m_pPlayer->GetDstP()->x + m_pPlayer->GetDstP()->w - m_pPlayer->GetVelX() <= m_pPlatforms[i]->x)
			{ // Collision from left.
				m_pPlayer->StopX(); // Stop the player from moving horizontally.
				m_pPlayer->SetX(m_pPlatforms[i]->x - m_pPlayer->GetDstP()->w);
			}
			else if (m_pPlayer->GetDstP()->x - (float)m_pPlayer->GetVelX() >= m_pPlatforms[i]->x + m_pPlatforms[i]->w)
			{ // Colliding right side of platform.
				m_pPlayer->StopX();
				m_pPlayer->SetX(m_pPlatforms[i]->x + m_pPlatforms[i]->w);
			}
		}
	}
	if (COMA::AABBCheck(*m_pEnemy->GetDstP(), *m_pPlayer->GetDstP()))
	{
		if (m_pPlayer->GetDstP()->y + m_pPlayer->GetDstP()->h - (float)m_pPlayer->GetVelY() <= (float)m_pEnemy->GetDstP()->y)
		{ // Colliding top side of platform.
			m_pPlayer->SetGrounded(true);
			m_pPlayer->StopY();
			m_pPlayer->SetY(m_pEnemy->GetDstP()->y - m_pPlayer->GetDstP()->h);
		}
		else if (m_pPlayer->GetDstP()->y - (float)m_pPlayer->GetVelY() >= m_pEnemy->GetDstP()->y + m_pEnemy->GetDstP()->h)
		{ // Colliding bottom side of platform.
			m_pPlayer->StopY();
			m_pPlayer->SetY(m_pEnemy->GetDstP()->y + m_pEnemy->GetDstP()->h);
		}
		else if (m_pPlayer->GetDstP()->x + m_pPlayer->GetDstP()->w - m_pPlayer->GetVelX() <= m_pEnemy->GetDstP()->x)
		{ // Collision from left.
			m_pPlayer->StopX(); // Stop the player from moving horizontally.
			m_pPlayer->SetX(m_pEnemy->GetDstP()->x - m_pPlayer->GetDstP()->w);
		}
		else if (m_pPlayer->GetDstP()->x - (float)m_pPlayer->GetVelX() >= m_pEnemy->GetDstP()->x + m_pEnemy->GetDstP()->w)
		{ // Colliding right side of platform.
			m_pPlayer->StopX();
			m_pPlayer->SetX(m_pEnemy->GetDstP()->x + m_pEnemy->GetDstP()->w);
		}
	}


	for (int i = 0; i < NUMPLATFORMS; i++) // For each platform.
	{
		if (COMA::AABBCheck(*m_pEnemy->GetDstP(), *m_pPlatforms[i]))
		{
			if (m_pEnemy->GetDstP()->y + m_pEnemy->GetDstP()->h - (float)m_pEnemy->GetVelY() <= m_pPlatforms[i]->y)
			{ // Colliding top side of platform.
				m_pEnemy->SetGrounded(true);
				m_pEnemy->StopY();
				m_pEnemy->SetY(m_pPlatforms[i]->y - m_pEnemy->GetDstP()->h);
			}
			else if (m_pEnemy->GetDstP()->y - (float)m_pEnemy->GetVelY() >= m_pPlatforms[i]->y + m_pPlatforms[i]->h)
			{ // Colliding bottom side of platform.
				m_pEnemy->StopY();
				m_pEnemy->SetY(m_pPlatforms[i]->y + m_pPlatforms[i]->h);
			}
			else if (m_pEnemy->GetDstP()->x + m_pEnemy->GetDstP()->w - m_pEnemy->GetVelX() <= m_pPlatforms[i]->x)
			{ // Collision from left.
				m_pEnemy->StopX(); // Stop the player from moving horizontally.
				m_pEnemy->SetX(m_pPlatforms[i]->x - m_pEnemy->GetDstP()->w);
			}
			else if (m_pEnemy->GetDstP()->x - (float)m_pEnemy->GetVelX() >= m_pPlatforms[i]->x + m_pPlatforms[i]->w)
			{ // Colliding right side of platform.
				m_pEnemy->StopX();
				m_pEnemy->SetX(m_pPlatforms[i]->x + m_pPlatforms[i]->w);
			}
		}
	}
}

void GameState::CheckCollisionHook()
{
	for (int i = 0; i < NUMPLATFORMS; i++) // For each platform.
	{
		if (COMA::AABBCheck(*m_pHook->GetDstP(), *m_pPlatforms[i]))
		{
			if (m_pHook->GetDstP()->y + m_pHook->GetDstP()->h - (float)m_pHook->GetVelY() <= m_pPlatforms[i]->y)
			{ // Colliding top side of platform.
				m_pHook->Stop();
				m_pHook->SetY(m_pPlatforms[i]->y - m_pHook->GetDstP()->h);
				hookColl = true;
			}
			else if (m_pHook->GetDstP()->y - (float)m_pHook->GetVelY() >= m_pPlatforms[i]->y + m_pPlatforms[i]->h)
			{ // Colliding bottom side of platform.
				m_pHook->Stop();
				m_pHook->SetY(m_pPlatforms[i]->y + m_pPlatforms[i]->h);
				hookColl = true;
			}
			else if (m_pHook->GetDstP()->x + m_pHook->GetDstP()->w - m_pHook->GetVelX() <= m_pPlatforms[i]->x)
			{ // Collision from left.
				m_pHook->Stop(); // Stop the player from moving horizontally.
				m_pHook->SetX(m_pPlatforms[i]->x - m_pHook->GetDstP()->w);
				hookColl = true;
			}
			else if (m_pHook->GetDstP()->x - (float)m_pHook->GetVelX() >= m_pPlatforms[i]->x + m_pPlatforms[i]->w)
			{ // Colliding right side of platform.
				m_pHook->Stop();
				m_pHook->SetX(m_pPlatforms[i]->x + m_pPlatforms[i]->w);
				hookColl = true;
			}
		}
	}
}

void GameState::Render()
{
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 64, 128, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	SDL_RenderCopy(Engine::Instance().GetRenderer(), TEMA::GetTexture("title"), nullptr, nullptr);
	// Draw the player.
	m_pPlayer->Render();
	m_pEnemy->Render();
	//draw the hook
	if (existHook == true)
	{
		m_pHook->Render();
	}
	// Draw the platforms.
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 192, 64, 0, 255);
	for (int i = 0; i < NUMPLATFORMS; i++)
		SDL_RenderFillRectF(Engine::Instance().GetRenderer(), m_pPlatforms[i]);
	// If GameState != current state.
	m_quitBtn->Render();
	if (dynamic_cast<GameState*>(STMA::GetStates().back()))
		State::Render();
}

void GameState::Exit()
{
	delete m_pEnemy;
	delete m_pPlayer;
	for (int i = 0; i < NUMPLATFORMS; i++)
		delete m_pPlatforms[i];
}

void GameState::Resume() { }
// End GameState.

// Begin TitleState.
TitleState::TitleState() {

	
}

void TitleState::Enter()
{
	
	m_playBtn = new PlayButton({ 0,0,480,140 }, { 380.0f,350.0f,240.0f,70.0f }, 
		Engine::Instance().GetRenderer(), TEMA::GetTexture("play"));
	m_quitBtn = new QuitButton({ 0,0,480,140 }, { 380.0f,420.0f,240.0f,70.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("quit"));
	SOMA::Load("Aud/power.wav", "beep", SOUND_SFX);
	SOMA::Load("Aud/Space_Loop1.wav", "BGM", SOUND_MUSIC);
	SOMA::SetMusicVolume(16);
	SOMA::SetSoundVolume(20);
	SOMA::PlayMusic("BGM");
}

void TitleState::Update()
{
	
	if (m_playBtn->Update() == 1)
		return;
	if (m_quitBtn->Update() == 1)
		return; 
}

void TitleState::Render()
{
	SDL_RenderCopy(Engine::Instance().GetRenderer(), TEMA::GetTexture("title"), nullptr, nullptr);
	
	
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 128, 0, 255, 255);
	//SDL_RenderClear(Engine::Instance().GetRenderer());
	m_playBtn->Render();
	m_quitBtn->Render();
	State::Render();
}

void TitleState::Exit()
{
	std::cout << "Exiting TitleState..." << std::endl;
}
// End TitleState.
// Begin EndState.
EndState::EndState() {}

void EndState::Enter()
{
	m_playBtn = new QuitButton({ 0,0,480,140 }, { 380.0f,420.0f,240.0f,70.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("quit"));
	SOMA::Load("Aud/power.wav", "beep", SOUND_SFX);
}

void EndState::Update()
{
	if (m_playBtn->Update() == 1)
		return;
}

void EndState::Render()
{
	SDL_RenderCopy(Engine::Instance().GetRenderer(), TEMA::GetTexture("title"), nullptr, nullptr);
	/*SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 128, 0, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());*/
	m_playBtn->Render();
	State::Render();
}

void EndState::Exit()
{
	std::cout << "Exiting EndState..." << std::endl;
}
// End TitleState.