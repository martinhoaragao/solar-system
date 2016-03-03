#include <GLUT/glut.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <queue>
#include <fstream>
#include <iostream>

#include "tinyxml2.h"
#include "point.h"

using namespace std;

queue<string> extractFileNames (char* configFileName) {
  queue<string> fileNames;
  tinyxml2::XMLDocument doc;

  doc.LoadFile(configFileName);
  tinyxml2::XMLElement * elem = doc.FirstChildElement()->FirstChildElement();

  while(elem != NULL) {
    fileNames.push(elem->Attribute("file"));

    elem = elem->NextSiblingElement();
  }
  return fileNames;
}

queue<Point>* extractPoints(string fileName) {
  queue<Point>* points = new queue<Point>();
  ifstream pointsFile;
  string line;
  float x, y, z;

  pointsFile.open(fileName);

  while (getline(pointsFile, line)) {
    istringstream iss(line);

    iss >> x >> y >> z;
    Point point = Point(x,y,z);

    points->push(point);
  }

  pointsFile.close();
  return points;
}

void drawTriangle(queue<Point>* points) {
  glBegin(GL_TRIANGLES);
  for(int i = 0; i < 3; i++){
    Point point = points->front();
    glVertex3f(point.getX(), point.getY(), point.getZ());
    points->pop();
  }
  glEnd();
}

void drawTriangles(queue<Point>* points) {
  int numTriangles = points->size()/3;
  for(int i = 0; i < numTriangles; i++) {
    drawTriangle(points);
  }
}

void drawTrianglesFromFile() {
  queue<string> fileNames;
  char configFileName[] = "config.xml";
  fileNames = extractFileNames(configFileName);

  while(fileNames.size() > 0) {
    queue<Point>* points = extractPoints(fileNames.front());
    fileNames.pop();
    drawTriangles(points);
  }
}

void renderScene() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // set the camera
  glLoadIdentity();
  gluLookAt(1.0,-1.0,4.0,
          0.0,0.0,0.0,
        0.0,1.0,0.0);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


  drawTrianglesFromFile();

  // End of frame
  glutSwapBuffers();
}

void changeSize(int width, int height) {

  // Prevent a divide by zero, when window is too short
  if(height == 0)
    height = 1;

  float windowAspectRatio = width / height;

  // Set the projection matrix as current
  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();

  // Set the viewport to be the entire window
  glViewport(0, 0, width, height);

  gluPerspective(45.0f, windowAspectRatio, 1.0f ,1000.0);

  // return to the model view matrix mode
  glMatrixMode(GL_MODELVIEW);
}

int main (int argc, char** argv) {

  // init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("CG-first-step");

  // Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

  //  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

  // enter GLUT's main cycle
    glutMainLoop();

    return 1;
}
