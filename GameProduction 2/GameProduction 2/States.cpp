#include "States.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "FontManager.h"
#include "StateManager.h" // Make sure this is NOT in "States.h" or circular reference.
#include "TextureManager.h"
#include "Engine.h"
#include "Button.h"
#include <iostream>
#include <fstream>
// Begin State. CTRL+M+H and CTRL+M+U to turn on/off collapsed code.
void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}
void State::Resume() {}
// End State.

// Begin GameState.
Level1State::Level1State() {}

void Level1State::Enter()
{

	std::cout << "Entering Level1State..." << std::endl;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	m_pPlayer = new Player({ 0,0,19,26 }, { 50.0f,50.0f,46.0f,64.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("playerIdle"), 0, 0, 4, 4);
	m_hook = new GrapplingHook({ 10,-2,10,10 }, { m_pPlayer->GetDstP()->x, m_pPlayer->GetDstP()->y, 30, 30 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("fireball"), 0.00, m_pPlayer);
	
	ifstream inFile("map/TileDataLevel1.txt");
	if (inFile.is_open())
	{ // Create map of Tile prototypes.
		char key;
		int x, y;
		bool o, h;
		while (!inFile.eof())
		{
			inFile >> key >> x >> y >> o >> h;
			Engine::Instance().GetTiles().emplace(key, new Tile({ x * 128, y * 128, 127, 127 }, { 0,0,32,32 }, 
				Engine::Instance().GetRenderer(), TEMA::GetTexture("tilemap1"), o, h));
		}
	}
	
	inFile.close();

	inFile.open("map/Game_map.txt");
	if (inFile.is_open())
	{ // Build the level from Tile prototypes.
		char key;
		for (int row = 0; row < ROWS; row++)
		{
			for (int col = 0; col < COLS; col++)
			{
				inFile >> key;
				Engine::Instance().GetLevel()[row][col] = Engine::Instance().GetTiles()[key]->Clone(); // Prototype design pattern used.
				Engine::Instance().GetLevel()[row][col]->GetDstP()->x = (float)(32 * col);
				Engine::Instance().GetLevel()[row][col]->GetDstP()->y = (float)(32 * row);
				if (Engine::Instance().GetLevel()[row][col]->IsObstacle())
					Engine::Instance().GetPlatform().push_back(Engine::Instance().GetLevel()[row][col]);
			}
		}
	}
	inFile.close();
	
	SOMA::Load("Aud/power.wav", "beep", SOUND_SFX);
	SOMA::Load("Aud/background_music2.wav", "BGM", SOUND_MUSIC);
	SOMA::Load("Aud/jump.wav", "jump", SOUND_SFX);
	SOMA::Load("Aud/hook_extension3.wav", "throw", SOUND_SFX);
	SOMA::Load("Aud/hook_grappling2.wav", "grab", SOUND_SFX);
	SOMA::Load("Aud/hook_retraction3.wav", "retract", SOUND_SFX);
	FOMA::RegisterFont("Img/LTYPE.TTF", "Font_1", 30);
	SOMA::PlayMusic("BGM");
	
}

void Level1State::Update()
{
	SOMA::StopMusic();
	m_pMusicVolume = m_pMusicSetVol;
	m_pSFXVolume = m_pSFXSetVol;
	SOMA::SetSoundVolume(m_pSFXVolume);
	SOMA::SetMusicVolume(m_pMusicVolume);

	m_pPlayer->Update();
	m_hook->Update();
	m_pPlayer->Collision();
	m_hook->Collision();
	if (EVMA::KeyReleased(SDL_SCANCODE_P))
	{
		STMA::ChangeState(new Level2State);
	}
	
}

void Level1State::CheckCollisionHook()
{

}



void Level1State::Render()
{
	
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 64, 128, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	SDL_RenderCopy(Engine::Instance().GetRenderer(), TEMA::GetTexture("title"), nullptr, nullptr);
	// Draw the player.
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			Engine::Instance().GetLevel()[row][col]->Render();
		}
	}
	
	m_pPlayer->Render();
	
	//draw the hook
	if (m_hook->GetExist() == true)
		m_hook->Render();
	// If GameState != current state.
	if (dynamic_cast<Level1State*>(STMA::GetStates().back()))
		State::Render();
}

void Level1State::Exit()
{
	delete m_pPlayer;
	delete m_hook;
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			delete Engine::Instance().GetLevel()[row][col];
			Engine::Instance().GetLevel()[row][col] = nullptr; // Wrangle your dangle.
		}
	}
	for (auto const& i : Engine::Instance().GetTiles())
	{
		
		delete Engine::Instance().GetTiles()[i.first];
	}

	Engine::Instance().GetTiles().clear();
	

	Engine::Instance().GetPlatform().clear();
	
	
	std::cout << "Cleaning Level1" << endl;
	
	
}

void Level1State::Resume() { }
// End GameState.

// Begin Level 2
Level2State::Level2State()
{
}
void Level2State::Enter()
{

	std::cout << "Entering Level2State..." << std::endl;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	m_pPlayer = new Player({ 0,0,19,26 }, { 50.0f,50.0f,46.0f,64.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("playerIdle"), 0, 0, 4, 4);
	m_hook = new GrapplingHook({ 10,-2,10,10 }, { m_pPlayer->GetDstP()->x, m_pPlayer->GetDstP()->y, 30, 30 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("fireball"), 0.00, m_pPlayer);
	
	ifstream inFile("map/TileDataLevel2.txt");
	if (inFile.is_open())
	{ // Create map of Tile prototypes.
		char key;
		int x, y;
		bool o, h;
		while (!inFile.eof())
		{
			inFile >> key >> x >> y >> o >> h;
			Engine::Instance().GetTiles().emplace(key, new Tile({ x * 128, y * 128, 127, 127 }, { 0,0,32,32 },
				Engine::Instance().GetRenderer(), TEMA::GetTexture("tilemap1"), o, h));
		}
	}

	inFile.close();
	

	inFile.open("map/Level2.txt");
	if (inFile.is_open())
	{ // Build the level from Tile prototypes.
		char key;
		for (int row = 0; row < ROWS2; row++)
		{
			for (int col = 0; col < COLS2; col++)
			{
				inFile >> key;
				Engine::Instance().GetLevel2()[row][col] = Engine::Instance().GetTiles()[key]->Clone(); // Prototype design pattern used.
				Engine::Instance().GetLevel2()[row][col]->GetDstP()->x = (float)(32 * col);
				Engine::Instance().GetLevel2()[row][col]->GetDstP()->y = (float)(32 * row);
				if (Engine::Instance().GetLevel2()[row][col]->IsObstacle())
					Engine::Instance().GetPlatform().push_back(Engine::Instance().GetLevel2()[row][col]);
			}
		}
	}
	inFile.close();

	SOMA::Load("Aud/power.wav", "beep", SOUND_SFX);
	SOMA::Load("Aud/background_music2.wav", "BGM", SOUND_MUSIC);
	SOMA::Load("Aud/jump.wav", "jump", SOUND_SFX);
	SOMA::Load("Aud/hook_extension3.wav", "throw", SOUND_SFX);
	SOMA::Load("Aud/hook_grappling2.wav", "grab", SOUND_SFX);
	SOMA::Load("Aud/hook_retraction3.wav", "retract", SOUND_SFX);
	FOMA::RegisterFont("Img/LTYPE.TTF", "Font_1", 30);
	SOMA::PlayMusic("BGM");

}
void Level2State::Update()
{

	m_pPlayer->Update(2);
	m_hook->Update();
	m_pPlayer->Collision();
	m_hook->Collision(2);
}

void Level2State::Render()
{
	
	SDL_RenderClear(Engine::Instance().GetRenderer());
	SDL_RenderCopy(Engine::Instance().GetRenderer(), TEMA::GetTexture("title"), nullptr, nullptr);
	
	for (int row = 0; row < ROWS2; row++)
	{
		for (int col = 0; col < COLS2; col++)
		{
			Engine::Instance().GetLevel2()[row][col]->Render();
		}
	}

	m_pPlayer->Render();

	//draw the hook
	if (m_hook->GetExist() == true)
		m_hook->Render();
	// If GameState != current state.
	if (dynamic_cast<Level2State*>(STMA::GetStates().back()))
		State::Render();
}



void Level2State::Exit()
{
	delete m_pPlayer;
	delete m_hook;
	for (int row = 0; row < ROWS2; row++)
	{
		for (int col = 0; col < COLS2; col++)
		{
			delete Engine::Instance().GetLevel2()[row][col];
			Engine::Instance().GetLevel2()[row][col] = nullptr; // Wrangle your dangle.
		}
	}

	Engine::Instance().GetTiles().clear();
	
	Engine::Instance().GetPlatform().clear();
}

void Level2State::Resume(){}
//End Level2State
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
	SOMA::Load("Aud/menu_screen_music1.wav", "BGM", SOUND_MUSIC);
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


