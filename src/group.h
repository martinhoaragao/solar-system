#ifndef GROUP_H
#define GROUP_H

// Include GLUT header based on operating system.
#ifdef __APPLE__
  #include <GLUT/glut.h>
#elif _WIN32
  #include <GL/glut.h>
#endif

#include <string>
#include <vector>
#include "tinyxml2.h"
#include "point.h"
#include "rotation.h"
#include "file.h"

using namespace std;

class Group {
  private:
    Point scale;
    Rotation rotation;
    Point translation;
    vector<File> files;
    vector<Group *> groups;

    vector<string> extractFileNames(char*);

  public:
    Group();
    Group(char*);

    void reload();
    void draw();


};

#endif
