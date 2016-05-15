#include "file.h"
#include <sstream>
#include <fstream>
#include <iostream>

/*-----------------------------------------------------------------------------------
	API
-----------------------------------------------------------------------------------*/

File::File(string fn) {
  // Generate Vertex Buffer Objects

  glGenBuffers(2, coordinatesID);

  fileName = fn;
  loadFile();
}

void File::draw() {
  glBindBuffer(GL_ARRAY_BUFFER, coordinatesID[0]);
  //Draw Triangle from VBO - do each time window, view point or data changes
  glVertexPointer(3, GL_FLOAT, 0, NULL);

  glBindBuffer(GL_ARRAY_BUFFER, coordinatesID[1]);
  //Draw Triangle from VBO - do each time window, view point or data changes
  glNormalPointer(GL_FLOAT, 0, NULL);

  // Enable buffer
  //glEnableClientState(GL_VERTEX_ARRAY);

  glDrawArrays(GL_TRIANGLES, 0, numberCoordinates[0]/3);

  //Force display to be drawn now
  glFlush();
  glDisableClientState(GL_VERTEX_ARRAY);
}

/*-----------------------------------------------------------------------------------
	Helpers
-----------------------------------------------------------------------------------*/

void File::loadFile() {
  vector<float>* points;
  ifstream pointsFile;
  string line;
  float x, y, z;

  pointsFile.open(fileName);

  for(int i = 0; i < 2; i++) {
    points = extractPointsSegment(pointsFile);
    numberCoordinates[i] = points->size();
    uploadData(points, i);
  }

  pointsFile.close();
}

vector<float>* File::extractPointsSegment(ifstream& pointsFile) {
  vector<float>* points = new vector<float>();
  string line;
  float x, y, z;
  int lines;

  getline(pointsFile, line);
  istringstream iss(line);
  iss >> lines;

  for (int i = 0; i < lines; i ++) {
    getline(pointsFile, line);
    istringstream iss(line);

    iss >> x >> y >> z;
    cout << x << endl;

    points->push_back(x);
    points->push_back(y);
    points->push_back(z);
  }

  return points;
}

void File::uploadData(vector<float>* points, int i) {
  glBindBuffer(GL_ARRAY_BUFFER, coordinatesID[i]);
  glBufferData(GL_ARRAY_BUFFER, numberCoordinates[i] * sizeof(float), &points->front(), GL_STATIC_DRAW);
}
