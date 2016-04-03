#ifndef XMLPARSER_H
#define XMLPARSER_H

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

using namespace std;

class XMLParser {
  private:
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* elem;

  public:
    XMLParser();
    XMLParser(char*);
    tinyxml2::XMLElement* getElem(); 
    void FirstChildGroup();
    void NextSiblingGroup();
    vector<string> extractFileNames();
    Point getScale();
    Rotation getRotation();
    Point getTranslation();

};

#endif
