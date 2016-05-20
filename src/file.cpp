#include "file.h"
#include <sstream>
#include <fstream>
#include <iostream>

/*------------------------------------------------------------------------------
	API
------------------------------------------------------------------------------*/

File::File(string fn, Material mat) {
  // Generate Vertex Buffer Objects

  glGenBuffers(3, coordinatesID);
  //glGenTextures(1, &textureID);
  textureID = 0;

  fileName = fn;
  material = mat;
  loadFile();
}

File::File(string fn, Material mat, string tn) {
  // Generate Vertex Buffer Objects

  glGenBuffers(3, coordinatesID);

  fileName = fn;
  material = mat;
  loadTexture(tn);
  loadFile();
}

void File::draw() {
  glBindTexture(GL_TEXTURE_2D, textureID);

  glBindBuffer(GL_ARRAY_BUFFER, coordinatesID[0]);
  glVertexPointer(3, GL_FLOAT, 0, NULL);

  glBindBuffer(GL_ARRAY_BUFFER, coordinatesID[1]);
  glNormalPointer(GL_FLOAT, 0, NULL);

  glBindBuffer(GL_ARRAY_BUFFER, coordinatesID[2]);
  glTexCoordPointer(2,GL_FLOAT,0,0);

  material.draw();

  glDrawArrays(GL_TRIANGLES, 0, numberCoordinates[0]/3);
  glBindTexture(GL_TEXTURE_2D, 0);

  //Force display to be drawn now
  glFlush();
}

/*------------------------------------------------------------------------------
	Helpers
------------------------------------------------------------------------------*/

void File::loadTexture(string tn) {
  cv::Mat img = cv::imread(tn);
  if(!img.data)
    std::cout <<  "Could not open or find the image: " << tn << std::endl;

  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.cols, img.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img.data);
  glBindTexture( GL_TEXTURE_2D, 0 );

  std::cout <<  (atan2(-0.000001, -2)  + M_PI) / (2*M_PI) << " " << (atan2(-2, 0)  + M_PI) / (2*M_PI) << " " << (atan2(0, 2)  + M_PI) / (2*M_PI) << " " << (atan2(2, 0)  + M_PI) / (2*M_PI)<< std::endl;
  std::cout << ((-(0.9/1)) + 1) / 2 << " " << ((-(0/1)) + 1) / 2.0 << " " << ((-(-0.9/1)) + 1) / 2 << std::endl;
  std::cout << atan2(-0.1, -0.1) << " " <<  atan2(-0.1, 0.1) << " " << atan2(1, 1) << std::endl;
  std::cout << (atan2(-0.1, -0.1)  + M_PI) / (2.0*M_PI) << " " <<  (atan2(-0.1, 0.1)  + M_PI) / (2*M_PI) << " " <<(atan2(1, 1)  + M_PI) / (2*M_PI) << std::endl;
}

void File::loadFile() {
  vector<float>* points;
  ifstream pointsFile;
  string line;
  float x, y, z;

  pointsFile.open(fileName);

  for(int i = 0; i < 3; i++) {
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

    points->push_back(x);
    points->push_back(y);
    points->push_back(z);
  }

  return points;
}

void File::uploadData(vector<float>* points, int i) {
  glBindBuffer(GL_ARRAY_BUFFER, coordinatesID[i]);
  glBufferData(GL_ARRAY_BUFFER, numberCoordinates[i] * sizeof(float) * 3, &points->front(), GL_STATIC_DRAW);
}
