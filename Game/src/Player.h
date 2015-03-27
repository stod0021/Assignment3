#pragma once

#include <GameObject.h>

struct Vertex;

class Player : public GameObject
{
public:
	Player();
	Player(Vector3 position);

	void Initialize(Graphics *graphics);
	void Update(float dt);
	void Draw(Graphics *graphics, float dt);

	///**
	//* \fn Vector3 Player::SetPosition()
	//* \brief A function that is used to set the player position.
	//* \return Vector3 The position fo the player.
	//*/
	//void SetPosition(Vector3 pos);
	///**
	//* \fn Vector3 Player::GetPosition()
	//* \brief A function that is used to get the player sposition.
	//* \return Vector3 The position fo the player.
	//*/
	//Vector3 GetPosition();

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