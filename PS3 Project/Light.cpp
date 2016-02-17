#include <PSGL/psgl.h>
#include <PSGL/psglu.h>
#include "Light.h"

int Light::lightNum = 0;

Light::Light(float posX, float posY, float  posZ, float local, 
             float colR, float colG, float colB)
{ // Sets the members of the class to the default values and enables lighting in
  // PSGL for the game
  position[0] = posX;
  position[1] = posY;
  position[2] = posZ;
  position[3] = local;
  colour[0] = colR;
  colour[1] = colG;
  colour[2] = colB;
  colour[3] = 1;
  ambColour[0] = 0.4;
  ambColour[1] = 0.4;
  ambColour[2] = 0.4;
  ambColour[3] = 1.0;
  direction[0] = 0.0;
  direction[1] = 1.0;
  direction[2] = 0.0;

  


  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glLightfv(GL_LIGHT0+lightNum, GL_DIFFUSE, colour);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambColour);
  glLightfv(GL_LIGHT0+lightNum, GL_AMBIENT, ambColour);
} // Constructor()

void Light::setAmbient(float colR, float colG, float colB)
{ // Sets custom ambient light for the selected light
  ambColour[0] = colR;
  ambColour[1] = colG;
  ambColour[2] = colB;
  ambColour[3] = 1.0;  
} // setAmbient()

void Light::SetPosition()
{ // Sets a custom position for the light chosen
  glLightfv(GL_LIGHT1, GL_POSITION, position);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, colour);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambColour);
  glLightfv(GL_LIGHT1, GL_AMBIENT, ambColour);
} // SetPosition()