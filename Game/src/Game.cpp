#include "Game.h"
#include <GameObject.h>
#include <SDL.h>
#include <math.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <InputManager.h>
#include <iostream>
#include <Cameras/Camera.h>
#include <Cameras/PerspectiveCamera.h>
#include <Cameras/OrthographicCamera.h>

// Initializing our static member pointer.
GameEngine* GameEngine::_instance = nullptr;

GameEngine* GameEngine::CreateInstance()
{
  if (_instance == nullptr)
  {
    _instance = new Game();
  }
  return _instance;
}

Game::Game() : GameEngine()
{

}

Game::~Game()
{
	delete _camera;
	_camera = nullptr;

	delete _enemyAI;
	_enemyAI = nullptr;

	for (unsigned int i = 0; i < _worldSize; i++){
		delete _worldCubes[i];
		_worldCubes[i] = nullptr;
	}

	
	Mix_CloseAudio();
	Mix_Quit();

	Mix_FreeChunk(_fall);
	Mix_FreeChunk(_land);
	Mix_FreeChunk(_qbertBeep1);
	Mix_FreeChunk(_qbertBeep2);
	Mix_FreeChunk(_dead);
	Mix_FreeChunk(_reallyDead);
	Mix_FreeChunk(_nextLvl);

	//_mixChunk
	_fall = NULL;
	_land = NULL;
	_qbertBeep1 = NULL;
	_qbertBeep2 = NULL;
	_dead = NULL;
	_reallyDead = NULL;
	_nextLvl = NULL;
}

void Game::InitializeImpl(Graphics *graphics)
{
	 _playerScore = 0;
	 _playerLife = 3;
	 _lvl = 1;
	 sprintf_s(_score, "Player Score %d |QBERT GAME Level-%d| Player Life %d", _playerScore, _lvl, _playerLife);
	SDL_SetWindowTitle(_window, _score);
	_spawnTimer = 8;

   _gridWidth  = 6; // Change this
   _gridHeight = _gridWidth;
   int offSet = _gridHeight;

   //Scales the fall ceilling down or up depending how big the level is
   _playerFallY = (_gridWidth * -1.0f) + 1.0f;
   
  float cube=0; //testing purposes
   // pyramid shape instead of bipyramid

  float nearPlane = 0.01f;
  float farPlane = 100.0f;

  Vector4 position(3.5f, 2.5f, 2.5f, 0.0f);
  Vector4 lookAt = Vector4::Normalize(Vector4::Difference(Vector4(0.0f, 0.0f, 0.0f, 0.0f), position));
  Vector4 up(0.0f, 1.0f, 0.0f, 0.0f);

  //_camera = new PerspectiveCamera(100.0f, 1.0f, nearPlane, farPlane, position, lookAt, up);
  _camera = new OrthographicCamera(-10.0f, 10.0f, 10.0f, -10.0f, nearPlane, farPlane, position, lookAt, up);

  //Spawn player
  _playerPos = Vector3(0, 1, 0);
  _player.GetTransform().position=_playerPos;

  //Spawn Enemy
  int enemyNumbers= 1 + _lvl;
  Enemy *_enemyAI;
  _enemyAI = new Enemy();

  ////Spawn Enemy
  //for (int i = 0; i < enemyNumbers; i++)
	 // {
	 // int random = rand() % 2;
	 // if (random == 0)
	 // {
		//  _enemyAI[i].GetTransform().position=(Vector3(1, 0, 0));
	 // }
	 // else
	 // {
		//  _enemyAI[i].GetTransform().position = (Vector3(1, 0, 0));
	 // }

	 // _enemyAI[i].Initialize(graphics);
	 // _enemyAI[i].SetSpawnDelay((i + 1)*(5.0f / _lvl));
	 // _enemyAI[i].SetIsLive(false);
	 // }

  _player.Initialize(graphics);

  int offSetY = 0;
  //Creating our grid.
  _worldCubes = (Cube**)malloc(sizeof(Cube*)* _worldSize);
  int index = 0;
  //initialize world cubes
  for (int gridX = 0; gridX < _gridHeight; gridX++)
  {
	  for (int gridZ = 0; gridZ < _gridWidth-offSetY; gridZ++)
	  {
		  _worldCubes[index] = new Cube[(int)(_gridWidth * _gridHeight) - offSet]();
		  //world coordinates
		  float worldX = (float)gridX;
		  float worldY = (float)-(gridX + gridZ);
		  float worldZ = (float)gridZ;
		  printf("x:%f,z:%f\n", worldX, worldZ);

		  _worldCubes[gridX][gridZ].GetTransform().position = Vector3(worldX, worldY, worldZ);
		  /*The issue was simpler than I thought and had to switch gridz, gridx to gridx,gridz. At first I thought it was
		  an issue with _pushback but as you can see I rewrote and didn't comment out the parts I rewrote.*/
		  _worldCubes[gridX][gridZ].Initialize(graphics);
		  index++;
	  }
	  offSetY++;
  }
  _worldSize = index;
  /*for (size_t i = 0; i < length; i++)
  {

  }*/

  Mix_Init(MIX_INIT_MP3);

  int success = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == 0;
  if (success){
	  _fall = Mix_LoadWAV("res/falling.wav");
	  if (_fall == nullptr)
	  {
		  printf("Failed to load %s", _fall);
		  exit(0);
	  }

	  _land = Mix_LoadWAV("res/land.wav");
	  if (_land == nullptr)
	  {
		  printf("Failed to load %s", _land);
		  exit(0);
	  }

	  _qbertBeep1 = Mix_LoadWAV("res/fx_17a.wav");
	  if (_qbertBeep1 == nullptr)
	  {
		  printf("Failed to load %s", _qbertBeep1);
		  exit(0);
	  }

	  _qbertBeep2 = Mix_LoadWAV("res/fx_19c.wav");
	  if (_qbertBeep2 == nullptr)
	  {
		  printf("Failed to load %s", _qbertBeep2);
		  exit(0);
	  }

	  _dead = Mix_LoadWAV("res/fx_28.wav");
	  if (_dead == nullptr)
	  {
		  printf("Failed to load %s", _dead);
		  exit(0);
	  }

	  _reallyDead = Mix_LoadWAV("res/fx_36.wav");
	  if (_reallyDead == nullptr)
	  {
		  printf("Failed to load %s", _reallyDead);
		  exit(0);
	  }
	  _nextLvl = Mix_LoadWAV("res/never-ending-upward-analog.wav");
	  if (_nextLvl == nullptr)
	  {
		  printf("Failed to load %s", _nextLvl);
		  exit(0);
	  }

  }
}

void Game::UpdateImpl(float dt)
{
	//SDL_Event evt;
	//SDL_PollEvent(&evt);
	InputManager *im = InputManager::GetInstance();
	im->Update(dt);

	if (_playerLife == 0)
	{
		ResetAndDead();
	}

	bool move = false;
	_spawnTimerTotal = _spawnTimer - dt;
	_timer = _timer + dt;
	_playerScoreTimer /= _timer;
	if (_spawnTimer == 0)
	{
		/*_enemyAI = new Enemy();
		_enemyAI->GetTransform().position = Vector3(1,0,1);*/
	}

	//Enemy movement


//	if (im->GetKeyState(SDL_KEYUP, SDL_KEYUP) == true){
		if (im->GetKeyState(SDLK_UP, SDL_KEYUP) == true) //up right
		{// move up grid 1 space towards upper right
			_playerPos.x = _player.GetTransform().position.x;
			_playerPos.y = _player.GetTransform().position.y + 1.0f;
			_playerPos.z = _player.GetTransform().position.z - 1.0f;
			PlaySound(_qbertBeep1);
			move = true;
		}
		else if (im->GetKeyState(SDLK_DOWN, SDL_KEYUP) == true) // down left
		{
			// move down grid 1 space towards lower left
			_playerPos.x = _player.GetTransform().position.x;
			_playerPos.y = _player.GetTransform().position.y - 1.0f;
			_playerPos.z = _player.GetTransform().position.z + 1.0f;
			PlaySound(_qbertBeep1);
			move = true;
		}
		else if (im->GetKeyState(SDLK_LEFT, SDL_KEYUP) == true) //up left
		{
			// move up grid 1 space towards upper left
			_playerPos.x = _player.GetTransform().position.x - 1.0f;
			_playerPos.y = _player.GetTransform().position.y + 1.0f;
			_playerPos.z = _player.GetTransform().position.z;
			PlaySound(_qbertBeep1);
			move = true;
		}
		else if (im->GetKeyState(SDLK_RIGHT, SDL_KEYUP) == true) // downright
		{
			// move down grid 1 space towards lower right
			_playerPos.x = _player.GetTransform().position.x + 1.0f;
			_playerPos.y = _player.GetTransform().position.y - 1.0f;
			_playerPos.z = _player.GetTransform().position.z;
			PlaySound(_qbertBeep1);
			move = true;
		}
		_player.GetTransform().position = _playerPos;

		if (_player.GetTransform().position.x == -1 || _player.GetTransform().position.z == -1 || _player.GetTransform().position.y == _playerFallY)
		{
			FallOffWorld(&_player);
		}


		if (move == true){

			printf("Z:%d,X:%d,Y:%d\n", (int)_playerPos.z, (int)_playerPos.x, (int)_playerPos.y);
			//printf("Index:%d\n", _cubeIndex);
				if (_playerPos.z >= 0) {
						//hmmm safeguard for now...
							if (_worldCubes[(int)_playerPos.x][(int)_playerPos.z].GetIsTouched() == false){
								_playerScore += 10;
								sprintf_s(_score, "Player Score %d |QBERT GAME Level-%d| Player Life %d", _playerScore, _lvl, _playerLife);
								SDL_SetWindowTitle(_window, _score);
								_worldCubes[(int)_playerPos.x][(int)_playerPos.z].SetIsTouched(true);
							}
						
					}
			move = false;
		}

		//Tile Check
		int offSet = 0;
		int j = 0;
		for (int gridX = 0; gridX < _gridHeight; gridX++)
		{
			for (int gridZ = 0; gridZ < _gridWidth - offSet; gridZ++)
			{
				if (_worldCubes[gridX][gridZ].GetIsTouched())
					{
						j++;
						
					}
				else
					{
						break;
					}
				}
				offSet++;
		}
		//std::cout << '\r' << j << std::endl;
		//printf("Worldsize%d\n", _worldSize);
		if (j == _worldSize) //need 21
		{
			printf("You win level %d\n", _lvl);
			GoToNextLvl();
		}

		//Update cube tiles.
		int offSetY = 0;
		for (int gridX = 0; gridX < _gridHeight; gridX++)
		{
			for (int gridZ = 0; gridZ < _gridWidth - offSetY; gridZ++)
			{	
				_worldCubes[gridX][gridZ].Update(dt);
			}
			offSetY++;
		}

		//Update Player
		_player.Update(dt);
}

void Game::DrawImpl(Graphics *graphics, float dt)
{
  glPushMatrix();
  CalculateCameraViewpoint();

  int offSetY = 0;
  for (int gridX = 0; gridX < _gridHeight; gridX++)
  {
	  for (int gridZ = 0; gridZ < _gridWidth - offSetY; gridZ++)
	  {
		  _worldCubes[gridX][gridZ].Draw(graphics, dt);
	  }
	  offSetY++;
  }
  _player.Draw(graphics, dt);
//  _enemyAI->Draw(graphics, dt);

  glPopMatrix();

}

void Game::CalculateDrawOrder(std::vector<GameObject *>& drawOrder)
{
  // SUPER HACK GARBAGE ALGO.
  drawOrder.clear();

  auto objectsCopy = _objects;
  auto farthestEntry = objectsCopy.begin();
  while (objectsCopy.size() > 0)
  {
    bool entryFound = true;
    for (auto itr = objectsCopy.begin(); itr != objectsCopy.end(); itr++)
    {
      if (farthestEntry != itr)
      {
        if ((*itr)->GetTransform().position.y < (*farthestEntry)->GetTransform().position.y)
        {
          entryFound = false;
          farthestEntry = itr;
          break;
        }
      }
    }

    if (entryFound)
    {
      GameObject *farthest = *farthestEntry;

      drawOrder.push_back(farthest);
      objectsCopy.erase(farthestEntry);
      farthestEntry = objectsCopy.begin();
    }
  }
}

void Game::CalculateCameraViewpoint()
{
  Vector4 xAxis(1.0f, 0.0f, 0.0f, 0.0f);
  Vector4 yAxis(0.0f, 1.0f, 0.0f, 0.0f);
  Vector4 zAxis(0.0f, 0.0f, 1.0f, 0.0f);

  Vector3 cameraVector(_camera->GetLookAtVector().x, _camera->GetLookAtVector().y, _camera->GetLookAtVector().z);
  Vector3 lookAtVector(0.0f, 0.0f, -1.0f);

  Vector3 cross = Vector3::Normalize(Vector3::Cross(cameraVector, lookAtVector));
  float dot = MathUtils::ToDegrees(Vector3::Dot(lookAtVector, cameraVector));

  glRotatef(cross.x * dot, 1.0f, 0.0f, 0.0f);
  glRotatef(cross.y * dot, 0.0f, 1.0f, 0.0f);
  glRotatef(cross.z * dot, 0.0f, 0.0f, 1.0f);

  glTranslatef(-_camera->GetPosition().x, -_camera->GetPosition().y, -_camera->GetPosition().z);
}

void Game::GoToNextLvl()
{
	PlaySound(_nextLvl);
	_playerScore = _playerScore * _playerScoreTimer;
	_lvl++;
	sprintf_s(_score, "Player Score %d |QBERT GAME Level-%d| Player Life %d", _playerScore, _lvl, _playerLife);
	SDL_SetWindowTitle(_window, _score);

	_playerPos = Vector3(0, 1, 0);

	if (_spawnTimerTotal < 2.0f){
		_spawnTimerTotal = 1.0f;
	}
	else
	{
		_spawnTimerTotal = _spawnTimer - 1.0f;
	}
}
void Game::ResetAndDead()
{
	printf("GAME OVER\n! Press Spacebar to continue!\n");
		getchar();
	_playerPos = Vector3(0, 1, 0);
	_playerLife = 3;
	_playerScore = 0;
	_spawnTimer = 10.0f;
}

void Game::FallOffWorld(Player* player){
	//Fall points
		PlaySound(_fall);
		_playerPos = Vector3(0, 1, 0);
		_playerScore = _playerScore;
		_playerLife = _playerLife - 1;

		sprintf_s(_score, "Player Score %d |QBERT GAME| Player Life %d", _playerScore, _playerLife);
		SDL_SetWindowTitle(_window, _score);
}

void Game::PlaySound(Mix_Chunk *sound){
	Mix_PlayChannel(-1, sound,0);
}