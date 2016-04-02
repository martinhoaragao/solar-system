#ifndef ROTATION_H
#define ROTATION_H

#include "point.h"

class Rotation {
  private:
    float angle;
    Point coordinate;

  public:
    Rotation();
    Rotation(float, Point);
    Rotation(float, float, float, float);
    float getAngle();
    float getX();
    float getY();
    float getZ();
};

#endif
