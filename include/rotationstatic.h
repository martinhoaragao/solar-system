#ifndef ROTATIONSTATIC_H
#define ROTATIONSTATIC_H

// Include GLUT header based on operating system.
#ifdef __APPLE__
  #include <GLUT/glut.h>
#elif _WIN32
  #include <GL/glut.h>
#endif

#include "rotation.h"

class RotationStatic : public Rotation  {
  private:
    float angle;

  public:
    RotationStatic();
    RotationStatic(float, Point);
    RotationStatic(float, float, float, float);
    void glRotate();
};

#endif
