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
    Point ambient;
    Point diffuse;
    Point specular;
    Point emission;

  public:
    Material();
    Material(Point, Point, Point, Point);
    void draw();
};

#endif
