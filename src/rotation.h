#ifndef ROTATION_H
#define ROTATION_H

#include "point.h"

class Rotation {
  protected:
    Point coordinate;

  public:
    Rotation();
    Rotation(Point);
    Rotation(float, float, float);
    virtual void glRotate();
};

#endif
