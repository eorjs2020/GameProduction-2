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

// Begin GameState.
Level1State::Level1State() {}

void Level1State::Enter()
{

	std::cout << "Entering Level1State..." << std::endl;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	
	m_pPlayer = new Player({ 0,0,19,26 }, { 60.0f,200.0f,46.0f,64.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("playerIdle"), 0, 0, 4, 4);
	m_hook = nullptr;

	///////////////////////////////////////////
	///   enemy spawn and mvmt boundaries   ///
	///////////////////////////////////////////
	fDrone.push_back( new FireDrone({ 0,0,11,19 }, { 100.0f,200.0f,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));

	m_bullNull = false;

	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 }, { 300,300,22,38 }, 
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 }, { 1000,300,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 }, { 500,1000,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 300));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 }, { 900,1000,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 400));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 }, { 200,2500,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 400));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 }, { 200,2000,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 250));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 }, { 950,2200,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));






	m_interface = new Sprite({ 6,455,224,44 }, { 400.0f,724.0f,224.0f,44.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("interface"));
	m_pause = new PauseButton({ 0,0,490,140 }, { 2.0f,2.0f,240.0f,70.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("pause"));
	m_quit = new QuitButton({ 0,0,490,140 }, { 380.0f,280.0f,240.0f,70.0f },
	    Engine::Instance().GetRenderer(), TEMA::GetTexture("quit"));
	m_timer = new Label("font1", 850, 10, m_defualtTimer, {255,255,255,255});
	m_energy = new Label("font1", 410, 680, m_defualtEnergy, { 255,255,255,255 });
	m_mainMenu = new MainMenuButton({ 0,0,490,140 }, { 380.0f,210.0f,240.0f,70.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("mainmenu"));
	m_resume = new ResumeButton({ 0,0,490,140 }, { 380.0f,420.0f,240.0f,70.0f },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("resume"));
	
	timer.start();
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
	
	for (int i = 0; i < 10; i++) {
		m_battery[i] = new Sprite({ 0,0,32,32 }, { Engine::Instance().GetLevel()[m_batteryX[i]][m_batteryY[i]]->GetDstP()->x,Engine::Instance().GetLevel()[m_batteryX[i]][m_batteryY[i]]->GetDstP()->y, 32, 32 },
			Engine::Instance().GetRenderer(), TEMA::GetTexture("battery"));
	}
	m_goal = new Sprite({ 226,37,12,7 }, { Engine::Instance().GetLevel()[72][166]->GetDstP()->x,Engine::Instance().GetLevel()[72][166]->GetDstP()->y, 32, 32 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("Key"));
	SOMA::Load("Aud/power.wav", "beep", SOUND_SFX);
	SOMA::Load("Aud/background_music2.wav", "BGM", SOUND_MUSIC);
	SOMA::Load("Aud/jump.wav", "jump", SOUND_SFX);
	SOMA::Load("Aud/hook_extension3.wav", "throw", SOUND_SFX);
	SOMA::Load("Aud/hook_grappling2.wav", "grab", SOUND_SFX);
	SOMA::Load("Aud/hook_retraction3.wav", "retract", SOUND_SFX);
	FOMA::RegisterFont("Img/LTYPE.TTF", "Font_1", 30);
	//SOMA::PlayMusic("BGM");
	
}

void Level1State::Update()
{
	
	if(COMA::AABBCheck(*m_pPlayer->GetDstP(),*m_goal->GetDstP()))
		m_stageEnd = true;
	if (Engine::Instance().Pause() == true)
	{
		if (m_mainMenu->Update() == 1)
			return;
		if (m_quit->Update() == 1)
			return;
		m_resume->Update();
	}
	if (EVMA::KeyHeld(SDL_SCANCODE_X))
	{
		Engine::Instance().Pause() = true;
	}
	
	if (Engine::Instance().Pause() == false)
	{
		for (unsigned i = 0; i < Engine::Instance().GetEnemy().size(); ++i)
		{
			Engine::Instance().GetEnemy()[i]->Update(m_pPlayer->GetVelX(),
				m_pPlayer->GetVelY(), m_pPlayer->BGScorllX(), m_pPlayer->BGScrollY(), m_pPlayer);
		}
		
		m_pMusicVolume = m_pMusicSetVol;
		m_pSFXVolume = m_pSFXSetVol;
		SOMA::SetSoundVolume(m_pSFXVolume);
		SOMA::SetMusicVolume(m_pMusicVolume);
		m_pause->Update();
		m_pPlayer->Update();
		m_goal->GetDstP()->x = Engine::Instance().GetLevel()[71][165]->GetDstP()->x;
		m_goal->GetDstP()->y = Engine::Instance().GetLevel()[71][165]->GetDstP()->y;
		
		
		for (auto i = 0; i < fDrone.size(); ++i)
		{
			fDrone[i]->Update(m_pPlayer->GetVelX(),
				m_pPlayer->GetVelY(), m_pPlayer->BGScorllX(), m_pPlayer->BGScrollY(), m_pPlayer);
		}
		m_pPlayer->Collision();
		
		m_updateTimer = m_defualtTimer + timer.getrunnningtime(timer);
		m_timer->SetText(m_updateTimer);
		m_energyNum = std::to_string(m_pPlayer->getEnergy());
		m_updateEnergy = m_defualtEnergy + m_energyNum;
		
		
		m_energy->SetText(m_updateEnergy);
		BulletCollision();
		for (auto i = 0; i < m_vEBullets.size(); ++i)
		{
			m_vEBullets[i]->Update();
		}
		if (EVMA::MousePressed(1)) {
			m_destinationX = EVMA::GetMousePos().x;
			m_destinationY = EVMA::GetMousePos().y;
			m_hook = new GrapplingHook({ 0,0,50,20 }, { m_pPlayer->GetDstP()->x, m_pPlayer->GetDstP()->y, 25, 10 },
				Engine::Instance().GetRenderer(), TEMA::GetTexture("hook"), 0.00, m_pPlayer, m_destinationX, m_destinationY);
			
		}
		if (m_hook != nullptr) {
			m_hook->Update();
			m_hook->Collision();
			if (m_hook->GetExist() == false) {
				m_hook = nullptr;
			}
		}
	}
	for (int i = 0; i < 10; i++) {
		if(m_battery[i] != nullptr){
			m_battery[i]->GetDstP()->x = Engine::Instance().GetLevel()[m_batteryX[i]][m_batteryY[i]]->GetDstP()->x;
		m_battery[i]->GetDstP()->y = Engine::Instance().GetLevel()[m_batteryX[i]][m_batteryY[i]]->GetDstP()->y;
		if (COMA::AABBCheck(*m_pPlayer->GetDstP(), *m_battery[i]->GetDstP())) {
				m_pPlayer->setEnergy(10);
				m_battery[i] = nullptr;
			}
		}
	}
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
	if (m_stageEnd){
		Engine::Instance().setScore(m_pPlayer->getEnergy());
		//change timer add
		Engine::Instance().setScore(timer.getmin() * 1000);
		Engine::Instance().setScore(timer.getsec() * 1000);
		STMA::ChangeState(new ScoreState);
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
	for (auto i = 0; i < m_vEBullets.size(); ++i)
	{
		m_vEBullets[i]->Render();
	}
	if (Engine::Instance().Pause() == false)
	{
		m_pause->Render();
	}
	for (auto i = 0; i < fDrone.size(); ++i)
	{
		fDrone[i]->Render();
	}
	m_pPlayer->Render();
	RenderLOS();
	//m_interface->Render();
	m_timer->Render();
	m_energy->Render();
	m_goal->Render();
	for (int i = 0; i < 10; i++) {
		if (m_battery[i] != nullptr)
			m_battery[i]->Render();
	}
	for (unsigned i = 0; i < Engine::Instance().GetEnemy().size();++i)
	{
		Engine::Instance().GetEnemy()[i]->Render();
	}
	//draw the hook
	if (m_hook != nullptr)
		m_hook->Render();
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


		SDL_Rect e = { m_pPlayer->GetDstP()->x, m_pPlayer->GetDstP()->y, 32, 32 };
		if (SDL_HasIntersection(&b, &e))
		{

			delete m_vEBullets[i];
			m_vEBullets[i] = nullptr;
			m_bullNull = true;
			if(!m_pPlayer->getBar())
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

void Level1State::RenderLOS()
{

	for (auto i = 0; i < fDrone.size(); ++i)
	{
		if ((MAMA::Distance(fDrone[i]->GetDstP()->x + fDrone[i]->GetDstP()->w / 2, m_pPlayer->GetDstP()->x + m_pPlayer->GetDstP()->w / 2,
			fDrone[i]->GetDstP()->y + fDrone[i]->GetDstP()->h / 2, m_pPlayer->GetDstP()->y + m_pPlayer->GetDstP()->h / 2) <= 500))
		{
			auto LOSColour = ((MAMA::Distance(fDrone[i]->GetDstP()->x + fDrone[i]->GetDstP()->w / 2, m_pPlayer->GetDstP()->x + m_pPlayer->GetDstP()->w / 2,
				fDrone[i]->GetDstP()->y + fDrone[i]->GetDstP()->h / 2, m_pPlayer->GetDstP()->y + m_pPlayer->GetDstP()->h / 2) <= 300)) ? glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(255.0f, 0.0f, 0.0f, 1.0f);
			DEMA::DrawLine({ int(m_pPlayer->GetDstP()->x + m_pPlayer->GetDstP()->w / 2), int(m_pPlayer->GetDstP()->y + m_pPlayer->GetDstP()->h / 2) },
				{ int(fDrone[i]->GetDstP()->x + fDrone[i]->GetDstP()->w / 2), int(fDrone[i]->GetDstP()->y + fDrone[i]->GetDstP()->h / 2) },
				{ Uint8(LOSColour.r), Uint8(LOSColour.g), Uint8(LOSColour.b), Uint8(LOSColour.a) });
		}
		
	
	}
}

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
		Engine::Instance().GetRenderer(), TEMA::GetTexture("hook"), 0.00, m_pPlayer, 0 ,0 );
	m_bullNull = false;
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

	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 }, 
		{ Engine::Instance().GetLevel2()[46][32]->GetDstP()->x,Engine::Instance().GetLevel2()[46][32]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[46][71]->GetDstP()->x,Engine::Instance().GetLevel2()[46][71]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[46][134]->GetDstP()->x,Engine::Instance().GetLevel2()[46][134]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[46][164]->GetDstP()->x,Engine::Instance().GetLevel2()[46][164]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[46][236]->GetDstP()->x,Engine::Instance().GetLevel2()[46][236]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[46][252]->GetDstP()->x,Engine::Instance().GetLevel2()[46][252]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[46][320]->GetDstP()->x,Engine::Instance().GetLevel2()[46][320]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[46][370]->GetDstP()->x,Engine::Instance().GetLevel2()[46][370]->GetDstP()->y,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));

	fDrone.push_back(new FireDrone({ 0,0,11,19 }, 
		{ Engine::Instance().GetLevel2()[19][52]->GetDstP()->x,Engine::Instance().GetLevel2()[19][52]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));
	fDrone.push_back(new FireDrone({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[30][135]->GetDstP()->x,Engine::Instance().GetLevel2()[30][135]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));
	fDrone.push_back(new FireDrone({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[17][172]->GetDstP()->x,Engine::Instance().GetLevel2()[17][172]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));
	fDrone.push_back(new FireDrone({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[34][200]->GetDstP()->x,Engine::Instance().GetLevel2()[34][200]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));
	fDrone.push_back(new FireDrone({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[24][230]->GetDstP()->x,Engine::Instance().GetLevel2()[24][230]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));
	fDrone.push_back(new FireDrone({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[26][253]->GetDstP()->x,Engine::Instance().GetLevel2()[26][253]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));
	fDrone.push_back(new FireDrone({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[14][300]->GetDstP()->x,Engine::Instance().GetLevel2()[14][300]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));
	fDrone.push_back(new FireDrone({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[8][353]->GetDstP()->x,Engine::Instance().GetLevel2()[8][353]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));
	fDrone.push_back(new FireDrone({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[30][343]->GetDstP()->x,Engine::Instance().GetLevel2()[30][343]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));
	fDrone.push_back(new FireDrone({ 0,0,11,19 },
		{ Engine::Instance().GetLevel2()[30][365]->GetDstP()->x,Engine::Instance().GetLevel2()[30][365]->GetDstP()->y,22.0f,38.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("firedrone"), 0, 0, 5, 5, &m_vEBullets));


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
	for (unsigned i = 0; i < Engine::Instance().GetEnemy().size(); ++i)
	{
		Engine::Instance().GetEnemy()[i]->Update(m_pPlayer->GetVelX(),
			m_pPlayer->GetVelY(), m_pPlayer->BGScorllX(), m_pPlayer->BGScrollY(), m_pPlayer);
	}
	m_pPlayer->Update(2);
	m_hook->Collision(2);
	m_hook->Update();
	m_pPlayer->Collision();
	for (auto i = 0; i < fDrone.size(); ++i)
	{
		fDrone[i]->Update(m_pPlayer->GetVelX(),
			m_pPlayer->GetVelY(), m_pPlayer->BGScorllX(), m_pPlayer->BGScrollY(), m_pPlayer);
	}
	for (auto i = 0; i < m_vEBullets.size(); ++i)
	{
		m_vEBullets[i]->Update();
	}
	BulletCollision();
	
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
	m_pPlayer->Render();
	RenderLOS();
	for (auto i = 0; i < m_vEBullets.size(); ++i)
	{
		m_vEBullets[i]->Render();
	}

	for (auto i = 0; i < fDrone.size(); ++i)
	{
		fDrone[i]->Render();
	}
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
			break;
		}

	}
	if (m_bullNull) CleanVector<Bullet*>(m_vEBullets, m_bullNull);
	for (int i = 0; i < (int)m_vEBullets.size(); i++)
	{
		SDL_Rect b = { m_vEBullets[i]->GetDstP()->x, m_vEBullets[i]->GetDstP()->y,
			m_vEBullets[i]->GetDstP()->w, m_vEBullets[i]->GetDstP()->h };
		for (int j = 0; j < Engine::Instance().GetPlatform2().size(); j++)
		{
			if (Engine::Instance().GetPlatform2()[j] == nullptr) continue;
			SDL_Rect e = { Engine::Instance().GetPlatform2()[j]->GetDstP()->x, Engine::Instance().GetPlatform2()[j]->GetDstP()->y, 32, 32 };
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
void Level2State::RenderLOS()
{
	for (auto i = 0; i < fDrone.size(); ++i)
	{
		if ((MAMA::Distance(fDrone[i]->GetDstP()->x + fDrone[i]->GetDstP()->w / 2, m_pPlayer->GetDstP()->x + m_pPlayer->GetDstP()->w / 2,
			fDrone[i]->GetDstP()->y + fDrone[i]->GetDstP()->h / 2, m_pPlayer->GetDstP()->y + m_pPlayer->GetDstP()->h / 2) <= 500))
		{
			auto LOSColour = ((MAMA::Distance(fDrone[i]->GetDstP()->x + fDrone[i]->GetDstP()->w / 2, m_pPlayer->GetDstP()->x + m_pPlayer->GetDstP()->w / 2,
				fDrone[i]->GetDstP()->y + fDrone[i]->GetDstP()->h / 2, m_pPlayer->GetDstP()->y + m_pPlayer->GetDstP()->h / 2) <= 300)) ? glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(255.0f, 0.0f, 0.0f, 1.0f);
			DEMA::DrawLine({ int(m_pPlayer->GetDstP()->x + m_pPlayer->GetDstP()->w / 2), int(m_pPlayer->GetDstP()->y + m_pPlayer->GetDstP()->h / 2) },
				{ int(fDrone[i]->GetDstP()->x + fDrone[i]->GetDstP()->w / 2), int(fDrone[i]->GetDstP()->y + fDrone[i]->GetDstP()->h / 2) },
				{ Uint8(LOSColour.r), Uint8(LOSColour.g), Uint8(LOSColour.b), Uint8(LOSColour.a) });
		}


	}
}
//End Level2State


TutorialState::TutorialState()
{
}
void TutorialState::Enter()
{
	std::cout << "Entering Level1State..." << std::endl;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	m_pPlayer = new Player({ 0,0,19,26 }, { 150.0f,500.0f,46.0f,64.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("playerIdle"), 0, 0, 4, 4);
	m_hook = new GrapplingHook({ 10,-2,10,10 }, { m_pPlayer->GetDstP()->x, m_pPlayer->GetDstP()->y, 30, 30 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("fireball"), 0.00, m_pPlayer, 0 ,0);
	Engine::Instance().GetEnemy().push_back(new Enemy({ 0,0,11,19 }, { 300,300,22,38 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("droneIdle"), 0, 0, 5, 5, 200));
	m_interface = new Sprite({ 6,455,224,44 }, { 400.0f,724.0f,224.0f,44.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("interface"));
	m_timer = new Label("font1", 900, 10, "Timer: ", { 255,255,255,255 });
	m_energy = new Label("font1", 410, 680, "Energy: ", { 255,255,255,255 });
	pressEnterL = new Label("font1", WIDTH / 2, HEIGHT / 2 +20, "Press Enter", { 255,255,255,255 });
	tuto1 = new Label("font1", WIDTH / 2 , HEIGHT / 2 , "WASD - Movement controll", { 255,255,255,255 });
	tuto2 = new Label("font1", WIDTH / 2 , HEIGHT / 2 , "Mouse right Click - Grappling Hook", { 255,255,255,255 });
	tuto3 = new Label("font1", WIDTH / 2 , HEIGHT / 2 , "Spacebar - Jump", { 255,255,255,255 });
	tuto4 = new Label("font1", WIDTH / 2 , HEIGHT / 2 , "Grean Batter - Fill energy", { 255,255,255,255 });
	tuto5 = new Label("font1", WIDTH / 2 , HEIGHT / 2 , "Energy - cost will be use for skill", { 255,255,255,255 });
	tuto6 = new Label("font1", WIDTH / 2 , HEIGHT / 2 , "Key - Goal of the stage", { 255,255,255,255 });
	explainKey = new Sprite({ 226,37,12,7 }, { WIDTH/2 -16, HEIGHT/2 - 16, 32, 32 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("Key"));
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
	m_MainMenu = new MainMenuButton({ 0,0,490,140 }, { 0.0f,0.0f,240.0f,70.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("mainmenu"));
	m_goal = new Sprite({ 226,37,12,7 }, { Engine::Instance().GetLevel()[20][81]->GetDstP()->x,Engine::Instance().GetLevel()[20][81]->GetDstP()->y, 32, 32 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("Key"));
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

	if (m_MainMenu->Update() == 1)
		return;
			
	if (COMA::AABBCheck(*m_pPlayer->GetDstP(), *m_goal->GetDstP()))
		m_stageEnd = true;
	
	if (EVMA::KeyReleased(SDL_SCANCODE_RETURN) && explainPause)
	{
		++pressEnter;
		if (pressEnter == 7)
			explainPause = false;
	}
	if (!explainPause)
	{
		m_pMusicVolume = m_pMusicSetVol;
		m_pSFXVolume = m_pSFXSetVol;
		SOMA::SetSoundVolume(m_pSFXVolume);
		SOMA::SetMusicVolume(m_pMusicVolume);
		m_pPlayer->Update();
		m_goal->GetDstP()->x = Engine::Instance().GetLevel()[20][81]->GetDstP()->x;
		m_goal->GetDstP()->y = Engine::Instance().GetLevel()[20][81]->GetDstP()->y;
		m_hook->Update();
		for (unsigned i = 0; i < Engine::Instance().GetEnemy().size(); ++i)
		{
			Engine::Instance().GetEnemy()[i]->Update(m_pPlayer->GetVelX(),
				m_pPlayer->GetVelY(), m_pPlayer->BGScorllX(), m_pPlayer->BGScrollY(), m_pPlayer);
		}
		m_pPlayer->Collision();
		m_hook->Collision();
		m_timer = new Label("font1", 900, 10, "Timer: ", { 255,255,255,255 });
		m_energy = new Label("font1", 410, 680, "Energy: ", { 255,255,255,255 });
		if (m_stageEnd)
			STMA::ChangeState(new TitleState);




	}

}

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
	m_interface->Render();
	m_timer->Render();
	m_energy->Render();
	m_goal->Render();
	if (explainPause)
		pressEnterL->Render();
	if (pressEnter == 1)
		tuto1->Render();
	if (pressEnter == 2)
		tuto2->Render();
	if (pressEnter == 3)
		tuto3->Render();
	if (pressEnter == 4)
		tuto4->Render();
	if (pressEnter == 5)
		tuto5->Render();
	if (pressEnter == 6)
	{
		explainKey->Render();
		tuto6->Render();
	}
	for (unsigned i = 0; i < Engine::Instance().GetEnemy().size(); ++i)
	{
		Engine::Instance().GetEnemy()[i]->Render();
	}
	//draw the hook
	if (m_hook->GetExist() == true)
		m_hook->Render();
	m_MainMenu->Render();
	// If GameState != current state.
	if (dynamic_cast<Level1State*>(STMA::GetStates().back()))
		State::Render();
	
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





// Begin TitleState.
TitleState::TitleState() {

	
}

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
