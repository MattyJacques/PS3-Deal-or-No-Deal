#include <PSGL/psgl.h>
#include <PSGL/psglu.h>
#include "Object.h"

Object::Object()
{
  theModel = NULL;
  xPos = 0;
  yPos = 0;
  zPos = 0;
  yRot = 0;
  yRotSpeed = 0;
  xRot = 0;
  xRotSpeed = 0;
  zRotSpeed = 0;
} // Object ()



void Object::Update()
{
  xRot += xRotSpeed;
  yRot += yRotSpeed;
  zPos += zRotSpeed;

} // Update()



void Object::SetDefault()
{
  xPos = defaultX;
  yPos = defaultY;
  zPos = defaultZ;
  xRot = 0;
  yRot = 0;
  zRotSpeed = 0;
} // SetDefault()


//
//void Object::Draw()
//{
//  glLoadIdentity();
//  glTranslatef(xPos, yPos, (zPos += zRotSpeed));
//  glScalef(0.5, 0.5, 0.5);
//  glRotatef(yRot, 0, 1, 0);
//  glRotatef(xRot, 1, 0, 0);
//  theModel->Draw(pGraphicsManager->GetMaterial(10));
//} // Draw()
//
//void Object::DrawSelected()
//{
//  glLoadIdentity();
//  glTranslatef(xPos, yPos, zPos);
//  glScalef(0.7, 0.7, 0.7);
//  glRotatef(yRot, 0, 1, 0);
//  glRotatef(xRot, 1, 0, 0);
//  theModel->Draw(pGraphicsManager->GetMaterial(10));
//} // DrawSelected()

void Object::SetPosition(float x, float y, float z, Model *myModel)
{
  theModel = myModel;
  xPos = x;
  defaultX = x;
  yPos = y;
  defaultY = y;
  zPos = z;
  defaultZ = z;
} // SetPosition()
