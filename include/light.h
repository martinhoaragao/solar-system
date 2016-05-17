#ifndef LIGHT_H
#define LIGHT_H

// Include GLUT header based on operating system.
#ifdef __APPLE__
  #include <GLUT/glut.h>
#elif _WIN32
  #include <GL/glut.h>
#endif

#include "point.h"

class Light {
  private:
    Point position;
    bool isPoint;

  public:
    Light(Point, bool);
    void draw();
};

#endif
