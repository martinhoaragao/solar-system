#include "translationcatmull.h"
#include <iostream>

/*-----------------------------------------------------------------------------------
	API
-----------------------------------------------------------------------------------*/

TranslationCatmull::TranslationCatmull() : Translation() {
  time = 0;
  controlPoints = new vector<Point>();
}

TranslationCatmull::TranslationCatmull( float _time
                                      , vector<Point> *_controlPoints
                                      ) : Translation() {
  time = _time;
  controlPoints = _controlPoints;
}

void TranslationCatmull::glTranslate() {
  float now = glutGet(GLUT_ELAPSED_TIME);
  float timeMiliSeconds = time * 1000;
  float t = fmod(now, timeMiliSeconds) / timeMiliSeconds;


  //static float up[3]  = { 0.0, 1.0, 0.0 };
  float res[3];
  float dir[3];
  //float left[3];
  //float m[16];

  getGlobalCatmullRomPoint(t, res);
  getGlobalCatmullRomDirection(t, dir);
  renderCatmullRomCurve();

  glTranslatef(res[0], res[1], res[2]);

  //cross(up, dir, left);
  //cross(dir, left, up);
  //normalize(up);
  //normalize(left);
  //normalize(dir);
  //buildRotMatrix(left, up, dir, m);
  //glMultMatrixf(m);

}

/*-----------------------------------------------------------------------------------
	Helpers
-----------------------------------------------------------------------------------*/

void TranslationCatmull::buildRotMatrix(float *x, float *y, float *z, float *m) {
  m[0]  = x[0]; m[1]  = x[1];
  m[2]  = x[2]; m[3]  = 0;
  m[4]  = y[0]; m[5]  = y[1];
  m[6]  = y[2]; m[7]  = 0;
  m[8]  = z[0]; m[9]  = z[1];
  m[10] = z[2]; m[11] = 0;
  m[12] = 0;    m[13] = 0;
  m[14] = 0;    m[15] = 1;
}

void TranslationCatmull::cross(float *a, float *b, float *res) {
  res[0] = a[1]*b[2] - a[2]*b[1];
  res[1] = a[2]*b[0] - a[0]*b[2];
  res[2] = a[0]*b[1] - a[1]*b[0];
}

void TranslationCatmull::normalize(float *a) {
  float l = sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
  a[0] = a[0]/l;
  a[1] = a[1]/l;
  a[2] = a[2]/l;
}

void TranslationCatmull::getCatmullRomPoint(float t, int *indices, float *res) {
  // catmull-rom matrix
  float m[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
            { 1.0f, -2.5f,  2.0f, -0.5f},
            {-0.5f,  0.0f,  0.5f,  0.0f},
            { 0.0f,  1.0f,  0.0f,  0.0f}};

  res[0] = 0.0; res[1] = 0.0; res[2] = 0.0;

  float aux[4];

  for (int i = 0; i < 4; i++)
    aux[i] = pow(t, 3.0) * m[0][i] + pow(t, 2.0) * m[1][i] + t * m[2][i] + m[3][i];

  for (int i = 0; i < 3; i++) {
    res[i] = aux[0] * controlPoints->at(indices[0]).get(i) + aux[1] * controlPoints->at(indices[1]).get(i) + aux[2] * controlPoints->at(indices[2]).get(i) + aux[3] * controlPoints->at(indices[3]).get(i);
  }
}

// given  global t, returns the point in the curve
void TranslationCatmull::getGlobalCatmullRomPoint(float gt, float *res) {
  float t   = gt * controlPoints->size(); // this is the real global t
  int index = floor(t);         // which segment
  t         = t - index;        // where within  the segment

  // indices store the points
  int indices[4];
  indices[0] = (index + controlPoints->size()-1)%controlPoints->size();
  indices[1] = (indices[0]+1)%controlPoints->size();
  indices[2] = (indices[1]+1)%controlPoints->size();
  indices[3] = (indices[2]+1)%controlPoints->size();

  getCatmullRomPoint(t, indices, res);
}

void TranslationCatmull::getCatmullRomDirection(float t, int *indices, float *res) {
  // catmull-rom matrix
  float m[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
            { 1.0f, -2.5f,  2.0f, -0.5f},
            {-0.5f,  0.0f,  0.5f,  0.0f},
            { 0.0f,  1.0f,  0.0f,  0.0f}};

  res[0] = 0.0; res[1] = 0.0; res[2] = 0.0;

  float aux[4];

  for (int i = 0; i < 4; i++)
    aux[i] = (3 * pow(t, 2.0)) * m[0][i] + (2 * t) * m[1][i] + 1 * m[2][i];

  for (int i = 0; i < 3; i++)
    res[i] = aux[0] * controlPoints->at(indices[0]).get(i) + aux[1] * controlPoints->at(indices[1]).get(i) + aux[2] * controlPoints->at(indices[2]).get(i) + aux[3] * controlPoints->at(indices[3]).get(i);
}

// given  global t, returns the point in the curve
void TranslationCatmull::getGlobalCatmullRomDirection(float gt, float *res) {
  float t   = gt * controlPoints->size(); // this is the real global t
  int index = floor(t);         // which segment
  t         = t - index;        // where within  the segment

  // indices store the points
  int indices[4];
  indices[0] = (index + controlPoints->size()-1)%controlPoints->size();
  indices[1] = (indices[0]+1)%controlPoints->size();
  indices[2] = (indices[1]+1)%controlPoints->size();
  indices[3] = (indices[2]+1)%controlPoints->size();

  getCatmullRomDirection(t, indices, res);
}

void TranslationCatmull::renderCatmullRomCurve() {
  float res[3];
  float t = 0;

  glBegin(GL_LINE_LOOP);

  for (t = 0; t < 1; t += 0.0001) {
    getGlobalCatmullRomPoint(t, res);
    glVertex3f(res[0], res[1], res[2]);
  }

  glEnd();
}
