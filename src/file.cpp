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

void File::draw() {
  glBindBuffer(GL_ARRAY_BUFFER, coordinatesID);

  //Draw Triangle from VBO - do each time window, view point or data changes
  glVertexPointer(3, GL_FLOAT, 0, NULL);

  // Enable buffer
  glEnableClientState(GL_VERTEX_ARRAY);

  glDrawArrays(GL_TRIANGLES, 0, numberCoordinates/3);

  //Force display to be drawn now
  glFlush();
  glDisableClientState(GL_VERTEX_ARRAY);
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
