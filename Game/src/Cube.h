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
  void Draw(Graphics *graphics, float dt);

  void SetIsEnemy(bool isEnemy);

  bool GetIsEnemy();

  void SetIsTouched(bool isTouched);
  /**
  * \fn bool Player::GetIsTouched()
  * \brief Checks if cube was touched
  * \return boolean
  */
  bool GetIsTouched();

  Vector3 GetVertex();
protected:
  //void *_vertexPointer;

  Vector3 *vertices;
  Vector4 *colours;

  float r, g, b;

  int size;
  int offset;
  bool _isEnemy = false;
  bool _isTouched = false;
  //Vertex *vertices;
  unsigned int* indices;

  void SetVertex(int index, float x, float y, float z, float r, float g, float b, float a);
};