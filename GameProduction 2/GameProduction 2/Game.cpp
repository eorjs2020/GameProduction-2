#include "Game.h"
#include <iostream>
#define WIDTH 1024
#define HEIGHT 768
#define FPS 60

Game::Game():m_bRunning(false)
{
	std::cout << "Game class constructed!" << std::endl;
}
Game::~Game() 
{
}
bool Game::Init(const char* title, int xposition, int yposition, int width, int height, int flags)
{

	std::cout << "booting game" << std::endl;
	// Attempt to initialize SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		// Create the window.
		m_pWindow = SDL_CreateWindow(title, xposition, yposition, width, height, flags);
		if (m_pWindow != nullptr) // Window init success.
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0); //(SDL_window* window, int index, Uint32 flags)
			if (m_pRenderer != nullptr) // Renderer init success.
			{
				if (IMG_Init(IMG_INIT_PNG) != 0)
				{
					
				}
				else return false; // Img init fail.
			}
			else return false; // Renderer init fail.
		}
		else return false; // Window init fail.
	}
	else return false; // SDL init fail.
	m_fps = (Uint32)round((1 / (double)FPS) * 1000); // Sets FPS in milliseconds and rounds.
	m_iKeystates = SDL_GetKeyboardState(nullptr);
	m_bRunning = true; // Everything is okay, start the game.
	
	return true;
	
}

void Game::Wake()
{
	m_start = SDL_GetTicks();
}

void Game::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start;
	if (m_delta < m_fps)
	{
		SDL_Delay(m_fps - m_delta);
	}
}

void Game::Events()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: // User pressed window's 'x' button.
			m_bRunning = false;
			break;
		case SDL_KEYDOWN: // Try SDL_KEYUP instead.
			if (event.key.keysym.sym == SDLK_ESCAPE)
				m_bRunning = false;
			break;
		}
	}
}

void Game::Update()
{

}

void Game::Render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer); // Clear the screen with the draw color.
	// Render stuff.

	// Draw anew.
	SDL_RenderPresent(m_pRenderer);
}

void Game::Clean()
{
	std::cout << "Cleaning game." << std::endl;
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}

int Game::Run()
{
	if (m_bRunning) // What does this do and what can it prevent?
		return -1;
	if (Init("Cyber Chaser", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0) == false)
		return 1;
	while (m_bRunning) // Main engine loop.
	{
		Wake();
		Events();
		Update();
		Render();
		if (m_bRunning)
			Sleep();
	}
	Clean();
	return 0;
}

bool Game::KeyDown(SDL_Scancode c)
{
	if (m_iKeystates != nullptr)
	{
		if (m_iKeystates[c] == 1)
			return true;
		else
			return false;
	}
	
	return false;
}

SDL_Renderer* Game::GetRenderer()
{
	return m_pRenderer;
}
