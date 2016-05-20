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
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include "material.h"

using namespace std;

class File {
  private:
    string fileName;
    Material material;
    int numberCoordinates[3];
    GLuint coordinatesID[3];
    GLuint textureID;

    void loadTexture(string);
    void loadFile();
    vector<float>* extractPointsSegment(ifstream&, int);
    void uploadData(vector<float>*, int);

  public:
    File(string, Material);
    File(string, Material, string);
    void draw();
};

#endif
