#include <GLUT/glut.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>

#include "tinyxml2.h"
#include "point.h"

using namespace std;

void drawTriangle(vector<Point> points) {
  glBegin(GL_TRIANGLES);
  for(int i = 0; i < 3; i++){
    Point point = points.back();
    glVertex3fv(point.toArray());
    points.pop_back();
  }
  glEnd();
}

void drawTriangles(vector<Point> points) {
  for(int i = 0; i < points.size(); i+=3)
  drawTriangle(points);
}

vector<Point> extractPoints(string fileName) {
  vector<Point> points;
  ifstream pointsFile;
  string line;
  float x, y, z;

  pointsFile.open(fileName);

  getline(pointsFile, line);

  while (getline(pointsFile, line)) {
    istringstream iss(line);

    if (!(iss >> x >> y >> z)) {
      break;
    } // error

    points.push_back(Point(x, y, z));
  }

  pointsFile.close();
  return points;
}

vector<string> extractFileNames (char* configFileName) {
  vector<string> fileNames;
  tinyxml2::XMLDocument doc;

  doc.LoadFile(configFileName);
  tinyxml2::XMLElement * elem = doc.FirstChildElement()->FirstChildElement();

  while(elem != NULL) {
    fileNames.push_back(elem->Attribute("file"));

    elem = elem->NextSiblingElement();
  }

  return fileNames;
}

void renderScene() {

}

int main (int argc, char ** argv) {

  vector<string> fileNames;
  char configFileName[] = "config.xml";
  fileNames = extractFileNames(configFileName);

  for(string fileName : fileNames)
    for(Point point : extractPoints(fileName)) {
      cout << point.getX() << endl;
    }

}
