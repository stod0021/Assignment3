#pragma once 
#include <GameObject.h>

class Cube;

class Enemy :public GameObject
{
public:

	void Initialize(Graphics *graphics);

	void Update(float dt);

	void Draw(Graphics *graphics, float dt);

	void Enemy::SetSpawnDelay(float delay);

	void Enemy::SetIsLive(bool live);

	bool Enemy::GetIsLive();

	//void Enemy::Reset();

protected:
	Cube *_enemyCube;
	float _move1StepTimer;
	float _spawnTimer;
	bool _isAlive;
};
