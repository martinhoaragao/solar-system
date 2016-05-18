#ifndef TRANSLATIONCATMULL_H
#define TRANSLATIONCATMULL_H

// Include GLUT header based on operating system.
#ifdef __APPLE__
  #include <GLUT/glut.h>
#elif _WIN32
  #include <GL/glut.h>
#endif

#include <math.h>
#include <vector>
#include "translation.h"
#include "point.h"
#include "material.h"

using namespace std;

class TranslationCatmull : public Translation  {
  private:
    float time;
    GLuint coordinatesID;
    vector<Point> *controlPoints;
    int nControlPoints;

    void buildRotMatrix(float*, float*, float*, float*);
    void cross(float*, float*, float*);
    void normalize(float*);
    void getCatmullRomPoint(float, int*, float*);
    void getGlobalCatmullRomPoint(float, float*);
    void getCatmullRomDirection(float, int*, float*);
    void getGlobalCatmullRomDirection(float, float*);
    void loadCatmullRomCurve();
    void drawCatmullRomCurve();

    void prepareCilinder(float, float, int);


  public:
    TranslationCatmull();
    TranslationCatmull(float, vector<Point>*);
    void glTranslate();
};

#endif
