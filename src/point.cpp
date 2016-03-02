#include "point.h"

point::point(float X, float Y, float Z) {
  x = X; y = Y; z = Z;
}

float point::getX() { return x; }

float point::getY() { return y; }

float point::getZ() { return z; }
