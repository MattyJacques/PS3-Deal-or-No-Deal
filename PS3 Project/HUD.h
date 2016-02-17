#ifndef HUD_H
#define HUD_H

#include "Font.h"
#include "AssetManager.h"
#include "Vector.h"

class HUD
{
private:
  Font* pFont;                  // Holds a pointer to the current font
  AssetManager* pMyAM;          // Holds the pointer to the asset manager
  Vector scaleVec;              // Holds the scale vector for the text on the HUD

public:
  HUD();
  void UpdateFont(Font* newFont);
  void StartHUD();
  void DisplayHUD();
  void StopHUD();
  void DisplayWin();
  void DisplayOffer();
  void DisplayControl();
};

#endif
