#ifndef OBJECT_H
#define OBJECT_H

#include "Model.h"

class Object
{
public:
  Model *theModel;
  float xPos;
  float yPos;
  float zPos;
  float yRot;
  float yRotSpeed;
  float xRot;
  float xRotSpeed;
  float zRotSpeed;
  float defaultX;
  float defaultY;
  float defaultZ;

  Object();
  void Draw();
  void DrawSelected();
  void SetPosition(float x, float y, float z, Model *myModel);
  void Update();
  void SetDefault();
};

#endif
