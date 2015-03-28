/**
* \class Player.h
* \brief A class that represents QBert also known as the player character.
* \author David Stoddard
* \date March 26, 2015
*/
#pragma once

#include <GameObject.h>

struct Vertex;

class Player : public GameObject
{
public:
	Player();
	Player(Vector3 position);
	/**
	* \fn void Game::InitializeImpl(Graphics *graphics)
	* \brief Initialize main game environment and players is drawn to the screen.
	* \param openGL graphics draws.
	*/
	void Initialize(Graphics *graphics);
	/**
	* \fn void Game::Update(float dt)
	* \brief Update game to check for ingame interactions such as player and enemy movement or falling off world.
	* \param dt keeps count on delta time.
	*/
	void Update(float dt);

	void Draw(Graphics *graphics,Matrix4x4 relativeTo, float dt);

protected:
	//void *_vertexPointer;

	Vector3 *vertices;
	Vector4 *colours;

	float r, g, b;

	int size;
	int offset;

	//Vertex *vertices;
	unsigned int* indices;

	void SetVertex(int index, float x, float y, float z, float r, float g, float b, float a);
};