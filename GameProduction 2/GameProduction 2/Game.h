#pragma once



#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

class Game
{
private:
	bool m_bRunning;
	const Uint8* m_iKeystates;
	Uint32 m_start, m_end, m_delta, m_fps;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
private:
	bool Init(const char* title, int xposition, int yposition, int width, int height, int flags);
	void Wake();
	void Sleep();
	void Events();
	void Update();
	void Render();
	void Clean();
public:
	Game();
	~Game();
	int Run();
	//static Game& Instance();
	bool KeyDown(SDL_Scancode c);
	SDL_Renderer* GetRenderer();
};

