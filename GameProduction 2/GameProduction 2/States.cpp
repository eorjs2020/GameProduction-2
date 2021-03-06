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
#include "DebugManager.h"
#include "Utility.h"

// Begin State. CTRL+M+H and CTRL+M+U to turn on/off collapsed code.
void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}
void State::Resume() {}
// End State.

// Begin level 1
Level1State::Level1State() {}

void Level1State::Enter()
{
	std::cout << "Entering Level1State..." << std::endl;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	m_pPlayer = new Player({ 0,0,19,26 }, { 60.0f,90.0f,46.0f,64.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("playerIdle"), 0, 0, 4, 4);
	m_hook = nullptr;
	m_bullNull = false;
	m_defualtTimer = "Timer: 0";
	Engine::Instance().Pause() = false;
	
	//loading data from txt for level layout
	//tile type data 
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
	//tile placement 
	inFile.open("map/LevelOneRedesign.txt");
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
				if (Engine::Instance().GetLevel()[row][col]->IsHazard())
					Engine::Instance().GetHazard().push_back(Engine::Instance().GetLevel()[row][col]);
			}
		}
	}
	inFile.close();
	
	m_goal = new Sprite({ 226,37,12,7 }, { Engine::Instance().GetLevel()[72][166]->GetDstP()->x,Engine::Instance().GetLevel()[72][166]->GetDstP()->y, 32, 32 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("Key"));
	//sound and font
		SOMA::Load("Aud/power.wav", "beep", SOUND_SFX);
	SOMA::Load("Aud/background_music2.wav", "BGM", SOUND_MUSIC);
	SOMA::Load("Aud/jump.wav", "jump", SOUND_SFX);
	SOMA::Load("Aud/hook_extension3.wav", "throw", SOUND_SFX);
	SOMA::Load("Aud/hook_grappling2.wav", "grab", SOUND_SFX);
	SOMA::Load("Aud/hook_retraction3.wav", "retract", SOUND_SFX);
	FOMA::RegisterFont("Img/LTYPE.TTF", "Font_1", 30);
	SOMA::PlayMusic("BGM");
	SOMA::SetMusicVolume(8);
	//UI
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel()[9][17]->GetDstP()->x,Engine::Instance().GetLevel()[9][17]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 }, 
		{ Engine::Instance().GetLevel()[25][80]->GetDstP()->x,Engine::Instance().GetLevel()[25][80]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 }, 
		{ Engine::Instance().GetLevel()[32][52]->GetDstP()->x,Engine::Instance().GetLevel()[32][52]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 300));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel()[37][36]->GetDstP()->x,Engine::Instance().GetLevel()[37][36]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 300));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel()[70][31]->GetDstP()->x,Engine::Instance().GetLevel()[70][31]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 300));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel()[63][94]->GetDstP()->x,Engine::Instance().GetLevel()[63][94]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 300));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel()[37][87]->GetDstP()->x,Engine::Instance().GetLevel()[37][87]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 300));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel()[25][113]->GetDstP()->x,Engine::Instance().GetLevel()[25][113]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 300));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel()[50][110]->GetDstP()->x,Engine::Instance().GetLevel()[50][110]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 300));
	fDrone.push_back(new FireDrone({ 0,0,11,19 }, 
		{ Engine::Instance().GetLevel()[20][66]->GetDstP()->x,Engine::Instance().GetLevel()[20][66]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));
	fDrone.push_back(new FireDrone({ 0,0,11,19 },
		{ Engine::Instance().GetLevel()[63][41]->GetDstP()->x,Engine::Instance().GetLevel()[63][41]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));
	fDrone.push_back(new FireDrone({ 0,0,11,19 },
		{ Engine::Instance().GetLevel()[45][88]->GetDstP()->x,Engine::Instance().GetLevel()[45][88]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));
	fDrone.push_back(new FireDrone({ 0,0,11,19 },
		{ Engine::Instance().GetLevel()[16][148]->GetDstP()->x,Engine::Instance().GetLevel()[16][148]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));
	fDrone.push_back(new FireDrone({ 0,0,11,19 },
		{ Engine::Instance().GetLevel()[50][75]->GetDstP()->x,Engine::Instance().GetLevel()[50][75]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));
	fDrone.push_back(new FireDrone({ 0,0,11,19 },
		{ Engine::Instance().GetLevel()[65][130]->GetDstP()->x,Engine::Instance().GetLevel()[65][130]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));



	m_pause = new PauseButton({ 0,0,490,140 }, { 2.0f,2.0f,240.0f,70.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("pause"));
	m_quit = new QuitButton({ 0,0,490,140 }, { 380.0f,280.0f,240.0f,70.0f },
	    Engine::Instance().GetRenderer(), TEMA::GetTexture("quit"));
	m_mainMenu = new MainMenuButton({ 0,0,490,140 }, { 380.0f,210.0f,240.0f,70.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("mainmenu"));
	m_resume = new ResumeButton({ 0,0,490,140 }, { 380.0f,420.0f,240.0f,70.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("resume"));
	m_timer = new Label("font1", 850, 10, m_defualtTimer, {255,255,255,255});
	m_energy = new Label("font1", 410, 680, m_defualtEnergy, { 255,255,255,255 });
	//start timer
	timer.start();
	//set variables  
	m_pNumBulletHit = 0; 
	//load battery strite pos and data
	for (int i = 0; i < 38; i++) {
		m_battery[i] = new Sprite({ 0,0,32,32 }, { Engine::Instance().GetLevel()[m_batteryX[i]][m_batteryY[i]]->GetDstP()->x,Engine::Instance().GetLevel()[m_batteryX[i]][m_batteryY[i]]->GetDstP()->y, 32, 32 },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("battery"));
	}
}

void Level1State::Update()
{
	//Check to see if goal was hit
	if(COMA::AABBCheck(*m_pPlayer->GetDstP(),*m_goal->GetDstP()))
		m_stageEnd = true;
	//updates when paused 
	if (Engine::Instance().Pause() == true)
	{
		if (m_mainMenu->Update() == 1)
			return;
		if (m_quit->Update() == 1)
			return;
		m_resume->Update();
	}
	// 
	if (EVMA::KeyHeld(SDL_SCANCODE_X))
	{
		Engine::Instance().Pause() = true;
	}
	//updates when not paused
	if (Engine::Instance().Pause() == false)
	{

		
		
		m_pMusicVolume = m_pMusicSetVol;
		m_pSFXVolume = m_pSFXSetVol;
		SOMA::SetSoundVolume(m_pSFXVolume);
		SOMA::SetMusicVolume(m_pMusicVolume);
		m_pause->Update();
		m_pPlayer->Update();
		m_goal->GetDstP()->x = Engine::Instance().GetLevel()[71][165]->GetDstP()->x;
		m_goal->GetDstP()->y = Engine::Instance().GetLevel()[71][165]->GetDstP()->y;
		for (unsigned i = 0; i < Engine::Instance().GetEnemy().size(); ++i)
		{
			Engine::Instance().GetEnemy()[i]->Update(m_pPlayer->GetVelX(),
				m_pPlayer->GetVelY(), m_pPlayer->BGScorllX(), m_pPlayer->BGScrollY(), m_pPlayer);
		}
		
		for (auto i = 0; i < fDrone.size(); ++i)
		{
			fDrone[i]->Update(m_pPlayer->GetVelX(),
				m_pPlayer->GetVelY(), m_pPlayer->BGScorllX(), m_pPlayer->BGScrollY(), m_pPlayer, LOS(i));
		}
		m_pPlayer->Collision();
		
		m_updateTimer = m_defualtTimer + timer.getrunnningtime(timer);
		m_timer->SetText(m_updateTimer);
		m_energyNum = std::to_string(m_pPlayer->getEnergy());
		m_updateEnergy = m_defualtEnergy + m_energyNum;
		
		
		m_energy->SetText(m_updateEnergy);
		BulletCollision();
		//bullet update
		for (auto i = 0; i < m_vEBullets.size(); ++i)
		{
			m_vEBullets[i]->Update();
		}
		//hook creation 
		if (EVMA::MousePressed(1)) {
			m_destinationX = EVMA::GetMousePos().x;
			m_destinationY = EVMA::GetMousePos().y;
			m_hook = new GrapplingHook({ 0,0,50,20 }, { m_pPlayer->GetDstP()->x, m_pPlayer->GetDstP()->y, 25, 10 },
				Engine::Instance().GetRenderer(), TEMA::GetTexture("hook"), 0.00, m_pPlayer, m_destinationX, m_destinationY);
		}
		//hook update and collision 
		if (m_hook != nullptr) {
			m_hook->Update();
			m_hook->Collision();
			if (m_hook->GetExist() == false) {
				m_hook = nullptr;
			}
		}
	}
	//Battery postion, collision and energy set
	for (int i = 0; i < 38; i++) {
		if(m_battery[i] != nullptr){
			m_battery[i]->GetDstP()->x = Engine::Instance().GetLevel()[m_batteryX[i]][m_batteryY[i]]->GetDstP()->x;
		m_battery[i]->GetDstP()->y = Engine::Instance().GetLevel()[m_batteryX[i]][m_batteryY[i]]->GetDstP()->y;
		if (COMA::AABBCheck(*m_pPlayer->GetDstP(), *m_battery[i]->GetDstP())) {
				m_pPlayer->setEnergy(10);
				m_battery[i] = nullptr;
			}
		}
	}
	//Bullet slow and reset to normal speed
	if (bulletslow)
	{
		m_pPlayer->SetMaxVel(2);
		++bulletTimer;
		if (bulletTimer >= 50)
		{
			m_pPlayer->SetMaxVel(5);
			bulletslow = false;
			bulletTimer = 0;
		}
	}
	
	//Score calculation and State change
	if (m_stageEnd){
		Engine::Instance().setScore((m_pPlayer->getEnergy() / 5) * 500);
		int a;
		if (timer.getmin() == 3)
		{
			a = 2000;
		}
		else if(timer.getmin() == 2 && timer.getsec() <= 59 && timer.getsec() >= 30)
		{
			a = 3000;
		}
		else if (timer.getmin() == 2 && timer.getsec() < 30)
		{
			a = 5000;
		}
		else if (timer.getmin() == 1 && timer.getsec() <= 59 && timer.getsec() >= 30)
		{
			a = 8000;
		}
		else if (timer.getmin() == 1 && timer.getsec() < 30)
		{
			a = 10000;
		}
		else if (timer.getmin() == 0 && timer.getsec() <= 59 && timer.getsec() >= 30)
		{
			a = 50000;
		}
		else if (timer.getmin() == 0 && timer.getsec() < 30)
		{
			a = 100000;
		}
		else {
			a = 0;
		}
		Engine::Instance().setScore(-(m_pNumBulletHit * 10));
		Engine::Instance().setScore(a);
		Engine::Instance().setScoreState(0);
		STMA::ChangeState(new ScoreState);
	}
}

//Empty ??? 
void Level1State::CheckCollisionHook()
{

}

void Level1State::Render()
{
	
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 64, 128, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	//Render Background
	SDL_RenderCopy(Engine::Instance().GetRenderer(), TEMA::GetTexture("title"), nullptr, nullptr);
	//Render tiles 
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			Engine::Instance().GetLevel()[row][col]->Render();
		}
	}
	//Render Bullets
	for (auto i = 0; i < m_vEBullets.size(); ++i)
	{
		m_vEBullets[i]->Render();
	}
	//Render Drone Enemies
	for (auto i = 0; i < fDrone.size(); ++i)
	{
		fDrone[i]->Render();
	}
	//Render Patrol Enemies  
	for (unsigned i = 0; i < Engine::Instance().GetEnemy().size();++i)
	{
		Engine::Instance().GetEnemy()[i]->Render();
	}
	
	
	m_goal->Render();
	
	
	//Render Battery 
	for (int i = 0; i < 38; i++) {
		if (m_battery[i] != nullptr)
			m_battery[i]->Render();
	}
	//Render hook
	if (m_hook != nullptr)
	{
		m_hook->Render();
	}
	m_pPlayer->Render();
	m_energy->Render();
	//Render timer
	m_timer->Render();
	// If GameState != current state.
	if (dynamic_cast<Level1State*>(STMA::GetStates().back()))
		State::Render();
	//Render Pause button
	if (Engine::Instance().Pause() == false)
	{
		m_pause->Render();
	}
	//Render Pause state buttons
	if (Engine::Instance().Pause() == true)
	{
		m_quit->Render();
		m_mainMenu->Render();
		m_resume->Render();
	}
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
	Engine::Instance().GetEnemy().clear();
	Engine::Instance().GetPlatform().clear();
	Engine::Instance().GetHazard().clear();
	fDrone.clear();
	m_vEBullets.clear();
	std::cout << "Cleaning Level1" << endl;
	
	
}

void Level1State::Resume() { }

void Level1State::BulletCollision()
{
	for (int i = 0; i < (int)m_vEBullets.size(); i++)
	{
		m_vEBullets[i]->Update();
		SDL_Rect b = { m_vEBullets[i]->GetDstP()->x, m_vEBullets[i]->GetDstP()->y,
			m_vEBullets[i]->GetDstP()->w, m_vEBullets[i]->GetDstP()->h };


		SDL_Rect e = { m_pPlayer->GetDstP()->x, m_pPlayer->GetDstP()->y, m_pPlayer->GetDstP()->w, m_pPlayer->GetDstP()->h };
		if (SDL_HasIntersection(&b, &e))
		{

			delete m_vEBullets[i];
			m_vEBullets[i] = nullptr;
			m_bullNull = true;
			if (!m_pPlayer->getBar())
			{
				++m_pNumBulletHit;
				bulletslow = true;
			}
			break;
		}

	}
	if (m_bullNull) CleanVector<Bullet*>(m_vEBullets, m_bullNull);
	for (int i = 0; i < (int)m_vEBullets.size(); i++)
	{
		SDL_Rect b = { m_vEBullets[i]->GetDstP()->x, m_vEBullets[i]->GetDstP()->y,
			m_vEBullets[i]->GetDstP()->w, m_vEBullets[i]->GetDstP()->h };
		for (int j = 0; j < Engine::Instance().GetPlatform().size(); j++)
		{
			if (Engine::Instance().GetPlatform()[j] == nullptr) continue;
			SDL_Rect e = { Engine::Instance().GetPlatform()[j]->GetDstP()->x, Engine::Instance().GetPlatform()[j]->GetDstP()->y, 32, 32 };
			if (SDL_HasIntersection(&b, &e))
			{
				delete m_vEBullets[i];
				m_vEBullets[i] = nullptr;
				m_bullNull = true;
				break;
			}
		}
	}
	if (m_bullNull) CleanVector<Bullet*>(m_vEBullets, m_bullNull);
}

bool Level1State::LOS(int n)
{

	int temp = 0;
	for (int i = 0; i < Engine::Instance().GetPlatform().size(); i++)
	{
		
		if (!COMA::LOSCheck(m_pPlayer, fDrone[n]->GetDstP(), Engine::Instance().GetPlatform()[i]))
			return false;
	
		
	}
	

	return true;
}

// End level 1

// Begin Level 2
Level2State::Level2State() {}

void Level2State::Enter()
{

	std::cout << "Entering Level2State..." << std::endl;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	
	m_hook = nullptr;
	m_bullNull = false;
	Engine::Instance().Pause() = false;
	//loading data from txt for level layout
	//tile type data
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
	//level layout
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
	m_pPlayer = new Player({ 0,0,19,26 }, { Engine::Instance().GetLevel2()[46][6]->GetDstP()->x,Engine::Instance().GetLevel2()[46][6]->GetDstP()->y,46.0f,64.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("playerIdle"), 0, 0, 4, 4);
	//   enemy spawn and mvmt boundaries  
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 }, 
		{ Engine::Instance().GetLevel2()[47][48]->GetDstP()->x,Engine::Instance().GetLevel2()[47][48]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[47][70]->GetDstP()->x,Engine::Instance().GetLevel2()[47][70]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[47][95]->GetDstP()->x,Engine::Instance().GetLevel2()[47][95]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[19][165]->GetDstP()->x,Engine::Instance().GetLevel2()[19][165]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[22][201]->GetDstP()->x,Engine::Instance().GetLevel2()[22][201]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[22][228]->GetDstP()->x,Engine::Instance().GetLevel2()[22][228]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[18][260]->GetDstP()->x,Engine::Instance().GetLevel2()[18][260]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[27][373]->GetDstP()->x,Engine::Instance().GetLevel2()[27][373]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[27][400]->GetDstP()->x,Engine::Instance().GetLevel2()[27][400]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	//fire Drone
	fDrone.push_back(new FireDrone({ 0,0,11,19 }, 
		{ Engine::Instance().GetLevel2()[35][83]->GetDstP()->x,Engine::Instance().GetLevel2()[35][83]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));
	fDrone.push_back(new FireDrone({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[13][100]->GetDstP()->x,Engine::Instance().GetLevel2()[13][100]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));
	fDrone.push_back(new FireDrone({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[13][137]->GetDstP()->x,Engine::Instance().GetLevel2()[13][137]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));
	fDrone.push_back(new FireDrone({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[13][218]->GetDstP()->x,Engine::Instance().GetLevel2()[13][218]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));
	fDrone.push_back(new FireDrone({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[17][333]->GetDstP()->x,Engine::Instance().GetLevel2()[17][333]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));
	

	m_Pause = new PauseButton({ 0,0,490,140 }, { 0.0f,0.0f,240.0f,70.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("pause"));
	m_quit = new QuitButton({ 0,0,490,140 }, { 380.0f,280.0f,240.0f,70.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("quit"));
	m_mainMenu = new MainMenuButton({ 0,0,490,140 }, { 380.0f,210.0f,240.0f,70.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("mainmenu"));
	m_resume = new ResumeButton({ 0,0,490,140 }, { 380.0f,420.0f,240.0f,70.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("resume"));
	m_goal = new Sprite({ 226,37,12,7 }, { Engine::Instance().GetLevel2()[3][14]->GetDstP()->x,Engine::Instance().GetLevel2()[3][14]->GetDstP()->y, 32, 32 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("Key"));
	for (int i = 0; i < 28; i++) {
		m_battery[i] = new Sprite({ 0,0,32,32 }, { Engine::Instance().GetLevel2()[m_batteryX[i]][m_batteryY[i]]->GetDstP()->x,Engine::Instance().GetLevel2()[m_batteryX[i]][m_batteryY[i]]->GetDstP()->y, 32, 32 },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("battery"));
	}
	m_timer = new Label("font1", 850, 10, m_defualtTimer, { 255,255,255,255 });
	m_energy = new Label("font1", 410, 680, m_defualtEnergy, { 255,255,255,255 });
	//start timer
	timer.start();
	//set variables  
	m_pNumBulletHit = 0;
	SOMA::Load("Aud/power.wav", "beep", SOUND_SFX);
	SOMA::Load("Aud/background_music2.wav", "BGM", SOUND_MUSIC);
	SOMA::Load("Aud/jump.wav", "jump", SOUND_SFX);
	SOMA::Load("Aud/hook_extension3.wav", "throw", SOUND_SFX);
	SOMA::Load("Aud/hook_grappling2.wav", "grab", SOUND_SFX);
	SOMA::Load("Aud/hook_retraction3.wav", "retract", SOUND_SFX);
	FOMA::RegisterFont("Img/LTYPE.TTF", "Font_1", 30);
	SOMA::PlayMusic("BGM");
	SOMA::SetMusicVolume(8);

}

void Level2State::Update()
{
	

	if (COMA::AABBCheck(*m_pPlayer->GetDstP(), *m_goal->GetDstP()))
		m_stageEnd = true;
	if (Engine::Instance().Pause() == true)
	{
		if (m_mainMenu->Update() == 1)
			return;
		if (m_quit->Update() == 1)
			return;
		m_resume->Update();
	}
	else
		if (m_Pause->Update() == 1)
			return;
	if (Engine::Instance().Pause() == false)
	{
		for (unsigned i = 0; i < Engine::Instance().GetEnemy().size(); ++i)
		{
			Engine::Instance().GetEnemy()[i]->Update(m_pPlayer->GetVelX(),
				m_pPlayer->GetVelY(), m_pPlayer->BGScorllX(), m_pPlayer->BGScrollY(), m_pPlayer);
		}
		m_pPlayer->Update(2);
		m_pPlayer->Collision();
		for (auto i = 0; i < fDrone.size(); ++i)
		{
			fDrone[i]->Update(m_pPlayer->GetVelX(),
				m_pPlayer->GetVelY(), m_pPlayer->BGScorllX(), m_pPlayer->BGScrollY(), m_pPlayer, LOS(i));
		}
		for (auto i = 0; i < m_vEBullets.size(); ++i)
		{
			m_vEBullets[i]->Update();
		}
		BulletCollision();
		m_goal->GetDstP()->x = Engine::Instance().GetLevel2()[3][410]->GetDstP()->x;
		m_goal->GetDstP()->y = Engine::Instance().GetLevel2()[3][410]->GetDstP()->y;
		m_updateTimer = m_defualtTimer + timer.getrunnningtime(timer);
		m_timer->SetText(m_updateTimer);
		m_energyNum = std::to_string(m_pPlayer->getEnergy());
		m_updateEnergy = m_defualtEnergy + m_energyNum;
		m_energy->SetText(m_updateEnergy);

		for (int i = 0; i < 28; i++) {
			if (m_battery[i] != nullptr) {
				m_battery[i]->GetDstP()->x = Engine::Instance().GetLevel2()[m_batteryX[i]][m_batteryY[i]]->GetDstP()->x;
				m_battery[i]->GetDstP()->y = Engine::Instance().GetLevel2()[m_batteryX[i]][m_batteryY[i]]->GetDstP()->y;
				if (COMA::AABBCheck(*m_pPlayer->GetDstP(), *m_battery[i]->GetDstP())) {
					m_pPlayer->setEnergy(10);
					m_battery[i] = nullptr;
				}
			}
		}
		//hook creation 
		if (EVMA::MousePressed(1)) {
			m_destinationX = EVMA::GetMousePos().x;
			m_destinationY = EVMA::GetMousePos().y;
			m_hook = new GrapplingHook({ 0,0,50,20 }, { m_pPlayer->GetDstP()->x, m_pPlayer->GetDstP()->y, 25, 10 },
				Engine::Instance().GetRenderer(), TEMA::GetTexture("hook"), 0.00, m_pPlayer, m_destinationX, m_destinationY);
		}
		//hook update and collision 
		if (m_hook != nullptr) {
			m_hook->Update();
			m_hook->Collision(2);
			if (m_hook->GetExist() == false) {
				m_hook = nullptr;
			}
		}
	}

	//Bullet slow and reset to normal speed
	if (bulletslow)
	{
		m_pPlayer->SetMaxVel(2);
		++bulletTimer;
		if (bulletTimer >= 50)
		{
			m_pPlayer->SetMaxVel(5);
			bulletslow = false;
			bulletTimer = 0;
		}
	}
	//Score calculation and State change
	if (m_stageEnd) {
		Engine::Instance().setScore((m_pPlayer->getEnergy() / 5) * 500);
		int a;
		if (timer.getmin() == 5)
		{
			a = 2000;
		}
		else if (timer.getmin() == 4 && timer.getsec() <= 59 && timer.getsec() >= 30)
		{
			a = 3000;
		}
		else if (timer.getmin() == 4 && timer.getsec() < 30)
		{
			a = 5000;
		}
		else if (timer.getmin() == 3 && timer.getsec() <= 59 && timer.getsec() >= 30)
		{
			a = 8000;
		}
		else if (timer.getmin() == 3 && timer.getsec() < 30)
		{
			a = 10000;
		}
		else if (timer.getmin() == 2 && timer.getsec() <= 59 && timer.getsec() >= 30)
		{
			a = 20000;
		}
		else if (timer.getmin() == 2 && timer.getsec() < 30)
		{
			a = 50000;
		}
		else if (timer.getmin() == 1 && timer.getsec() <= 59 && timer.getsec() >= 30)
		{
			a = 80000;
		}
		else if (timer.getmin() == 1 && timer.getsec() < 30)
		{
			a = 100000;
		}
		else if (timer.getmin() == 0 && timer.getsec() <= 59 && timer.getsec() >= 30)
		{
			a = 500000;
		}
		else if (timer.getmin() == 0 && timer.getsec() < 30)
		{
			a = 1000000;
		}
		else {
			a = 0;
		}
		Engine::Instance().setScore(-(m_pNumBulletHit * 10));
		Engine::Instance().setScore(a);
		Engine::Instance().setScoreState(2);
		STMA::ChangeState(new ScoreState);
	}
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
	for (unsigned i = 0; i < Engine::Instance().GetEnemy().size(); ++i)
	{
		Engine::Instance().GetEnemy()[i]->Render();
	}
	
	
	for (auto i = 0; i < m_vEBullets.size(); ++i)
	{
		m_vEBullets[i]->Render();
	}

	for (auto i = 0; i < fDrone.size(); ++i)
	{
		fDrone[i]->Render();
	}
	//draw the hook
	if (m_hook != nullptr)
	{
		m_hook->Render();
	}
	m_timer->Render();
	m_energy->Render();
	m_goal->Render();
	for (int i = 0; i < 28; i++) {
		if (m_battery[i] != nullptr)
			m_battery[i]->Render();
	}
	m_pPlayer->Render();
	// If GameState != current state.
	if (dynamic_cast<Level2State*>(STMA::GetStates().back()))
		State::Render();
	
	if (Engine::Instance().Pause())
	{
		m_quit->Render();
		m_mainMenu->Render();
		m_resume->Render();
	}
	else
		m_Pause->Render();
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
	Engine::Instance().GetEnemy().clear();
	fDrone.clear();
	m_vEBullets.clear();
	Engine::Instance().GetPlatform().clear();
	Engine::Instance().GetHazard().clear();
}

void Level2State::Resume(){}

void Level2State::BulletCollision()
{
	for (int i = 0; i < (int)m_vEBullets.size(); i++)
	{
		m_vEBullets[i]->Update();
		SDL_Rect b = { m_vEBullets[i]->GetDstP()->x, m_vEBullets[i]->GetDstP()->y,
			m_vEBullets[i]->GetDstP()->w, m_vEBullets[i]->GetDstP()->h };


		SDL_Rect e = { m_pPlayer->GetDstP()->x, m_pPlayer->GetDstP()->y, 32, 32 };
		if (SDL_HasIntersection(&b, &e))
		{

			delete m_vEBullets[i];
			m_vEBullets[i] = nullptr;
			m_bullNull = true;
			bulletslow = true;
			break;
		}

	}
	if (m_bullNull) CleanVector<Bullet*>(m_vEBullets, m_bullNull);
	for (int i = 0; i < (int)m_vEBullets.size(); i++)
	{
		SDL_Rect b = { m_vEBullets[i]->GetDstP()->x, m_vEBullets[i]->GetDstP()->y,
			m_vEBullets[i]->GetDstP()->w, m_vEBullets[i]->GetDstP()->h };
		for (int j = 0; j < Engine::Instance().GetPlatform().size(); j++)
		{
			if (Engine::Instance().GetPlatform()[j] == nullptr) continue;
			SDL_Rect e = { Engine::Instance().GetPlatform()[j]->GetDstP()->x, Engine::Instance().GetPlatform()[j]->GetDstP()->y, 32, 32 };
			if (SDL_HasIntersection(&b, &e))
			{
				delete m_vEBullets[i];
				m_vEBullets[i] = nullptr;
				m_bullNull = true;
				break;
			}
		}
	}
	if (m_bullNull) CleanVector<Bullet*>(m_vEBullets, m_bullNull);
}
bool Level2State::LOS(int n)
{

	int temp = 0;
	for (int i = 0; i < Engine::Instance().GetPlatform().size(); i++)
	{

		if (!COMA::LOSCheck(m_pPlayer, fDrone[n]->GetDstP(), Engine::Instance().GetPlatform()[i]))
			return false;


	}


	return true;
}

//End Level2State

//Start of Tutorial State
TutorialState::TutorialState() {}

void TutorialState::Enter()
{
	std::cout << "Entering Level1State..." << std::endl;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	m_pPlayer = new Player({ 0,0,19,26 }, { 150.0f,500.0f,46.0f,64.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("playerIdle"), 0, 0, 4, 4);
	m_hook = nullptr;
	Engine::Instance().Pause() = false;
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 }, { 300,300,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));

	m_timer = new Label("font1", 850, 10, m_defualtTimer, { 255,255,255,255 });
	m_energy = new Label("font1", 410, 680, m_defualtEnergy, { 255,255,255,255 });
	m_input = new Label("font1", WIDTH / 2 - 250, 120, "Press enter", { 255,255,255,255 });
	tuto1 = new Label("font1", WIDTH / 2 -250, 150 , "Player can be controlled with WASD", { 255,255,255,255 });
	tuto2 = new Label("font1", WIDTH / 2 - 250, 150,  "Grappling Hook can be controlled with", { 255, 255, 255, 255 });
	tuto2b = new Label("font1", WIDTH / 2 - 250, 180, "right mouse click to aim and held to move player", { 255,255,255,255 });
	tuto3 = new Label("font1", WIDTH / 2 - 250, 150, "Spacebar - Jump", { 255,255,255,255 });
	tuto4 = new Label("font1", WIDTH / 2 - 250, 150, "Collect green batteries to gain energy", { 255,255,255,255 });
	tuto4b = new Label("font1", WIDTH / 2 - 250, 180, "to use skills and gain a better score", { 255,255,255,255 });
	tuto5 = new Label("font1", WIDTH / 2 - 250, 150, "There are four skills", { 255,255,255,255 });
	tuto5b = new Label("font1", WIDTH / 2 - 250, 180, "speed boost, invisibility, double jump and barrier", { 255,255,255,255 });
	tuto5c = new Label("font1", WIDTH / 2 - 250, 210, "which can be activated with buttons 1,2,3 and 4 ", { 255,255,255,255 });
	tuto6 = new Label("font1", WIDTH / 2 - 250, 150, "Find the key in the stage as", { 255,255,255,255 });
	tuto6b = new Label("font1", WIDTH / 2 - 250, 180, "fast as possible to complete the stage", { 255,255,255,255 });
	tuto7 = new Label("font1", WIDTH / 2 - 250 , 150, "Game can be paused with", { 255,255,255,255 });
	tuto7b = new Label("font1", WIDTH / 2 - 250, 180, "the button on the top left", { 255,255,255,255 });
	tuto8 = new Label("font1", WIDTH / 2 - 250, 150, "Be careful of enemies and hazards", { 255,255,255,255 });
	tuto8b = new Label("font1", WIDTH / 2 - 250, 180, "becasue they will slow you down", { 255,255,255,255 });
	explainKey = new Sprite({ 226,37,12,7 }, { WIDTH/2 -16, HEIGHT/2 - 16, 32, 32 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("Key"));
	//loading data from txt for level layout
	//tile type data 
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
	//loading level layout
	inFile.open("map/tutorial.txt");
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
	m_Pause = new PauseButton({ 0,0,490,140 }, { 0.0f,0.0f,240.0f,70.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("pause"));
	m_quit = new QuitButton({ 0,0,490,140 }, { 380.0f,280.0f,240.0f,70.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("quit"));
	m_mainMenu = new MainMenuButton({ 0,0,490,140 }, { 380.0f,210.0f,240.0f,70.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("mainmenu"));
	m_resume = new ResumeButton({ 0,0,490,140 }, { 380.0f,420.0f,240.0f,70.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("resume"));
	m_goal = new Sprite({ 226,37,12,7 }, { Engine::Instance().GetLevel()[20][81]->GetDstP()->x,Engine::Instance().GetLevel()[20][81]->GetDstP()->y, 32, 32 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("Key"));
	for (int i = 0; i < 4; i++) {
		m_battery[i] = new Sprite({ 0,0,32,32 }, { Engine::Instance().GetLevel()[m_batteryX[i]][m_batteryY[i]]->GetDstP()->x,Engine::Instance().GetLevel()[m_batteryX[i]][m_batteryY[i]]->GetDstP()->y, 32, 32 },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("battery"));
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

void TutorialState::Update()
{

	if (m_Pause->Update() == 1)
		return;
			
	if (COMA::AABBCheck(*m_pPlayer->GetDstP(), *m_goal->GetDstP()))
		m_stageEnd = true;
	if (Engine::Instance().Pause() == true)
	{
		if (m_mainMenu->Update() == 1)
			return;
		if (m_quit->Update() == 1)
			return;
		m_resume->Update();
	}

	if (EVMA::KeyReleased(SDL_SCANCODE_RETURN) && explainPause)
	{
		++pressEnter;
		if (pressEnter == 9) {
			explainPause = false;
			//start timer
			timer.start();
		}
	}
	//updates when not paused
	if (Engine::Instance().Pause() == false)
	{
		if (!explainPause)
		{
			m_pMusicVolume = m_pMusicSetVol;
			m_pSFXVolume = m_pSFXSetVol;
			SOMA::SetSoundVolume(m_pSFXVolume);
			SOMA::SetMusicVolume(m_pMusicVolume);
			m_pPlayer->Update();
			m_goal->GetDstP()->x = Engine::Instance().GetLevel()[20][81]->GetDstP()->x;
			m_goal->GetDstP()->y = Engine::Instance().GetLevel()[20][81]->GetDstP()->y;
			for (unsigned i = 0; i < Engine::Instance().GetEnemy().size(); ++i)
			{
				Engine::Instance().GetEnemy()[i]->Update(m_pPlayer->GetVelX(),
					m_pPlayer->GetVelY(), m_pPlayer->BGScorllX(), m_pPlayer->BGScrollY(), m_pPlayer);
			}
			m_pPlayer->Collision();
			m_updateTimer = m_defualtTimer + timer.getrunnningtime(timer);
			m_timer->SetText(m_updateTimer);
			m_energyNum = std::to_string(m_pPlayer->getEnergy());
			m_updateEnergy = m_defualtEnergy + m_energyNum;
			m_energy->SetText(m_updateEnergy);
	
			for (int i = 0; i < 4; i++) {
				if (m_battery[i] != nullptr) {
					m_battery[i]->GetDstP()->x = Engine::Instance().GetLevel()[m_batteryX[i]][m_batteryY[i]]->GetDstP()->x;
					m_battery[i]->GetDstP()->y = Engine::Instance().GetLevel()[m_batteryX[i]][m_batteryY[i]]->GetDstP()->y;
					if (COMA::AABBCheck(*m_pPlayer->GetDstP(), *m_battery[i]->GetDstP())) {
						m_pPlayer->setEnergy(10);
						m_battery[i] = nullptr;
					}
				}
			}
			//hook creation 
			if (EVMA::MousePressed(1)) {
				m_destinationX = EVMA::GetMousePos().x;
				m_destinationY = EVMA::GetMousePos().y;
				m_hook = new GrapplingHook({ 0,0,50,20 }, { m_pPlayer->GetDstP()->x, m_pPlayer->GetDstP()->y, 25, 10 },
					Engine::Instance().GetRenderer(), TEMA::GetTexture("hook"), 0.00, m_pPlayer, m_destinationX, m_destinationY);
			}
			//hook update and collision 
			if (m_hook != nullptr) {
				m_hook->Update();
				m_hook->Collision();
				if (m_hook->GetExist() == false) {
					m_hook = nullptr;
				}
			}
		}
	}
	if (m_stageEnd)
		STMA::ChangeState(new TitleState);
}

//Empty ?? 
void TutorialState::CheckCollisionHook()
{
}

void TutorialState::Render()
{
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			Engine::Instance().GetLevel()[row][col]->Render();
		}
	}
	m_pPlayer->Render();
	m_timer->Render();
	m_energy->Render();
	m_goal->Render();
	for (int i = 0; i < 4; i++) {
		if (m_battery[i] != nullptr)
			m_battery[i]->Render();
	}
	if (explainPause)
		m_input->Render();
	if (pressEnter == 1)
		tuto1->Render();
	if (pressEnter == 2)
	{
		tuto2->Render();
		tuto2b->Render();
	}
	if (pressEnter == 3)
		tuto3->Render();
	if (pressEnter == 4)
	{
		tuto4->Render();
		tuto4b->Render();
	}
	if (pressEnter == 5) {
		tuto5->Render();
		tuto5b->Render();
		tuto5c->Render();
	}
	if (pressEnter == 6)
	{
		explainKey->Render();
		tuto6->Render();
		tuto6b->Render();
	}
	if (pressEnter == 7) {
		tuto7->Render();
		tuto7b->Render();
	}
	if (pressEnter == 8) {
		tuto8->Render();
		tuto8b->Render();
	}
	for (unsigned i = 0; i < Engine::Instance().GetEnemy().size(); ++i)
	{
		Engine::Instance().GetEnemy()[i]->Render();
	}
	//Render hook
	if (m_hook != nullptr)
	{
		m_hook->Render();
	}
	m_Pause->Render();
	// If GameState != current state.
	if (dynamic_cast<Level1State*>(STMA::GetStates().back()))
		State::Render();
	if (Engine::Instance().Pause() == true)
	{
		m_quit->Render();
		m_mainMenu->Render();
		m_resume->Render();
	}
}

void TutorialState::Exit()
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
	Engine::Instance().GetEnemy().clear();
	Engine::Instance().GetPlatform().clear();


	std::cout << "Cleaning Level1" << endl;

}

void TutorialState::Resume()
{

}
//End of Tutorial State


// Begin TitleState.
TitleState::TitleState() {}

void TitleState::Enter()
{
	
	m_playBtn = new PlayButton({ 0,0,480,140 }, { 380.0f,350.0f,240.0f,70.0f }, 
		Engine::Instance().GetRenderer(), TEMA::GetTexture("play"));
	m_quitBtn = new QuitButton({ 0,0,490,140 }, { 380.0f,490.0f,240.0f,70.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("quit"));
	m_tuto = new TutorialButton({ 0,0,490,140 }, { 380.0f,420.0f,240.0f,70.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("tutorial"));
	SOMA::Load("Aud/power.wav", "beep", SOUND_SFX);
	SOMA::Load("Aud/menu_screen_music1.wav", "BGM", SOUND_MUSIC);
	SOMA::SetMusicVolume(16);
	SOMA::SetSoundVolume(20);
	SOMA::PlayMusic("BGM");
}

void TitleState::Update()
{
	if (m_tuto->Update() == 1)
		return;

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
	m_tuto->Render();
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
	m_playBtn->Render();
	State::Render();
}

void EndState::Exit()
{
	std::cout << "Exiting EndState..." << std::endl;
}
// End TitleState.

ScoreState::ScoreState() {}

void ScoreState::Update()
{
	
	++updatingtimer;
	if ((updatingtimer % 2) == 0)
	{
		if (timeScore > totalScore)
		{
			totalScore += 100;
			tS = std::to_string(totalScore);
			m_score->SetText(tS);
		}
	}
	
	if (m_nextLevelBtn->Update())
		return;

}

void ScoreState::Render()
{
	SDL_RenderCopy(Engine::Instance().GetRenderer(), TEMA::GetTexture("title"), nullptr, nullptr);
	m_nextLevelBtn->Render();
	m_score->Render();
}

void ScoreState::Enter()
{
	m_nextLevelBtn = new LevelTwoButton({ 0,0,490,140 }, { 380.0f,280.0f,240.0f,70.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("resume"));
	timeScore = Engine::Instance().getScore();
	updatingtimer = 0;
	totalScore = 0;
	m_score = new Label("font1", WIDTH / 2 - 100, HEIGHT / 2, tS, { 255,255,255,255 });;
}

void ScoreState::Exit()
{
}
