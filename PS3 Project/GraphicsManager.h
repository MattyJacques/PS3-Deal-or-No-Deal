#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include <PSGL/psgl.h>
#include <PSGL/psglu.h>
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "GIFReader.h"
#include "HUD.h"


class GraphicsManager
{
private:
  static GraphicsManager *instance;   // Holds the instance of the graphics manager
  PSGLcontext *context;               // The PSGL context
  Light *theLight[2];                 // Holds array of lights 

  GraphicsManager();
  void InitialisePSGL();
  ~GraphicsManager();
  void CreateLight();
  void SetupDraw();
public:
  PSGLdevice* device;                 // The PSGL device
  Camera *theCamera;                  // The camera for the game
  HUD *pHUD;                          // The HUD of the game

  void RefreshLight(Camera *theCamera);
  static GraphicsManager* GetInstance();
  void CreateCamera();
  void DrawWorld();

};

#endif
