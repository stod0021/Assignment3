/**
* \class Cube.h
* \brief A class that represents the world block objects within the world of QBert.
* \author David Stoddard
* \date March 26, 2015
*/
#pragma once

#include <GameObject.h>

struct Vertex;

class Cube : public GameObject
{
public:
  Cube();

  Cube(Vector3 position);

  void Initialize(Graphics *graphics);
  void Update(float dt);
  void Draw(Graphics *graphics, Matrix4x4 relativeTo, float dt);

  void SetIsEnemy(bool isEnemy);

  bool GetIsEnemy();

  void SetIsTouched(bool isTouched);

  void Reset();

  /**
  * \fn bool Player::GetIsTouched()
  * \brief Checks if cube was touched
  * \return boolean
  */
  bool GetIsTouched();

  Vector3 GetVertex();
protected:
  Vector3 *vertices;
  Vector4 *colours;

  float r, g, b;

  int size;
  int offset;
  bool _isEnemy = false;
  bool _isTouched = false;
  unsigned int* indices;

  void SetVertex(int index, float x, float y, float z, float r, float g, float b, float a);
};