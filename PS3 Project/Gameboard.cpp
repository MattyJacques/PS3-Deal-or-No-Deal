#include <PSGL/psgl.h>
#include <PSGL/psglu.h>
#include "Gameboard.h"
#include "AssetManager.h"
#include "CollisionManager.h"


float Gameboard::cashPrizes[22] =
{
  0.01, 0.10, 0.50, 1.00, 5.00, 10.00, 50.00,
  100.00, 250.00, 500.00, 750.00, 1000.00, 3000.00, 5000.00,
  10000.00, 15000.00, 20000.00, 35000.00, 50000.00, 75000.00,
  100000.00, 250000.00
};

Gameboard::Gameboard()
{ // Sets all the class memebers to their default values
  boxesOpen = 0;
  bankerOffer = false;
  bankerAccepted = false;
  endGame = false;
  bankersOffer = 0;
  timer = 0;
  cashWon = 0;
  for (int i = 0; i <= 23; i++)
  { // Links all the gameboard notices to a cash prize
    theNotices[i] = new CashNotice();
    theNotices[i]->cashAmount = cashPrizes[i];
  }
} // Constructor()


void Gameboard::DrawBoard()
{ // Draws the gameboard into the world
  int texCount = 4;
  AssetManager* pMyAM = AssetManager::GetInstance();
  glTranslatef(6, 0, 10);
  glRotatef(45, 0, 1, 0);
  // Draws the left side of the game board
  pMyAM->DrawModel(MODEL_BOARD_SIDE, TEX_WOOD);
  glTranslatef(1.24, 2.5, -0.01);

  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 11; j++)
    {
      if (!theNotices[texCount-4]->isOpened)
      { // If the notice is opened draw black instead of the cash amount
        pMyAM->DrawModel(MODEL_CASH_NOTICE, texCount);
      }
      else
      {
        pMyAM->DrawModel(MODEL_CASH_NOTICE, TEX_BLACK);
      }
      glTranslatef(0, -0.5, 0);
      texCount++;
    }
    glTranslatef(2, 5.5, 0);
    if(texCount <= 17)
    {
      for (int k = 0; k < 11; k++)
      { // Draws the right side of the notices
        pMyAM->DrawModel(MODEL_CASH_NOTICE, TEX_BLACK);
        glTranslatef(0, -0.5, 0);
      }
      glTranslatef(2, 5.5, 0);
    }
  }
  // Draws the right side, top and bottom of the game board
  glTranslatef(-0.75, -2.5, 0);
  pMyAM->DrawModel(MODEL_BOARD_SIDE, TEX_WOOD);
  glTranslatef(-3.25, -3, 0);
  pMyAM->DrawModel(MODEL_BOARD_BOTTOM, TEX_WOOD);
  glTranslatef(0, 6, 0);
  pMyAM->DrawModel(MODEL_BOARD_BOTTOM, TEX_WOOD);
} // DrawBoard()


void Gameboard::CheckBox()
{ // Checks to see if the player is next to a box, if so display activator
  activeBox = CollisionManager::GetInstance()->CheckBoxCollision();
  if (activeBox != 999)
  {
    GraphicsManager* pMyGM = GraphicsManager::GetInstance();
    pMyGM->pHUD->StartHUD();
    pMyGM->pHUD->DisplayControl();
    pMyGM->pHUD->StopHUD();
  } 
} // CheckBox()


void Gameboard::OpenBox()
{ // Opens up a game box, checks if the box has already been opened and sets the notice
  // of that cash amount to open so it does not draw on the next frame
  if (timer <= 0)
  {
    AssetManager* pMyAM = AssetManager::GetInstance();
    if (activeBox != 999 && !pMyAM->theBoxes[activeBox]->isOpen)
    {
      bool foundNotice = false;
      int count = 0;
      pMyAM->theBoxes[activeBox]->isOpen = true;

      for (int i = 0; i < 22; i++)
      { // Find the notice relating to the box opened
        if (theNotices[i]->cashAmount == pMyAM->theBoxes[activeBox]->cashAmount)
        {
          theNotices[i]->isOpened = true;
          i += 22;
        }
      }

      if (boxesOpen > 25)
        boxesOpen = 1;
      else              // Increment the amount of boxes open
        boxesOpen++;

      if (boxesOpen == 5 || boxesOpen == 8 || boxesOpen == 11 || boxesOpen == 14 || 
          boxesOpen == 17 || boxesOpen == 20 || boxesOpen == 21)
      { // Checks to see if a banker offer should be make or we have run out of boxes
        // to open.
        if (boxesOpen == 21)
        {
          endGame = true;
        }
        bankerOffer = true;
        BankOffer();
      }
    }
    timer = 50; // Reset timer so player can not spam open
  }
} // OpenBox()

void Gameboard::BankOffer()
{ // Generates the banker offer to be made to the player
  AssetManager* pMyAM = AssetManager::GetInstance();
  float tempOffer = 0;
  int temp = 0;
  for (int i = 0; i < 21; i++)
  {

    if (!pMyAM->theBoxes[i]->isOpen) // Gets all the unopened box amounts
      tempOffer += pMyAM->theBoxes[i]->cashAmount;
  }

  temp = 22 - boxesOpen; // find out how many boxes are still closed
  tempOffer /= temp; // average it to find out what the offer should be 
  bankersOffer = int(tempOffer); // Set the banker offer to the offer calculated
} // BankOffer()


