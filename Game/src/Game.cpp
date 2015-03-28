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
	//free memory
	delete _camera;
	_camera = nullptr;

	for (unsigned int i = 0; i < _worldSize; i++){
		delete _worldCubes[i];
		_worldCubes[i] = nullptr;
	}
	free(_worldCubes);
	_worldCubes = nullptr;

	Mix_FreeChunk(_fallSound);
	Mix_FreeChunk(_landSound);
	Mix_FreeChunk(_qbertBeep1Sound);
	Mix_FreeChunk(_qbertBeep2Sound);
	Mix_FreeChunk(_deadSound);
	Mix_FreeChunk(_reallyDeadSound);
	Mix_FreeChunk(_nextLvlSound);

	//_mixChunk
	_fallSound = NULL;
	_landSound = NULL;
	_qbertBeep1Sound = NULL;
	_qbertBeep2Sound = NULL;
	_deadSound = NULL;
	_reallyDeadSound = NULL;
	_nextLvlSound = NULL;
	Mix_CloseAudio();
	Mix_Quit();

}

void Game::InitializeImpl()
{
	win = false;

	 _playerScore = 0;
	 _playerLife = 3;
	 _lvl = 1;
	 sprintf_s(_score, "Player Score %d |QBERT GAME Level_%d| Player Life %d", _playerScore, _lvl, _playerLife);
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

  ////Spawn Enemy
  for (int i = 0; i < 3; i++)
	  {
	  int random = rand() % 2;
	  if (random == 0)
	  {
		  _enemyAI[i].GetTransform().position=(Vector3(1, 0, 0));
	  }
	  else
	  {
		  _enemyAI[i].GetTransform().position = (Vector3(1, 0, 0));
	  }

	  _objects.push_back(&_enemyAI[i]);
	  _enemyAI[i].SetSpawnDelay((i + 1)*(5.0f / _lvl));
	  _enemyAI[i].SetIsLive(false);
	 }

 // _player.Initialize(graphics);
  _objects.push_back(&_player);
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
		  
		  _objects.push_back(&_worldCubes[gridX][gridZ]);

		  index++;
	  }
	  offSetY++;
  }
  _worldSize = index;

  for (auto itr = _objects.begin(); itr != _objects.end(); itr++)
  {
	  (*itr)->Initialize(_graphicsObject);
  }


  Mix_Init(MIX_INIT_MP3);

  int success = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == 0;
  if (success){
	  _fallSound = Mix_LoadWAV("res/falling.wav");
	  if (_fallSound == nullptr)
	  {
		  printf("Failed to load %s", _fallSound);
		  exit(0);
	  }

	  _landSound = Mix_LoadWAV("res/land.wav");
	  if (_landSound == nullptr)
	  {
		  printf("Failed to load %s", _landSound);
		  exit(0);
	  }

	  _qbertBeep1Sound = Mix_LoadWAV("res/fx_17a.wav");
	  if (_qbertBeep1Sound == nullptr)
	  {
		  printf("Failed to load %s", _qbertBeep1Sound);
		  exit(0);
	  }

	  _qbertBeep2Sound = Mix_LoadWAV("res/fx_19c.wav");
	  if (_qbertBeep2Sound == nullptr)
	  {
		  printf("Failed to load %s", _qbertBeep2Sound);
		  exit(0);
	  }

	  _deadSound = Mix_LoadWAV("res/fx_28.wav");
	  if (_deadSound == nullptr)
	  {
		  printf("Failed to load %s", _deadSound);
		  exit(0);
	  }

	  _reallyDeadSound = Mix_LoadWAV("res/fx_36.wav");
	  if (_reallyDeadSound == nullptr)
	  {
		  printf("Failed to load %s", _reallyDeadSound);
		  exit(0);
	  }
	  _nextLvlSound = Mix_LoadWAV("res/never-ending-upward-analog.wav");
	  if (_nextLvlSound == nullptr)
	  {
		  printf("Failed to load %s", _nextLvlSound);
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
		GameOver();
	}

	bool move = false;

	//Player to Obstacle Interactions
	for (int i = 0; i < 3; i++)
	{
		if (_enemyAI[i].GetIsLive())
		{
			if (V3Collision(_enemyAI[i].GetTransform().position, _playerPos))
			{
				EnemyHitPlayer(&_player);
				break;
			}
		}
	}

		if (im->GetKeyState(SDLK_UP, SDL_KEYUP) == true) //up right
		{// move up grid 1 space towards upper right
			_playerPos.x = _player.GetTransform().position.x;
			_playerPos.y = _player.GetTransform().position.y + 1.0f;
			_playerPos.z = _player.GetTransform().position.z - 1.0f;
			PlaySound(_qbertBeep1Sound);
			move = true;
		}
		else if (im->GetKeyState(SDLK_DOWN, SDL_KEYUP) == true) // down left
		{
			// move down grid 1 space towards lower left
			_playerPos.x = _player.GetTransform().position.x;
			_playerPos.y = _player.GetTransform().position.y - 1.0f;
			_playerPos.z = _player.GetTransform().position.z + 1.0f;
			PlaySound(_qbertBeep1Sound);
			move = true;
		}
		else if (im->GetKeyState(SDLK_LEFT, SDL_KEYUP) == true) //up left
		{
			// move up grid 1 space towards upper left
			_playerPos.x = _player.GetTransform().position.x - 1.0f;
			_playerPos.y = _player.GetTransform().position.y + 1.0f;
			_playerPos.z = _player.GetTransform().position.z;
			PlaySound(_qbertBeep1Sound);
			move = true;
		}
		else if (im->GetKeyState(SDLK_RIGHT, SDL_KEYUP) == true) // downright
		{
			// move down grid 1 space towards lower right
			_playerPos.x = _player.GetTransform().position.x + 1.0f;
			_playerPos.y = _player.GetTransform().position.y - 1.0f;
			_playerPos.z = _player.GetTransform().position.z;
			PlaySound(_qbertBeep1Sound);
			move = true;
		}
	//Enemy
		

		_player.GetTransform().position = _playerPos;
		//Check Player gall
		if (_player.GetTransform().position.x == -1 || _player.GetTransform().position.z == -1 || _player.GetTransform().position.y == _playerFallY)
		{
			FallOffWorld(&_player);
		}

		if (move == true){
			//printf("Z:%d,X:%d,Y:%d\n", (int)_playerPos.z, (int)_playerPos.x, (int)_playerPos.y);
							if (_worldCubes[(int)_playerPos.x][(int)_playerPos.z].GetIsTouched() == false){
								_playerScore += 10;
								sprintf_s(_score, "Player Score %d |QBERT GAME Level_%d| Player Life %d", _playerScore, _lvl, _playerLife);
								SDL_SetWindowTitle(_window, _score);
								_worldCubes[(int)_playerPos.x][(int)_playerPos.z].SetIsTouched(true);
							}	
			move = false;
		}

		win = CheckIfAllCubesTouched(win);
		if (win == true)
			GoToNextLvl();

	
		//Update cube tiles.
		//int offSetY = 0;
		//for (int gridX = 0; gridX < _gridHeight; gridX++)
		//{
		//	for (int gridZ = 0; gridZ < _gridWidth - offSetY; gridZ++)
		//	{	
		//		//_worldCubes[gridX][gridZ].Update(dt);
		//		_worldCubes[gridX][gridZ].Update(dt);
		//	}
		//	offSetY++;
		//}

		////Update Player
		//_player.Update(dt);
		for (auto itr = _objects.begin(); itr != _objects.end(); itr++)
		{
			(*itr)->Update(dt);
		}
}

void Game::DrawImpl(Graphics *graphics, float dt)
{
	std::vector<GameObject *> renderOrder = _objects;
	// CalculateDrawOrder(renderOrder);
	glPushMatrix();
	CalculateCameraViewpoint();

  for (auto itr = renderOrder.begin(); itr != renderOrder.end(); itr++)
  {
	  (*itr)->Draw(graphics, _camera->GetProjectionMatrix(), dt);
  }

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
	win = false;
	_playerPos = Vector3(0, 1, 0);

	printf("You win level %d\n", _lvl);

	PlaySound(_nextLvlSound);
	_playerScore += 1000; // bonus points
	_lvl++;

	sprintf_s(_score, "Player Score %d |QBERT GAME Level_%d| Player Life %d", _playerScore, _lvl, _playerLife);
	SDL_SetWindowTitle(_window, _score);

	int offSet = 0;
	for (int gridX = 0; gridX < _gridHeight; gridX++)
	{
		for (int gridZ = 0; gridZ < _gridWidth - offSet; gridZ++)
		{
			_worldCubes[gridX][gridZ].SetIsTouched(false);
			_worldCubes[gridX][gridZ].Reset();
		}
		offSet++;
	}
	//Reset enemies and increase difficulty
	for (int i = 0; i < 3; i++)
	{
		_enemyAI[i].Reset();
		_enemyAI[i].SetSpawnDelay((i + 1)*(5.0f / _lvl));
	}
}

void Game::GameOver()
{
	PlaySound(_reallyDeadSound);
	printf("GAME OVER\n! Press enter to continue!\n");
		getchar();

	_playerPos = Vector3(0, 1, 0);
	_lvl = 1;
	_playerLife = 3;
	_playerScore = 0;
	sprintf_s(_score, "Player Score %d |QBERT GAME Level_%d| Player Life %d", _playerScore, _lvl, _playerLife);
	SDL_SetWindowTitle(_window, _score);
	for (int i = 0; i < 3; i++)
	{
		_enemyAI[i].Reset();
		_enemyAI[i].SetSpawnDelay((i + 1)*(5.0f / _lvl));
	}
	int offSet = 0;

	for (int gridX = 0; gridX < _gridHeight; gridX++)
	{
		for (int gridZ = 0; gridZ < _gridWidth - offSet; gridZ++)
		{
			_worldCubes[gridX][gridZ].SetIsTouched(false);
			_worldCubes[gridX][gridZ].Reset();
		}
		offSet++;
	}
}
void Game::EnemyHitPlayer(Player* player){
	PlaySound(_deadSound);
	_playerLife--;
	sprintf_s(_score, "Player Score %d |QBERT GAME Level_%d| Player Life %d", _playerScore, _lvl, _playerLife);
	SDL_SetWindowTitle(_window, _score);
	_playerPos = Vector3(0, 1, 0);

	for (int i = 0; i < 3; i++)
	{
		_enemyAI[i].Reset();
		_enemyAI[i].SetSpawnDelay((i + 1)*(5.0f / _lvl));
	}
}
void Game::FallOffWorld(Player* player){
	//Fall points
		PlaySound(_fallSound);
		_playerPos = Vector3(0, 1, 0);
		_playerScore = _playerScore;
		_playerLife--;

		sprintf_s(_score, "Player Score %d |QBERT GAME Level_%d| Player Life %d", _playerScore, _lvl, _playerLife);
		SDL_SetWindowTitle(_window, _score);
}
bool Game::CheckIfAllCubesTouched(bool win)
{
	int offSet = 0;
	int j = 0;

	for (int gridX = 0; gridX < _gridHeight; gridX++)
	{
		for (int gridZ = 0; gridZ < _gridWidth - offSet; gridZ++)
		{
			if (_worldCubes[gridX][gridZ].GetIsTouched() == true)
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
	if (j == _worldSize)
		win = true;
	else
		win = false;

	return win;
}

bool Game::V3Collision(Vector3 object1, Vector3 object2)
{
	if
		(
		object1.x == object2.x &&
		object1.y == object2.y &&
		object1.z == object2.z
		)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void Game::PlaySound(Mix_Chunk *sound){
	Mix_PlayChannel(-1, sound,0);
}