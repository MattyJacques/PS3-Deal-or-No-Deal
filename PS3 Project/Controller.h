#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <cell/pad.h>
#include "Object.h"
#include "Camera.h"
#include "GraphicsManager.h"

class Controller
{
private:
  bool gotController;                     // Bool to hold if we have the controller
  static bool initialised;                // Bool to hold if the controller is initialised
	CellPadInfo2 *padInfo;                  // Holds the info about the controller
	CellPadData *padData;                   // Holds data such as button presses
  CellPadActParam myParams;               // Holds the paramatersfor the rumble motors
  GraphicsManager *pGraphicsManager;      // Holds the pointer to the graphics manager

  void DPadPress();
  void SelectPress();
  void CrossPress();
  void CirclePress();
  void SquarePress();

public:
  bool Poll();
  Controller();
  void Stop();
};

#endif