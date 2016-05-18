#ifndef FILE_H
#define FILE_H

// Include GLUT header based on operating system.
#ifdef __APPLE__
  #include <GLUT/glut.h>
#elif _WIN32
  #include <GL/glut.h>
#endif

#include <string>
#include <vector>

#include "material.h"

using namespace std;

class File {
  private:
    string fileName;
    Material material;
    int numberCoordinates[3];
    GLuint coordinatesID[2];

    void uploadData(vector<float>*, int);
    vector<float>* extractPointsSegment(ifstream&);

  public:
    File(string, Material);
    void loadFile();
    void draw();
};

#endif
