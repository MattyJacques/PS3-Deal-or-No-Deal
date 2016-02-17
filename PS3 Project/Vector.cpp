#include <Math.h>
#include "Vector.h"

Vector Vector::AddVector(Vector vecA, Vector vecB)
{ // Add 2 vectors togethor returning a new one
  Vector result;

  result.xValue = vecA.xValue + vecB.xValue;
  result.yValue = vecA.yValue + vecB.yValue;
  result.zValue = vecA.zValue + vecB.zValue;

  return result;
} // AddVector()


Vector Vector::SubtractVector(Vector vecA, Vector vecB)
{ // Subtract 2 vectors returning a new one
  Vector result;

  result.xValue = vecA.xValue - vecB.xValue;
  result.yValue = vecA.yValue - vecB.yValue;
  result.zValue = vecA.zValue - vecB.zValue;

  return result;
} // SubtractVector()


Vector Vector::CrossProduct(Vector vecA, Vector vecB)
{ // Work out the cross product of the two vectors, returning a new one
  Vector result;

  result.xValue = (vecA.yValue * vecB.zValue) - (vecA.zValue * vecB.yValue);
  result.yValue = (vecA.zValue * vecB.xValue) - (vecA.xValue * vecB.zValue);
  result.zValue = (vecA.xValue * vecB.yValue) - (vecA.yValue * vecB.xValue);

  return result;
} // CrossProduct()

void Vector::Normalise()
{ // Make the vector into a unit 1 vector by diving each element by the length
  float length = sqrt((xValue * xValue) + (yValue * yValue) 
                      + (zValue * zValue));

  xValue = xValue / length;
  yValue = yValue / length;
  zValue = zValue / length;

} // Normalise()