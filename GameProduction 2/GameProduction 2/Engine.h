#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_
#define ROWS 73
#define COLS 171
#define WIDTH 1024
#define HEIGHT 768
#define FPS 60
#define ROWS2 51
#define COLS2 419
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Label.h"
#include <map>
#include <array>
#include <vector>
#include "Tile.h"

// Do not include any macros, initialized properties or full function definitions in this header.

class Engine
{
private: // Private properties.
	bool m_exit = false;
	bool m_running; // Loop control flag.
	Uint32 m_start, m_end, m_delta, m_fps; // Fixed timestep variables.
	SDL_Window* m_pWindow; // This represents the SDL window.
	SDL_Renderer* m_pRenderer; // This represents the buffer to draw to.
	std::map<char, Tile*> m_tiles;
	std::array<std::array<Tile*, COLS>, ROWS> m_level;
	std::array<std::array<Tile*, COLS2>, ROWS2> m_level2;
	std::vector<Tile*> m_platforms;
	std::vector<Tile*> m_platforms2;
private: // Private methods.
	Engine();
	bool Init(const char* title, int xpos, int ypos, int width, int height, int flags);
	void Wake();
	void Sleep();
	void HandleEvents();
	void Update();
	void Render();
	void Clean();
public: // Public methods.
	int Run();
	static Engine& Instance(); // This static method creates the static instance that can be accessed 'globally'
	SDL_Renderer* GetRenderer();
	bool& Running();
	bool& End();
	std::array<std::array<Tile*, COLS>, ROWS>& GetLevel() { return m_level; }
	std::array<std::array<Tile*, COLS2>, ROWS2>& GetLevel2() { return m_level2; }
	std::map<char, Tile*>& GetTiles() { return m_tiles; }
	std::vector<Tile*>& GetPlatform() { return m_platforms; }
	std::vector<Tile*>& GetPlatform2() { return m_platforms2; }
};
#endif