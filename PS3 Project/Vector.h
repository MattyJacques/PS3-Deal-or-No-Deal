#ifndef VECTOR_H
#define VECTOR_H

class Vector
{
public:
  float xValue; // Holds the x value for the vector
  float yValue; // Holds the y value for the vector
  float zValue; // Holds the z value for the vector

  static Vector AddVector(Vector vecA, Vector vecB);
  static Vector SubtractVector(Vector vecA, Vector vecB);
  static Vector CrossProduct(Vector vecA, Vector vecB);
  void Normalise();
};

#endif