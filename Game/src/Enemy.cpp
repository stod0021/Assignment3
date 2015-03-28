#include "Enemy.h"
#include <SDL_opengl.h>
#include <iostream>
#include <InputManager.h>
#include <stdlib.h>
#include <ctime>
#include <random>

struct Vertex
{
	Vector3 position;
	Vector4 color;
};

Enemy::Enemy()
{
	_transform.position = Vector3::Zero();
	_transform.rotation = Vector3::Zero();
	_transform.scale = Vector3::One();
}

Enemy::Enemy(Vector3 position)
{
	_transform.position = position;
	_transform.rotation = Vector3::Zero();
	_transform.scale = Vector3::One();
}
void Enemy::Initialize(Graphics *graphics)
{
	printf("new emeny");
	size = 0;
	_moveDelay = 2.0f;
	_moving = false;
	_move1StepTimer = 10.0f;
	_isAlive = false;	
	vertices = new Vector3[8]();
	colours = new Vector4[8]();

	SetVertex(0, /*pos*/-0.5f, 0.5f, 0.5f,  /*color*/ 255.0f, 0.0f, 255.0f, 1.0f);
	SetVertex(1, /*pos*/0.5f, 0.5f, 0.5f,   /*color*/ 255.0f, 109.0f, 255.0f, 1.0f);
	SetVertex(2, /*pos*/-0.5f, -0.5f, 0.5f, /*color*/ 255.0f, 0.0f, 255.0f, 1.0f);
	SetVertex(3, /*pos*/0.5f, -0.5f, 0.5f,  /*color*/ 255.0f, 109.0f, 255.0f, 1.0f);
	SetVertex(4, /*pos*/-0.5f, 0.5f, -0.5f, /*color*/ 255.0f, 109.0f, 255.0f, 1.0f);
	SetVertex(5, /*pos*/0.5f, 0.5f, -0.5f,  /*color*/ 255.0f, 0.0f, 255.0f, 1.0f);
	SetVertex(6, /*pos*/-0.5f, -0.5f, -0.5f,/*color*/ 255.0f, 109.0f, 255.0f, 1.0f);
	SetVertex(7, /*pos*/0.5f, -0.5f, -0.5f, /*color*/ 255.0f, 109.0f, 255.0f, 1.0f);

	indices = new unsigned int[30];

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	// front
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 1;
	indices[4] = 3;
	indices[5] = 2;

	// right
	indices[6] = 1;
	indices[7] = 5;
	indices[8] = 3;

	indices[9] = 5;
	indices[10] = 7;
	indices[11] = 3;

	// back
	indices[12] = 5;
	indices[13] = 4;
	indices[14] = 7;

	indices[15] = 4;
	indices[16] = 6;
	indices[17] = 7;

	// left
	indices[18] = 4;
	indices[19] = 0;
	indices[20] = 6;

	indices[21] = 6;
	indices[22] = 0;
	indices[23] = 2;

	// top
	indices[24] = 4;
	indices[25] = 5;
	indices[26] = 0;

	indices[27] = 5;
	indices[28] = 1;
	indices[29] = 0;

	// bottom
	indices[30] = 2;
	indices[31] = 3;
	indices[32] = 6;

	indices[33] = 3;
	indices[34] = 7;
	indices[35] = 6;
}

void Enemy::Update(float dt)
{
	if (_isAlive == true)
	{
		if (_transform.position.y < -5)
		{
			Reset();

		}
	}

	if (_isAlive == false)
	{
		_spawnDelay -= dt;
	}
	else
	{
		if (_moving==false)
		{
			_moveDelay -= dt;
		}
		else
		{
			Vector3 tempPos = _transform.position;

			int random = rand() % 2;
			if (random == 0)
			{
				_transform.position.x++;
			}
			else
			{
				_transform.position.z++;
			}
			_transform.position.y--;


			_moving = false;
			_moveDelay = 2.0f;
		}
		if (_moveDelay < 0.0f)
		{
			_moving = true;
		}
	}

	if (_spawnDelay < 0.0f)
	{
		_isAlive = true;
	}
}

void Enemy::Draw(Graphics *graphics, Matrix4x4 relativeTo, float dt)
{
	if (_isAlive == true){
		GLenum error = glGetError();

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_INDEX_ARRAY);

		glPushMatrix();
		glTranslatef(_transform.position.x, _transform.position.y, _transform.position.z);
		glRotatef(_transform.rotation.x, 1.0f, 0.0f, 0.0f);
		glRotatef(_transform.rotation.y, 0.0f, 1.0f, 0.0f);
		glRotatef(_transform.rotation.z, 0.0f, 0.0f, 1.0f);

		glScalef(_transform.scale.x, _transform.scale.y, _transform.scale.z);

		glVertexPointer(3, GL_FLOAT, 0, vertices);

		glColorPointer(4, GL_FLOAT, 0, colours);

		int indexCount = sizeof(indices) / sizeof(unsigned int);
		indexCount = 30;
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);

		glPopMatrix();

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_INDEX_ARRAY);
	}
	
}

void Enemy::SetVertex(int index, float x, float y, float z, float r, float g, float b, float a)
{

	vertices[index] = Vector3(x, y, z);

	colours[index].x = r;
	colours[index].y = g;
	colours[index].z = b;
	colours[index].w = a;
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

void Enemy::Reset()
{
	_isAlive = false;
	_moving = false;
	_moveDelay = 2.0f;
	_spawnDelay = 3.0f;
	int random = rand() % 2;
	if (random == 0)
	{
		_transform.position = Vector3(1, 0, 0);
	}
	else
	{
		_transform.position = Vector3(0, 0, 1);
	}
}