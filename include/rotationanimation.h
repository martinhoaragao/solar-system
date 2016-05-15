#ifndef ROTATIONANIMATION_H
#define ROTATIONANIMATION_H

// Include GLUT header based on operating system.
#ifdef __APPLE__
  #include <GLUT/glut.h>
#elif _WIN32
  #include <GL/glut.h>
#endif

#include <math.h>
#include "rotation.h"

class RotationAnimation : public Rotation  {
  private:
    float time;

  public:
    RotationAnimation();
    RotationAnimation(float, Point);
    RotationAnimation(float, float, float, float);
    void glRotate();
};

#endif
