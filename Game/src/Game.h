/**
* \class Game.h
* \brief A class that represents the main game interactions between QBert, the world and enemies.
* \author David Stoddard
* \date March 26, 2015
*/
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
  /**
  * \fn void Game::InitializeImpl()
  * \brief Initialize main game environment and players is drawn to the screen.
  * \param renderer The SDL renderer used to draw the object.
  */
  void InitializeImpl();
  void UpdateImpl(float dt);
  void DrawImpl(Graphics *graphics, float dt);

  void GoToNextLvl();
  void GameOver();
  void EnemyHitPlayer(Player* player);
  void FallOffWorld(Player* player);
  void PlaySound(Mix_Chunk *sound);
  bool CheckIfAllCubesTouched(bool win);
  bool V3Collision(Vector3 object1, Vector3 object2);
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

  bool win;
  bool untouch;

  // Enemy
 int enemyNumbers;
 Enemy _enemyAI[3];
 int _totalEnemies;
 unsigned int _timer;
 float _spawnTimer;
 float _spawnTimerTotal;

 Cube** _worldCubes;

 char _score[200];

 int _gridWidth;
 int _gridHeight;

 //The sound effects that will be used
 Mix_Chunk *_fallSound;
 Mix_Chunk *_landSound;
 Mix_Chunk *_qbertBeep1Sound;
 Mix_Chunk *_qbertBeep2Sound;
 Mix_Chunk *_deadSound;
 Mix_Chunk *_reallyDeadSound;
 Mix_Chunk *_nextLvlSound;
};