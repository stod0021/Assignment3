/**
* \class Enemy.h
* \brief A class that represents an Enemy that can kill Qbert.
* \author David Stoddard
* \date March 26, 2015
*/
#pragma once 
#include <GameObject.h>

struct Vertex;

class Enemy :public GameObject
{
public:

	Enemy();

	Enemy(Vector3 position);

	void Initialize(Graphics *graphics);

	void Update(float dt);

	void Draw(Graphics *graphics, Matrix4x4 relativeTo, float dt);

	void Enemy::SetSpawnDelay(float delay);

	void Enemy::SetIsLive(bool live);

	bool Enemy::GetIsLive();

	void Enemy::Reset();

protected:
	float _move1StepTimer;
	float _spawnTimer;
	bool _isAlive;
	bool _moving;
	float _spawnDelay;
	float _moveDelay;
	Vector3 *vertices;
	Vector4 *colours;

	float r, g, b;

	int size;
	int offset;

	unsigned int* indices;

	void SetVertex(int index, float x, float y, float z, float r, float g, float b, float a);

};
