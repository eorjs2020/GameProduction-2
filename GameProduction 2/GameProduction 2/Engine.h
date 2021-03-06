#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Label.h"
#include <map>
#include <array>
#include <vector>
#include "Tile.h"
#include "Enemy.h"
#include "Define.h"

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
	std::vector<Tile*> m_hazard;
	std::vector<Tile*> m_platforms2;
	std::vector<Enemy*> m_enemies;
	bool m_pPause, m_invis = false;
	int m_score, m_scorestate;

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
	std::vector<Tile*>& GetHazard() { return m_hazard; }
	std::vector<Enemy*>& GetEnemy() { return m_enemies; }
	bool& Pause();
	std::vector<Tile*>& GetPlatform2() { return m_platforms2; }
	bool& getinvis() { return m_invis; }
	void setinvis(bool a) { m_invis = a; }
	void setScore(int a) { m_score += a; }
	int getScore() { return m_score; }
	void setScoreState(int a) { m_scorestate += a; }
	int getScoreState() { return m_scorestate; }
};
#endif