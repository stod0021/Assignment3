#include "Cube.h"
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

Cube::Cube()
{
  _transform.position = Vector3::Zero();
  _transform.rotation = Vector3::Zero();
  _transform.scale = Vector3::One();
}

Cube::Cube(Vector3 position)
{
  _transform.position = position;
  _transform.rotation = Vector3::Zero();
  _transform.scale = Vector3::One();
}

void Cube::Initialize(Graphics *graphics)
{
	//srand(time(NULL));

  r = (float)(rand() % 255);
  g = (float)(rand() % 255);
  b = (float)(rand() % 255);

  vertices = new Vector3[8]();
  colours = new Vector4[8]();

  if (_isEnemy == false){
	  SetVertex(0, /*pos*/-0.5f, 0.5f, 0.5f,  /*color*/ r, 0.0f, b, 1.0f);
	  SetVertex(1, /*pos*/0.5f, 0.5f, 0.5f,   /*color*/ 0.0f, g, b, 1.0f);
	  SetVertex(2, /*pos*/-0.5f, -0.5f, 0.5f, /*color*/ r, g, 0.0f, 1.0f);
	  SetVertex(3, /*pos*/0.5f, -0.5f, 0.5f,  /*color*/ r, g, 0.0f, 1.0f);
	  SetVertex(4, /*pos*/-0.5f, 0.5f, -0.5f, /*color*/ r, g, b, 1.0f); //top vertex
	  SetVertex(5, /*pos*/0.5f, 0.5f, -0.5f,  /*color*/ r, 0.0f, b, 1.0f);
	  SetVertex(6, /*pos*/-0.5f, -0.5f, -0.5f,/*color*/ r, g, b, 1.0f);
	  SetVertex(7, /*pos*/0.5f, -0.5f, -0.5f, /*color*/ 0.0f, g, b, 1.0f);
	  _isEnemy = true;
  }
  else if (_isEnemy == true){

	  SetVertex(0, /*pos*/-0.5f, 0.5f, 0.5f,  /*color*/ 1.0f, 0.0f, 0.0f, 1.0f);
	  SetVertex(1, /*pos*/0.5f, 0.5f, 0.5f,   /*color*/ 1.0f, 0.0f, 0.0f, 1.0f);
	  SetVertex(2, /*pos*/-0.5f, -0.5f, 0.5f, /*color*/ 1.0f, 0.0f, 0.0f, 1.0f);
	  SetVertex(3, /*pos*/0.5f, -0.5f, 0.5f,  /*color*/ 1.0f, 0.0f, 0.0f, 1.0f);
	  SetVertex(4, /*pos*/-0.5f, 0.5f, -0.5f, /*color*/ 1.0f, 0.0f, 0.0f, 1.0f); //top vertex
	  SetVertex(5, /*pos*/0.5f, 0.5f, -0.5f,  /*color*/ 1.0f, 0.0f, 0.0f, 1.0f);
	  SetVertex(6, /*pos*/-0.5f, -0.5f, -0.5f,/*color*/ 1.0f, 0.0f, 0.0f, 1.0f);
	  SetVertex(7, /*pos*/0.5f, -0.5f, -0.5f, /*color*/ 1.0f, 0.0f, 0.0f, 1.0f);
  }
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

void Cube::Update(float dt)
{
	if (_isTouched == true)
	{
		SetVertex(0, /*pos*/-0.5f, 0.5f, 0.5f,  /*color*/ 1.0f, 0.0f, 0.0f, 1.0f);
		SetVertex(1, /*pos*/0.5f, 0.5f, 0.5f,   /*color*/ 0.0f, 1.0f, 1.0f, 1.0f);
		SetVertex(2, /*pos*/-0.5f, -0.5f, 0.5f, /*color*/  1.0f, 1.0f, 0.0f, 1.0f);
		SetVertex(3, /*pos*/0.5f, -0.5f, 0.5f,  /*color*/  1.0f, 1.0f, 0.0f, 1.0f);
		SetVertex(4, /*pos*/-0.5f, 0.5f, -0.5f, /*color*/  1.0f, 1.0f, 1.0f, 1.0f);
		SetVertex(5, /*pos*/0.5f, 0.5f, -0.5f,  /*color*/  1.0f, 1.0f, 1.0f, 1.0f);
		SetVertex(6, /*pos*/-0.5f, -0.5f, -0.5f,/*color*/  1.0f, 1.0f, 1.0f, 1.0f);
		SetVertex(7, /*pos*/0.5f, -0.5f, -0.5f, /*color*/ 0.0f, 1.0f, 1.0f, 1.0f);
	}
}

void Cube::Draw(Graphics *graphics, Matrix4x4 relativeTo, float dt)
{ 
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
  //glVertexPointer(3, GL_FLOAT, sizeof(Vertex), vertices);

  glColorPointer(4, GL_FLOAT, 0, colours);
  //glColorPointer(4, GL_FLOAT, sizeof(Vertex), vertices + sizeof(Vector3));

  int indexCount = sizeof(indices) / sizeof(unsigned int);
  indexCount = 30;
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);

  glPopMatrix();

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_INDEX_ARRAY);
}

void Cube::SetVertex(int index, float x, float y, float z, float r, float g, float b, float a)
{

  vertices[index] = Vector3(x, y, z);

  colours[index].x = r;
  colours[index].y = g;
  colours[index].z = b;
  colours[index].w = a;
}

void Cube::SetIsEnemy(bool isEnemy){
	_isEnemy = isEnemy;
}

bool Cube::GetIsEnemy(){
	return _isEnemy;
}
void Cube::SetIsTouched(bool isTouched)
{
	_isTouched = isTouched;
}

bool Cube::GetIsTouched()
{
	return _isTouched;
}

void Cube::Reset()
{
	//_isTouched = false;
	//_touchedSet = false;

	//r = ((float)rand() / (RAND_MAX));

	SetVertex(0, /*pos*/-0.5f, 0.5f, 0.5f,  /*color*/ 1.0f, 0, 0, 1.0f);
	SetVertex(1, /*pos*/0.5f, 0.5f, 0.5f,   /*color*/ 1.0f, 0, 0, 1.0f);
	SetVertex(2, /*pos*/-0.5f, -0.5f, 0.5f, /*color*/ 1.0f, 0, 0, 1.0f);
	SetVertex(3, /*pos*/0.5f, -0.5f, 0.5f,  /*color*/ 1.0f, 0, 0, 1.0f);
	SetVertex(4, /*pos*/-0.5f, 0.5f, -0.5f, /*color*/ 0, 0, 1.0f, 1.0f);
	SetVertex(5, /*pos*/0.5f, 0.5f, -0.5f,  /*color*/ 0, 0, 1.0f, 1.0f);
	SetVertex(6, /*pos*/-0.5f, -0.5f, -0.5f,/*color*/ 0, 0, 1.0f, 1.0f);
	SetVertex(7, /*pos*/0.5f, -0.5f, -0.5f, /*color*/ 0, 0, 1.0f, 1.0f);
}