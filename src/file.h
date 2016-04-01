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

using namespace std;

class File {
  private:
    string fileName;
    int numberCoordinates;
    GLuint coordinatesID;


    void uploadData(vector<float>*);
    vector<float>* extractPoints();

  public:
    File(string);
    int coordinatesLength();
    GLuint getCoordinatesID();
    void reloadFile();
};

#endif
