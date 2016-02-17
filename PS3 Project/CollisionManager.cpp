#include "CollisionManager.h"
#include <stdlib.h>

CollisionManager* CollisionManager::instance = NULL;

CollisionManager::CollisionManager()
{ // Constructor to keep the compiler happy

} // Constructor()

CollisionManager* CollisionManager::GetInstance()
{ // Returns the instance of the manager. If no current instance,
  // makes a new one and then returns that
  if(!instance) 
  {
    instance = new CollisionManager();
  }

  return instance;
} // GetInstance()

bool CollisionManager::CheckCollision()
{ // Works out which element of the array the current camera location is in
  // Then calls IsWall to see if the element we are trying to move into is a wall
  // as collision detection.

  bool result = false;
  pMyGM = GraphicsManager::GetInstance();
  pMyAM = AssetManager::GetInstance();

  
  int i = (int(pMyGM->theCamera->tempPosition.zValue-1)/2);
  int j = (int(pMyGM->theCamera->tempPosition.xValue-1)/2);

  //fprintf(stderr, "Player at: %i, %i\n", abs(i), abs(j));
  if (pMyAM->theMap->IsWall(std::abs(i),std::abs(j)))
    result = true;


  return result;
} // CheckCollision()

int CollisionManager::CheckBoxCollision()
{ // Checks to see if the player is colliding with a box to see if the player
  // is close enough to be able to activate a box.
  int boxNum = 999;
  int y = (int(pMyGM->theCamera->tempPosition.zValue-1)/2);
  int x = (int(pMyGM->theCamera->tempPosition.xValue-1)/2);
  
  for (int i = 0; i < 21; i++)
  { // Loops through all the boxes to see if we are in a colliding location
    if (std::abs(x) == pMyAM->theBoxes[i]->position.xValue+1 && std::abs(y) == pMyAM->theBoxes[i]->position.zValue)
    {
      boxNum = i;
      fprintf(stderr, "%i\n", boxNum);
    }
    else if (std::abs(x) == pMyAM->theBoxes[i]->position.xValue && std::abs(y) == pMyAM->theBoxes[i]->position.zValue+1)
    {
      boxNum = i;
      fprintf(stderr, "%i\n", boxNum);
    }
  }

  return boxNum;
} // CheckBoxCollision()