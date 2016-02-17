#ifndef CAMERA_H
#define CAMERA_H

#include "Vector.h"

class GraphicsManager;

class Camera
{
public:
  float myMatrix[16];      // Matrix to hold camera translations and rotations
  float zoomSpeed;         // Holds the float for z translation
  float strafeSpeed;       // Holds the float for x translation
  float xRot;              // Holds the data around the x axis
  float yRot;              // Holds the data around the y axis
  float zRot;              // Holds the data around the z axis
  float aspectRatio;       // Holds the aspect ratio of the camera
  float fieldOfView;       // Holds the field of view of the camera
  Vector myPosition;       // Holds the current position of the camera
  Vector tempPosition;     // Holds the temp position of the camera to check for collision
  GraphicsManager *pTheGraphicsManager;   // Holds a pointer to the graphics manager
  GLuint renderWidth;       // Holds the render width of PSGL
  GLuint renderHeight;      // Holds the render height of PSGL
  GLfloat aRatio;           // Holds the aspect ratio for PSGL

  Camera();
  void SetUpCamera();
  void UseCamera();
  void Reset();
  void Process();
  void Rotate();
  void ChangeLens();
  void RefreshLight();
  void ExtractPosition();
  void SetPosition(Vector newPos);
}; // class

#endif
