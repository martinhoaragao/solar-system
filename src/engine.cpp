#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#include <GLUT/glut.h>
#include "tinyxml2.h"

using namespace std;

struct Vertex {
  float x;
  float y;
  float z;
};

float* vertexToArray (Vertex vertex) {
  float[3] vertexArray;

  vertexArray[0] = vertex.x;
  vertexArray[1] = vertex.y;
  vertexArray[2] = vertex.z;

  return vertexArray;
}

void drawTriangle(vector<Vertex> vertices) {
  glBegin(GL_TRIANGLES);
  for(int i = 0; i < 3; i++){
    glVertex3fv(vertexArray(vertices.pop_back()));
  }
  glEnd();
}

void drawTriangles(vector<Vertex> vertices) {
  for(int i = 0; i < vertices.size(); i+=3)
  drawTriangle(vertices);
}

vector<Vertex> extractVertices(char * fileName) {
  vector<Vertex> vertices;
  Vertex vertex;
  ifstream verticesFile;
  string line;

  verticesFile.open (fileName);

  while (getline(verticesFile, line)) {
    istringstream iss(line);

    if (!(iss >> vertex.x >> vertex.y >> vertex.z)) {
      break;
    } // error

    vertices.push_back(vertex);
  }

  verticesFile.close();
  return vertices;
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

int main (int argc, char ** argv) {

  vector<string> fileNames;
  char configFileName[] = "config.xml";
  fileNames = extractFileNames(configFileName);

  for(string fileName : fileNames)
  cout << fileName << endl;

}
