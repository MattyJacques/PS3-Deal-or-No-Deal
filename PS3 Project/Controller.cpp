#include <PSGL/psgl.h>
#include <PSGL/psglu.h>
#include "Controller.h"

bool Controller::initialised = false;

Controller::Controller()
{ // Creates all the new objects for the class members
  // and then initialises the controller
  padInfo = new CellPadInfo2();
	padData = new CellPadData();
	gotController = false;
  pGraphicsManager = GraphicsManager::GetInstance();
  
  if (!initialised)
  {
    cellPadInit(1); 
    initialised = true;
  }

} // Constructor()


void Controller::Stop()
{ // Stops the controller for when it is no longer needed
  cellPadEnd();
  initialised = false;
} // Stop()


void Controller::DPadPress()
{ // Handles all the DPad presses for when the player can select the boxes via the DPAD
  if (padData->button[CELL_PAD_BTN_OFFSET_DIGITAL1]&CELL_PAD_CTRL_LEFT)
  {
    fprintf(stderr, "Left pressed\n");
    //pGraphicsManager->theCamera->myPosition.xValue-=1;
  }
  else if (padData->button[CELL_PAD_BTN_OFFSET_DIGITAL1]&CELL_PAD_CTRL_DOWN)
  {
    fprintf(stderr, "Down pressed\n");
    //pGraphicsManager->theCamera->myPosition.yValue-=1;
  }
  else if (padData->button[CELL_PAD_BTN_OFFSET_DIGITAL1]&CELL_PAD_CTRL_RIGHT)
  {
    fprintf(stderr, "Right pressed\n");
    //pGraphicsManager->theCamera->myPosition.xValue+=1;
  }
  else if (padData->button[CELL_PAD_BTN_OFFSET_DIGITAL1]&CELL_PAD_CTRL_UP)
  {
    fprintf(stderr, "Up pressed\n");
    //pGraphicsManager->theCamera->myPosition.yValue+=1;
  }

} // DPadPress()


void Controller::SelectPress()
{ // Resets the camera the inital camera position
  pGraphicsManager->theCamera->myPosition.xValue=-41;
  pGraphicsManager->theCamera->myPosition.zValue=-41;
  pGraphicsManager->theCamera->myPosition.yValue=0;
} // SelectPress()


void Controller::CrossPress()
{ // If cross is pressed, open the box we are standing next to.
  AssetManager::GetInstance()->theBoard->OpenBox();
  if (AssetManager::GetInstance()->theBoard->activeBox != 999)
  {
    myParams.motor[0] = 1;
    cellPadSetActDirect(0, &myParams);
  }
} // CrossPress()


void Controller::CirclePress()
{ // If circle is pressed, end the game if the correct circumstance arises
  AssetManager* pMyAM = AssetManager::GetInstance();
  if (pMyAM->theBoard->bankerOffer && !pMyAM->theBoard->endGame)
  {
    pMyAM->theBoard->bankerOffer = false;
  }
} // CirclePress()


void Controller::SquarePress()
{ // Used to allow the player to deal on the bankers offers if in the correct
  // sitation
  AssetManager* pMyAM = AssetManager::GetInstance();
  if (pMyAM->theBoard->bankerOffer && !pMyAM->theBoard->endGame)
  {
    pMyAM->theBoard->cashWon = pMyAM->theBoard->bankersOffer;
    pMyAM->theBoard->bankerAccepted = true;
    pMyAM->theBoard->endGame = true;
  }
} // SquarePress()


bool Controller::Poll()
{ // Handles all of the button presses and calls the associated function for that button
  bool result = true;
	cellPadGetInfo2(padInfo);

  if (padInfo->port_status[0]&CELL_PAD_STATUS_ASSIGN_CHANGES)
	{
		if (padInfo->port_status[0]&CELL_PAD_STATUS_CONNECTED)
		{
			gotController = true;
			printf("Acquired controller\n");
		}//if
		else
		{
			gotController = false;
			printf("Lost controller\n");
		}//else
	}//if

  if (gotController)
	{
		cellPadGetData(0, padData);
		if (true) // Checks for which button has been pressed and then calls the linked function
		{
			if (padData->button[CELL_PAD_BTN_OFFSET_DIGITAL1]&CELL_PAD_CTRL_START)
			{ // Start button
				printf ("Quit command received\n");
				result = false;
			}
      else if (padData->button[CELL_PAD_BTN_OFFSET_DIGITAL1]&CELL_PAD_CTRL_SELECT)
      { // Select button
        SelectPress();
      }
      else if (padData->button[CELL_PAD_BTN_OFFSET_DIGITAL2]&CELL_PAD_CTRL_SQUARE)
      { // Square button
        SquarePress();
      }
      else if (padData->button[CELL_PAD_BTN_OFFSET_DIGITAL2]&CELL_PAD_CTRL_CIRCLE)
      { // Circle button
        CirclePress();
      }
      else if (padData->button[CELL_PAD_BTN_OFFSET_DIGITAL2]&CELL_PAD_CTRL_CROSS)
      { // Triangle button
        CrossPress();
      }
      // Tracks the left Y to allow the player to move forward and back
      if ((padData->button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_Y]-0x007F) < -16 || (padData->button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_Y]-0x007F) > 16)
        pGraphicsManager->theCamera->zoomSpeed = ((float)padData->button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_Y]-0x007F)/300;
      else
        pGraphicsManager->theCamera->zoomSpeed = 0;
        
      // Tracks the left X to allow the player to move left and right
      if ((padData->button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_X]-0x007F) < -16 || (padData->button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_X]-0x007F) > 16)
        pGraphicsManager->theCamera->strafeSpeed = ((float)padData->button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_X]-0x007F)/550;
      else
        pGraphicsManager->theCamera->strafeSpeed = 0;

      // Tracks the right X to allow the player to look left and right
      if ((padData->button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X]-0x007F) < -16 || (padData->button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X]-0x007F) > 16)
        pGraphicsManager->theCamera->yRot += ((float)padData->button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X]-0x007F)/50;
      else
        pGraphicsManager->theCamera->yRot += 0;

      // Tracks the right Y to allow the player to look up and down
      if ((padData->button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_Y]-0x007F) < -16 || (padData->button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_Y]-0x007F) > 16)
        pGraphicsManager->theCamera->xRot += ((float)padData->button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_Y]-0x007F)/50;
      else
        pGraphicsManager->theCamera->xRot += 0;

      DPadPress(); // Called to check for DPad press
		}//if (len)
	}//if

	return result;
} // Poll()
