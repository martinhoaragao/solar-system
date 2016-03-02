#include "point.h"

Point::Point(float X, float Y, float Z) {
  x = X; y = Y; z = Z;
}

float Point::getX() { return x; }

float Point::getY() { return y; }

float Point::getZ() { return z; }

float* Point::toArray() {
  float* pointArray;

  pointArray[0] = x;
  pointArray[1] = y;
  pointArray[2] = z;

  return pointArray;
}
