#ifndef MATERIAL_H
#define MATERIAL_H

// Include GLUT header based on operating system.
#ifdef __APPLE__
  #include <GLUT/glut.h>
#elif _WIN32
  #include <GL/glut.h>
#endif

#include "point.h"

class Material {
  private:
    float ambient[4] = {0.2, 0.2, 0.2, 1};
    float diffuse[4] = {0.8, 0.8, 0.8, 1};
    float specular[4] = {0, 0, 0, 1};
    float emission[4] = {0, 0, 0, 1};

  public:
    Material();
    Material(Point, Point, Point, Point);
    void draw();
};

#endif
