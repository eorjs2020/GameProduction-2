#include "Engine.h"
#include "CollisionManager.h"
#include "DebugManager.h"
#include "EventManager.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "StateManager.h"
#include "TextureManager.h"
#include <iostream>
#include <fstream>

using namespace std;

Engine::Engine():m_running(false){ 
	cout << "Engine class constructed!" << endl; 
	m_pPause = false;
}

bool Engine::Init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	cout << "Initializing game..." << endl;
	// Attempt to initialize SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		// Create the window.
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (m_pWindow != nullptr) // Window init success.
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			if (m_pRenderer != nullptr) // Renderer init success.
			{
				EVMA::Init();
				SOMA::Init();
				TEMA::Init();

			}
			else return false; // Renderer init fail.
		}
		else return false; // Window init fail.
	}
	else return false; // SDL init fail.
	m_fps = (Uint32)round((1 / (double)FPS) * 1000); // Sets FPS in milliseconds and rounds.
	TEMA::RegisterTexture("Img/startButton.png", "play");
	TEMA::RegisterTexture("Img/quitButton.png", "quit");
	TEMA::RegisterTexture("Img/pauseButton.png", "pause");
	TEMA::RegisterTexture("Img/resumeButton.png", "resume");
	TEMA::RegisterTexture("Img/tutorialButton.png", "tutorial");
	TEMA::RegisterTexture("Img/mainmenuButton.png", "mainmenu");
	TEMA::RegisterTexture("Img/exit.png", "end");
	TEMA::RegisterTexture("Img/Concept2.png", "player");
	TEMA::RegisterTexture("Img/Concept.png", "Enemy");
	TEMA::RegisterTexture("Img/startBackground.png", "title");
	TEMA::RegisterTexture("Img/fireball.png", "fireball");
	TEMA::RegisterTexture("Img/tilemap1.png", "tilemap1");
	TEMA::RegisterTexture("Img/CharacterWalk.png", "playerWalk");
	TEMA::RegisterTexture("Img/CharacterJumpFall.png", "playerJump");
	TEMA::RegisterTexture("Img/CharacterIdle.png", "playerIdle");
	TEMA::RegisterTexture("Img/DroneIdle.png", "droneIdle");
	TEMA::RegisterTexture("Img/LightCast_96.png", "lightEffect");
	TEMA::RegisterTexture("Img/Kit.png", "interface");
	TEMA::RegisterTexture("Img/battery.png", "battery");
	TEMA::RegisterTexture("Img/Assets.png", "Key");
	TEMA::RegisterTexture("Img/skills.png", "skill");
	TEMA::RegisterTexture("Img/DroneFireIdle.png", "firedrone");
	TEMA::RegisterTexture("Img/DroneBullet.png", "dronebullet");
	TEMA::RegisterTexture("Img/CharacterWalkBoost.png", "playerboost");
	TEMA::RegisterTexture("Img/CharacterWalkInvisible2.png", "playerinvis");
	TEMA::RegisterTexture("Img/CharacterFall.png", "playerfall");
	TEMA::RegisterTexture("Img/GH.png", "hook");
	TEMA::RegisterTexture("Img/flashlight.png", "light");
	TEMA::RegisterTexture("Img/Barrier.png", "barrier");
	
	FOMA::RegisterFont("Img/LTYPE.TTF", "font1", 20);
	//STMA::ChangeState(new TitleState);
	STMA::ChangeState(new Level2State);
	SOMA::AllocateChannels(16);
	
	m_running = true; // Everything is okay, start the engine.
	cout << "Engine Init success!" << endl;
	return true;
}

void Engine::Wake()
{
	m_start = SDL_GetTicks();
}

void Engine::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start;
	if (m_delta < m_fps) // Engine has to sleep.
		SDL_Delay(m_fps - m_delta);
}

void Engine::HandleEvents()
{
	EVMA::HandleEvents();
}

void Engine::Update()
{
	
	STMA::Update();

}

void Engine::Render() 
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer); // Clear the screen with the draw color.
	// Draw anew.
	STMA::Render();
	SDL_RenderPresent(m_pRenderer);
	if (m_exit == true)
	{
		STMA::ChangeState(new EndState);
	}
}

void Engine::Clean()
{
	cout << "Cleaning game." << endl;
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			delete m_level[row][col];
			m_level[row][col] = nullptr; // Wrangle your dangle.
		}
	}
	for (auto const& i : m_tiles)
		delete m_tiles[i.first];
	m_tiles.clear();
	DEMA::Quit();
	EVMA::Quit();
	FOMA::Quit();
	SOMA::Quit();
	STMA::Quit();
	TEMA::Quit();
	IMG_Quit();
	SDL_Quit();
}

int Engine::Run()
{
	if (m_running) // What does this do and what can it prevent?
		return -1; 
	if (Init("GAME1017 Engine Template", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0) == false)
		return 1;
	while (m_running) // Main engine loop.
	{
		Wake();
		HandleEvents();
		Update();
		Render();
		if (m_running)
			Sleep();
	}
	Clean();
	return 0;
}

Engine& Engine::Instance()
{
	static Engine instance; // C++11 will prevent this line from running more than once. Magic statics.
	return instance;
}

SDL_Renderer* Engine::GetRenderer() { return m_pRenderer; }
bool& Engine::Running() { return m_running; }
bool& Engine::End() { return m_exit; }

bool& Engine::Pause()
{
	return m_pPause;
}
