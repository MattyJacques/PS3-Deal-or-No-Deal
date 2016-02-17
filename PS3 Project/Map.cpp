#include "Map.h"
#include "MapReader.h"
#include "AssetManager.h"
#include "Box.h"
#include <PSGL/psgl.h>
#include <PSGL/psglu.h>

Map::Map(char* _filename)
{ // Sets the members of the class to default values and loads the map via mapreader
  MapReader theReader(_filename, this);
  firstPass = false;
} // Constructor()

void Map::DrawData()
{ // Draws the map onto the map world
  AssetManager* pMyAM = AssetManager::GetInstance();
  int boxNumber = 0;
  for(int x=0;x<64;x++)
  {
    for(int y=0;y<64;y++)
    {
      switch(mapArray[x][y])
      {
        case 'A': // If A is found, if first pass create a box object otherwise just draw box
          if(!firstPass)
          {
            pMyAM->theBoxes[boxNumber] = new Box();
            pMyAM->theBoxes[boxNumber]->position.xValue = y;
            pMyAM->theBoxes[boxNumber]->position.zValue = x;
          }
          glTranslatef(0, -0.15, 0);
          glScalef(0.5, 0.5, 0.5);
          if (!pMyAM->theBoxes[boxNumber]->isOpen) // If the box is open, don't draw
            pMyAM->DrawModel(MODEL_BOX_LID, TEX_RED);
          glScalef(2, 2, 2);
          glTranslatef(0, -0.5, 0);
          glScalef(0.5, 0.5, 0.5);
          if (!pMyAM->theBoxes[boxNumber]->isOpen)  // If the box is open, don't draw
            pMyAM->DrawModel(MODEL_BOX, pMyAM->boxTexArray[boxNumber]+28);
          glScalef(2, 2, 2);
          glTranslatef(0, -1.35, 0);
          pMyAM->DrawModel(MODEL_DIRECT_CUBE, TEX_TABLE);
          glTranslatef(0, 2, 0);

          boxNumber++;
          break;

        case 'B':
          pMyAM->DrawModel(MODEL_WALL, TEX_WALL); // If B found, draw wall
         break;

        case 'C':
          pMyAM->DrawModel(MODEL_WALL, TEX_WALL); // If C found, draw wall
          break;
      }
      glTranslatef(2,0,0);
    }
    glTranslatef(-128,0,2);
  }
  firstPass = true;
} // DrawData()

bool Map::IsWall(int index1, int index2)
{ // checks to see if the element given is a wall or empty space
  return (mapArray[index1][index2] != '0');
} // IsWall()