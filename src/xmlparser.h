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
#include "rotationstatic.h"
#include "rotationanimation.h"


using namespace std;

class XMLParser {
  private:
    tinyxml2::XMLDocument* doc;
    tinyxml2::XMLElement* elem;

    tinyxml2::XMLNode *deepCopy(tinyxml2::XMLNode*, tinyxml2::XMLDocument*);

  public:
    XMLParser();
    XMLParser(char*);
    XMLParser(XMLParser*);
    XMLParser* deepCopy(XMLParser*);
    tinyxml2::XMLElement* getElem() const;
    tinyxml2::XMLDocument* getDoc() const;
    void FirstChildGroup();
    void NextSiblingGroup();
    vector<string> extractFileNames();
    Point getScale();
    Rotation* getRotation();
    Point getTranslation();


};

#endif
