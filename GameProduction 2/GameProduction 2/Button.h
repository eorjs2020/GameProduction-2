#pragma once
#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Sprite.h"
#include <SDL.h>

// Button will be an example of the command design pattern.
class Button : public Sprite
{
public:
	int Update();
	void Render();
protected: // Private but inherited!
	Button(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t);
	enum state { STATE_UP, STATE_OVER, STATE_DOWN } m_state;
	bool MouseCollision();
	virtual void Execute() = 0; // Pure virtual method, meaning Button is now an abstract class (interface)!
};

// List button subclasses here...
class PlayButton : public Button
{
private:
	void Execute();
public:
	PlayButton(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t);
};
class QuitButton : public Button
{
private:
	void Execute();
public:
	QuitButton(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t);
};
class PauseButton : public Button
{
private:
	void Execute();
public:
	PauseButton(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t);
};
class ResumeButton : public Button
{
private:
	void Execute();
public:
	ResumeButton(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t);
};
class MainMenuButton :public Button
{
private:
	void Excute();
public:
	MainMenuButton(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t);
};
class TutorialButton :public Button
{
private:
	void Excute();
public:
	TutorialButton(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t);
};
#endif