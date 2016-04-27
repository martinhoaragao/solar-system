#ifndef ROTATIONANIMATION_H
#define ROTATIONANIMATION_H

// Include GLUT header based on operating system.
#ifdef __APPLE__
  #include <GLUT/glut.h>
#elif _WIN32
  #include <GL/glut.h>
#endif

#include "rotation.h"
#include <math.h>


class RotationAnimation : public Rotation  {
  private:
    float time;
    Point coordinate;

  public:
    RotationAnimation();
    RotationAnimation(float, Point);
    RotationAnimation(float, float, float, float);
    void glRotate();
};

#endif
