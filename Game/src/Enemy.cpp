#include "Enemy.h"
#include "Cube.h"

#include <iostream>

void Enemy::Initialize(Graphics *graphics)
{
	_move1StepTimer = 10.0f;
//	_enemyCube->GetTransform().scale = Vector3(0.5f, 0.5f, 0.5f);
}

void Enemy::Update(float dt)
{
_enemyCube->GetTransform().position = _transform.position;
	_enemyCube->GetTransform().scale = _transform.scale;}

void Enemy::Draw(Graphics *graphics, float dt)
{
	_enemyCube->Draw(graphics, dt);
}


void Enemy::SetSpawnDelay(float delay)
{
	_spawnTimer = delay;
}

void Enemy::SetIsLive(bool live)
{
	_isAlive = live;
}

bool Enemy::GetIsLive()
{
	return _isAlive;
}

//void Enemy::Reset()
//{
//	_isAlive = false;
//	int random = rand() % 2;
//	if (random == 0)
//	{
//		_transform.position = Vector3(1, 0, 0);
//	}
//	else
//	{
//		_transform.position = Vector3(0, 0, 1);
//	}
//}