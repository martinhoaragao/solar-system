#include "file.h"
#include <sstream>
#include <fstream>
#include <iostream>

/*-----------------------------------------------------------------------------------
	API
-----------------------------------------------------------------------------------*/

File::File(string fn) {
  // Generate Vertex Buffer Objects
  glGenBuffers(1, &coordinatesID);

  fileName = fn;
  reloadFile();
}

int File::coordinatesLength() { return numberCoordinates; }

GLuint File::getCoordinatesID() { return coordinatesID; }


void File::reloadFile() {
  vector<float>* points = extractPoints();
  numberCoordinates = points->size();
  uploadData(points);
}

/*-----------------------------------------------------------------------------------
	Helpers
-----------------------------------------------------------------------------------*/

vector<float>* File::extractPoints() {
  vector<float>* points = new vector<float>();
  ifstream pointsFile;
  string line;
  float x, y, z;

  pointsFile.open(fileName);

  while (getline(pointsFile, line)) {
    istringstream iss(line);

    iss >> x >> y >> z;
    points->push_back(x);
    points->push_back(y);
    points->push_back(z);
  }

  pointsFile.close();
  return points;
}

void File::uploadData(vector<float>* points) {
  glBindBuffer(GL_ARRAY_BUFFER, coordinatesID);
  glBufferData(GL_ARRAY_BUFFER, numberCoordinates * sizeof(float), &points->front(), GL_STATIC_DRAW);
}
