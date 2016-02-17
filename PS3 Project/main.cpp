#include <stdio.h>
#include <PSGL/psgl.h>
#include <PSGL/psglu.h>
#include <sys/sys_time.h>
#include "Model.h"
#include "Object.h"
#include "Controller.h"
#include "Camera.h"
#include "GraphicsManager.h"
#include "AssetManager.h"
#include "CollisionManager.h"


// Variables needed for frame count
float lastTime, nowTime;
int frameCounter = 0;

// Object for the controller
Controller theController; 

// Pointers for the managers of the game
GraphicsManager *pMyGM;
AssetManager *pMyAM;


int main()
{
	//the obligatory test
	printf("Hello, World!\n");

  // Start up the graphics and asset manager
  AssetManager::Start();
  pMyGM = GraphicsManager::GetInstance();
  pMyAM = AssetManager::GetInstance();
  
  // Creates the game camera
  pMyGM->CreateCamera();

  // Tells hud where the font is
  pMyGM->pHUD->UpdateFont(pMyAM->theFont);

  // Load in map data for set up
  pMyAM->theMap->DrawData();

  // Randomise the boxes
  pMyAM->GenBoxes(); 

  // Swap out buffers to clear screen
  psglSwap();

	// Infinite drawing loop
	while (theController.Poll()) 
	{
		//clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // Draws the game world
    pMyGM->DrawWorld();
  
    // Frame counter
    frameCounter++;
    if (pMyAM->theBoard->timer >= -100)
      pMyAM->theBoard->timer--;
    if (frameCounter > 2000) 
    {
	    frameCounter=0;
	    nowTime = (float)sys_time_get_system_time() * .000001f;
	    printf("fps = %f\n", (float)2000/(nowTime-lastTime));
	    lastTime=nowTime;
    }//if

		//swap buffers
	  psglSwap();
	}//while()

  // Terminate the AssetManager
  AssetManager::Terminate();

	//being tidy we dispose of the context and device - but this is never reached
  theController.Stop();
  exit(1);
	return 1;
}//main()




