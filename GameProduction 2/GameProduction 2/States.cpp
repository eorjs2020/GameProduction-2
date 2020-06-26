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
GameState::GameState() {}

void GameState::Enter()
{

	std::cout << "Entering GameState..." << std::endl;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	m_pPlayer = new Player({ 0,0,19,25 }, { 50.0f,50.0f,32.0f,32.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("playerIdle"), 0, 0, 4, 4);


	ifstream inFile("map/TileDataLevel1.txt");
	if (inFile.is_open())
	{ // Create map of Tile prototypes.
		char key;
		int x, y;
		bool o, h;
		while (!inFile.eof())
		{
			inFile >> key >> x >> y >> o >> h;
			Engine::Instance().GetTiles().emplace(key, new Tile({ x * 127, y * 127, 127, 127 }, { 0,0,16,16 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("tilemap1"), o, h));
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
				Engine::Instance().GetLevel()[row][col]->GetDstP()->x = (float)(16 * col);
				Engine::Instance().GetLevel()[row][col]->GetDstP()->y = (float)(16 * row);
			}
		}
	}
	inFile.close();
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			m_tilePos[row][col].x = Engine::Instance().GetLevel()[row][col]->GetDstP()->x;
			m_tilePos[row][col].y = Engine::Instance().GetLevel()[row][col]->GetDstP()->y;
		}
	}

	SOMA::Load("Aud/power.wav", "beep", SOUND_SFX);
	SOMA::Load("Aud/background_music2.wav", "BGM", SOUND_MUSIC);
	SOMA::Load("Aud/jump.wav", "jump", SOUND_SFX);
	SOMA::Load("Aud/hook_extension3.wav", "throw", SOUND_SFX);
	SOMA::Load("Aud/hook_grappling2.wav", "grab", SOUND_SFX);
	SOMA::Load("Aud/hook_retraction3.wav", "retract", SOUND_SFX);
	FOMA::RegisterFont("Img/LTYPE.TTF", "Font_1", 30);
	SOMA::PlayMusic("BGM");

	
	
}

void GameState::Update()
{
	m_pMusicVolume = m_pMusicSetVol;
	m_pSFXVolume = m_pSFXSetVol;
	SOMA::SetSoundVolume(m_pSFXVolume);
	SOMA::SetMusicVolume(m_pMusicVolume);

	m_pPlayer->Update();

	HandleCamera();
	
}

void GameState::CheckCollision()
{
	
}

void GameState::CheckCollisionHook()
{

}

void GameState::HandleCamera()
{
	
	Engine::Instance().GetCamera().x = (int)m_pPlayer->GetDstP()->x - (WIDTH /2);
	Engine::Instance().GetCamera().y = (int)m_pPlayer->GetDstP()->y - (HEIGHT /2);
	
	Engine::Instance().GetCamera().x = Engine::Instance().GetCamera().x < 0 ? 0 : Engine::Instance().GetCamera().x;
	Engine::Instance().GetCamera().y = Engine::Instance().GetCamera().y < 0 ? 0 : Engine::Instance().GetCamera().y;
	Engine::Instance().GetCamera().x = Engine::Instance().GetCamera().x > Engine::Instance().GetCamera().w ? Engine::Instance().GetCamera().w : Engine::Instance().GetCamera().x;
	Engine::Instance().GetCamera().y = Engine::Instance().GetCamera().y > Engine::Instance().GetCamera().h ? Engine::Instance().GetCamera().h : Engine::Instance().GetCamera().y;
	std::cout <<"x : " <<Engine::Instance().GetCamera().x << endl;
	std::cout << "y : " << Engine::Instance().GetCamera().y << endl;
	std::cout << "player x : " << m_pPlayer->GetDstP()->x << endl;
	std::cout << "player y : " << m_pPlayer->GetDstP()->y << endl;
	/*if (Engine::Instance().GetCamera().x < 0)
	{
		Engine::Instance().GetCamera().x = 0;
	}
	if (Engine::Instance().GetCamera().y < 0)
	{
		Engine::Instance().GetCamera().y = 0;
	}
	if (Engine::Instance().GetCamera().x > (32*COLS) - Engine::Instance().GetCamera().w)
	{
		Engine::Instance().GetCamera().x = (32 * COLS) - Engine::Instance().GetCamera().w;
	}
	if (Engine::Instance().GetCamera().y > (32 * ROWS) - Engine::Instance().GetCamera().h)
	{
		Engine::Instance().GetCamera().y = (32 *ROWS) - Engine::Instance().GetCamera().h;
	}*/
	
	
		//m_pPlayer->GetDstP()->x = 30;
		for (int row = 0; row < ROWS; row++)
		{
			for (int col = 0; col < COLS; col++)
			{

				Engine::Instance().GetLevel()[row][col]->GetDstP()->x = m_tilePos[row][col].x - Engine::Instance().GetCamera().x;
				Engine::Instance().GetLevel()[row][col]->GetDstP()->y = m_tilePos[row][col].y - Engine::Instance().GetCamera().y;
			}
		}
	
}

void GameState::Render()
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

	// If GameState != current state.
	if (dynamic_cast<GameState*>(STMA::GetStates().back()))
		State::Render();
}

void GameState::Exit()
{

	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			delete Engine::Instance().GetLevel()[row][col];
			Engine::Instance().GetLevel()[row][col] = nullptr; // Wrangle your dangle.
		}
	}

	for (auto const& i : Engine::Instance().GetTiles())
		delete Engine::Instance().GetTiles()[i.first];

	Engine::Instance().GetTiles().clear();
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
