#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "AssetManager.h"
#include "GraphicsManager.h"

class CollisionManager
{
private:
  static CollisionManager *instance;    // Pointer to the single instance of the manager
  GraphicsManager *pMyGM;               // Pointer to the graphics manager
  AssetManager *pMyAM;                  // Pointer to the asset manager
  CollisionManager();

public:
  static CollisionManager* GetInstance();
  bool CheckCollision();
  int CheckBoxCollision();
};

#endif
