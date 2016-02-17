#include <sstream>
#include <string>
#include "HUD.h"

HUD::HUD()
{ // Sets the initial values for the HUD members
  pFont = NULL;
  scaleVec.xValue = 0.06;
  scaleVec.yValue = 0.06;
  scaleVec.zValue = 0.06;
} // Constructor()


void HUD::UpdateFont(Font* newFont)
{ // Updates the font to a new font
  pFont = newFont;
} // UpdateFont()


void HUD::StartHUD()
{ // Starts the HUD by setting the correct draw settings
  pMyAM = AssetManager::GetInstance();
  //Turn into 2D
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glDisable(GL_LIGHTING);
  glClear(GL_DEPTH_BUFFER_BIT);
} // StartHUD()


void HUD::DisplayHUD()
{ // Displays boxes left the screen so the player knows how many are left
  glScalef(scaleVec.xValue, scaleVec.yValue, 0.06);
  glTranslatef(-16, 15.5, -0.1);
  pFont->UseChar('B');
  pMyAM->DrawModel(MODEL_HUD, 999);
  glTranslatef(0.77, 0, 0);
  pFont->UseChar('o');
  pMyAM->DrawModel(MODEL_HUD, 999);
  glTranslatef(0.77, 0, 0);
  pFont->UseChar('x');
  pMyAM->DrawModel(MODEL_HUD, 999);
  glTranslatef(0.77, 0, 0);
  pFont->UseChar('e');
  pMyAM->DrawModel(MODEL_HUD, 999);
  glTranslatef(0.77, 0, 0);
  pFont->UseChar('s');
  pMyAM->DrawModel(MODEL_HUD, 999);
  glTranslatef(0.77, 0, 0);
  pFont->UseChar(' ');
  pMyAM->DrawModel(MODEL_HUD, 999);
  glTranslatef(0.77, 0, 0);
  pFont->UseChar('l');
  pMyAM->DrawModel(MODEL_HUD, 999);
  glTranslatef(0.77, 0, 0);
  pFont->UseChar('e');
  pMyAM->DrawModel(MODEL_HUD, 999);
  glTranslatef(0.77, 0, 0);
  pFont->UseChar('f');
  pMyAM->DrawModel(MODEL_HUD, 999);
  glTranslatef(0.77, 0, 0);
  pFont->UseChar('t');
  pMyAM->DrawModel(MODEL_HUD, 999);
  glTranslatef(0.77, 0, 0);
  pFont->UseChar(':');
  pMyAM->DrawModel(MODEL_HUD, 999);
  glTranslatef(0.77, 0, 0);
  pFont->UseChar(' ');
  pMyAM->DrawModel(MODEL_HUD, 999);
  glTranslatef(0.77, 0, 0);

  std::ostringstream intStr; // Draws the dynamic data to the screen for the player
  int openBoxes = 22 - pMyAM->theBoard->boxesOpen;
  if (openBoxes <= 0)
    openBoxes = 22;
  intStr << openBoxes;
  std::string str = intStr.str();
  for (int i = 0; i < str.length(); i++)
  {
    pFont->UseChar(str[i]);
    pMyAM->DrawModel(MODEL_HUD, 999);
    glTranslatef(0.77, 0, 0);
  }
}


void HUD::StopHUD()
{ // Stops the hud by turning draw settings back to 3D
  //Turn 3D back on
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_LIGHTING);
} // StopHUD()

void HUD::DisplayWin()
{ // Display the win screen to the player. Letting them know how much they won
  // and how much they had in their box.
  glScalef(scaleVec.xValue, scaleVec.yValue, 0.06);
  glTranslatef(-6, 0.5, -0.1);

  std::string outputString = "You won: ";
  for (int i = 0; i < outputString.length(); i++)
  {
    pFont->UseChar(outputString[i]);
    pMyAM->DrawModel(MODEL_HUD, 999);
    glTranslatef(0.77, 0, 0);
  }

  std::ostringstream intStr;
  intStr << pMyAM->theBoard->cashWon;
  outputString = intStr.str();
  for (int i = 0; i < outputString.length(); i++)
  {
    pFont->UseChar(outputString[i]);
    pMyAM->DrawModel(MODEL_HUD, 999);
    glTranslatef(0.77, 0, 0.01);
  }
  glTranslatef(-1, 0, 0);
  if (pMyAM->theBoard->bankerAccepted)
  {
    glTranslatef(-13, -5, 0);
    outputString = "Your box contained: ";
    for (int i = 0; i < outputString.length(); i++)
    {
      pFont->UseChar(outputString[i]);
      pMyAM->DrawModel(MODEL_HUD, 999);
      glTranslatef(0.77, 0, 0.01);
    }
    std::ostringstream outStr;
    outStr << pMyAM->theBoard->playerCash;
    outputString = outStr.str();
    for (int i = 0; i < outputString.length(); i++)
    {
      pFont->UseChar(outputString[i]);
      pMyAM->DrawModel(MODEL_HUD, 999);
      glTranslatef(0.77, 0, 0.01);
    }
  }
} // DisplayWin()

void HUD::DisplayOffer()
{ // Display the banker offer to the screen so the player can decide whether
  // to take it or leave it and continue playing
  glScalef(scaleVec.xValue, scaleVec.yValue, 0.06);
  glTranslatef(-8, 0.5, -0.1);

  std::string outputString = "Bankers offer: ";
  for (int i = 0; i < outputString.length(); i++)
  {
    pFont->UseChar(outputString[i]);
    pMyAM->DrawModel(MODEL_HUD, 999);
    glTranslatef(0.77, 0, 0);
  }

  std::ostringstream intStr;
  intStr << (int)pMyAM->theBoard->bankersOffer;
  outputString = intStr.str();
  for (int i = 0; i < outputString.length(); i++)
  {
    pFont->UseChar(outputString[i]);
    pMyAM->DrawModel(MODEL_HUD, 999);
    glTranslatef(0.77, 0, 0);
  }

  glTranslatef(-17, -5, 0);
  outputString = "[] = DEAL     O = NO DEAL"; // Displays the controls for this screen
  for (int i = 0; i < outputString.length(); i++)
  {
    pFont->UseChar(outputString[i]);
    pMyAM->DrawModel(MODEL_HUD, 999);
    glTranslatef(0.77, 0, 0);
  }
} // DisplayOffer()

void HUD::DisplayControl()
{ // Displays how to activate the box to the screen
  std::string outputString = "Press X to activate";
  glScalef(scaleVec.xValue, scaleVec.yValue, 0.06);
  glTranslatef(-6, -8, 0);
  for (int i = 0; i < outputString.length(); i++)
  {
    pFont->UseChar(outputString[i]);
    pMyAM->DrawModel(MODEL_HUD, 999);
    glTranslatef(0.77, 0, 0);
  }
} // DisplayControl()