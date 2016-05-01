#ifndef TRANSLATIONSIMPLE_H
#define TRANSLATIONSIMPLE_H

// Include GLUT header based on operating system.
#ifdef __APPLE__
  #include <GLUT/glut.h>
#elif _WIN32
  #include <GL/glut.h>
#endif

#include "translation.h"

class TranslationSimple : public Translation  {
  private:
    Point point;

  public:
    TranslationSimple();
    TranslationSimple(Point);
    TranslationSimple(float, float, float);
    void glTranslate();
};

#endif
