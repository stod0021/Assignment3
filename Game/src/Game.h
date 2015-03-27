#pragma once

#include <GameEngine.h>
#include <SDL_mixer.h>
#include "Cube.h"
#include "Player.h"
#include "Enemy.h"

union SDL_Event;
class Graphics;
class Camera;

class Game: public GameEngine
{
  friend class GameEngine;

public:
  ~Game();

protected:
  Game();

  void InitializeImpl(Graphics *graphics);
  void UpdateImpl(float dt);
  void DrawImpl(Graphics *graphics, float dt);
  void GoToNextLvl();
  void ResetAndDead();
  void FallOffWorld(Player* player);
  void PlaySound(Mix_Chunk *sound);
  void CalculateDrawOrder(std::vector<GameObject *>& drawOrder);
  void CalculateCameraViewpoint();

  Camera *_camera;
  Player _player;

  unsigned int _worldSize;
  unsigned int _lvl;

	 unsigned int _playerScore;
	 unsigned int _playerScoreTimer;
	 unsigned int _playerLife;
  float _playerFallY;
  Vector3 _playerPos;

  int enemyNumbers;
  Enemy *_enemyAI;
 // Enemy *_enemyAI;
  unsigned int _timer;
  float _spawnTimer;
  float _spawnTimerTotal;

  Cube** _worldCubes;

 char _score[200];

 int _gridWidth;
 int _gridHeight;

 //The sound effects that will be used
 Mix_Chunk *_fall;
 Mix_Chunk *_land;
 Mix_Chunk *_qbertBeep1;
 Mix_Chunk *_qbertBeep2;
 Mix_Chunk *_dead;
 Mix_Chunk *_reallyDead;
 Mix_Chunk *_nextLvl;
};