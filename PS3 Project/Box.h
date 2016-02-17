#ifndef BOX_H
#define BOX_H

#include "Gameboard.h"
#include "Vector.h"

class Box
{
public:
  float cashAmount;       // Holds the cashAmount for the box.
  int boxNumber;          // Holds which number the box is.
  bool isOpen;            // Holds whether the box is open or not.
  Vector position;        // Holds the position of the box for collision.

  Box();
};

#endif