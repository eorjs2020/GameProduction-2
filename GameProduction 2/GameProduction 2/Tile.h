#pragma once
#ifndef _TILE_H_
#define _TILE_H_
#include "Sprite.h"
#include "glm.hpp"


class Tile : public Sprite
{
public:
	Tile(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, const bool o, const bool h)
		:Sprite(s, d, r, t), m_obstacle(o), m_hazard(h) {
		position.x = d.x;
		position.y = d.y;
	}
	bool IsObstacle() { return m_obstacle; }
	bool IsHazard() { return m_hazard; }
	void Update(SDL_Rect rect)
	{ 
		m_dst.x = position.x - rect.x;
		m_dst.y = position.y - rect.y;
	}
	void Render() {
		SDL_FRect drawing = { m_dst.x, m_dst.y, m_dst.w, m_dst.h };
		SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), &drawing, m_angle, 0, SDL_FLIP_NONE); };
	SDL_FRect Position() { return position; }
	Tile* Clone() { return new Tile(m_src, m_dst, m_pRend, m_pText, m_obstacle, m_hazard); }
private:
	bool m_obstacle,
		m_hazard;
	SDL_FRect position;
};
#endif