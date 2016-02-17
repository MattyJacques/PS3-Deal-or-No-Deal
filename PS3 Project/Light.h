#ifndef LIGHT_H
#define LIGHT_H

class Light
{
private:
  float colour[4];        // Holds the colour of the light
  float ambColour[4];     // Holds the ambient colour of the light 
  float direction[3];     // Holds the direction of the light
  static int lightNum;    // Holds which light this object is

public:
  float position[4];      // Holds the position of the light

  Light(float posX, float posY, float  posZ, float local, 
        float colR, float colG, float colB);
  void setAmbient(float colR, float colG, float colB);
  void SetPosition();
};

#endif
