#include <PSGL/psgl.h>
#include <PSGL/psglu.h>
#include <cell/cell_fs.h>
#include <cell/sysmodule.h>
#include <Math.h>
#include "Camera.h"
#include "GraphicsManager.h"
#include "CollisionManager.h"


Camera::Camera()
{ // Sets up member variables to their default values
  // also sets the camera to the starting position and rotation
  SetUpCamera();
  Reset();
  xRot = 0;
  yRot = -45;
  zRot = 0;
  zoomSpeed = 0;
  strafeSpeed = 0;
  myPosition.xValue = 0;
  myPosition.yValue = 0;
  myPosition.zValue = 0;
  tempPosition.xValue = 0;
  tempPosition.yValue = 0;
  tempPosition.zValue = 0;

  Vector newPos;
  newPos.xValue = -41;
  newPos.yValue = 0;
  newPos.zValue = -41;
  SetPosition(newPos);
} // Constructor()


void Camera::SetUpCamera()
{ // Sets up the initial values for the camera using PSGL from the graphics manager
  // and using the identity matrix
  pTheGraphicsManager = GraphicsManager::GetInstance();
  psglGetRenderBufferDimensions(pTheGraphicsManager->device, &renderWidth, &renderHeight);
	glViewport(0, 0, renderWidth, renderHeight);
  aRatio = psglGetDeviceAspectRatio(pTheGraphicsManager->device);  
	aspectRatio = aRatio;
  fieldOfView = 30;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspectivef(30, aspectRatio, 0.1, 500);
} // SetUpCamera()


void Camera::UseCamera()
{ // Allows the use of the camera position to allow for models to be drawn
  // around it.
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
  Rotate();
  glLoadIdentity();
  glRotatef(xRot, 1, 0, 0);
  glRotatef(yRot, 0, 1, 0);
  glRotatef(zRot, 0, 0, 1);
  glTranslatef(myPosition.xValue,myPosition.yValue,myPosition.zValue);
	glPushMatrix();
}// UseCamera()


void Camera::Reset()
{ // Resets the camera to the default position using the identity matrix.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, myMatrix);
  ExtractPosition();
  pTheGraphicsManager->RefreshLight(this);
}// Reset()


void Camera::Rotate()
{ // Does all the snazzy maths to allow for free camera rotation and collision
  // detection
  tempPosition = myPosition;
  tempPosition.zValue += (float)sin((yRot+90)*(M_PI/180))*-zoomSpeed;
  tempPosition.xValue += (float)cos((yRot+90)*(M_PI/180))*-zoomSpeed;

  tempPosition.zValue += (float)sin((yRot)*(M_PI/180))*-strafeSpeed;
  tempPosition.xValue += (float)cos((yRot)*(M_PI/180))*-strafeSpeed;
  
  if (!CollisionManager::GetInstance()->CheckCollision())
  { // If no collision, allow the movement requested
    myPosition = tempPosition;
  }
} // Rotate()


void Camera::Process()
{ // Retrieves the camera matrix and then refreshes the lights so they have a 
  // fixed location
  UseCamera();
  pTheGraphicsManager->RefreshLight(this);
}// Process()


void Camera::ExtractPosition()
{ // Extracts the position from the camera matrix into a vector.
	myPosition.xValue = myMatrix[12];
	myPosition.yValue = myMatrix[13];
	myPosition.zValue = myMatrix[14];
} // ExtractPosition()

void Camera::SetPosition(Vector newPos)
{ // Sets a new camera position using the vector provided.
  myPosition.xValue = newPos.xValue;
  myPosition.yValue = newPos.yValue;
  myPosition.zValue = newPos.zValue;
} // SetPosition()

