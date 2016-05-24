#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "../lib/tinyxml2.h"
#include "light.h"
#include "file.h"
#include "material.h"
#include "rotationstatic.h"
#include "rotationanimation.h"
#include "translationsimple.h"
#include "translationcatmull.h"

using namespace std;

class XMLParser {
  private:
    tinyxml2::XMLDocument* doc;
    tinyxml2::XMLElement* elem;

    tinyxml2::XMLNode* deepCopy(tinyxml2::XMLNode*, tinyxml2::XMLDocument*);
    TranslationCatmull* getTranslationCatmull( tinyxml2::XMLElement*, float);
    Point getPoint(tinyxml2::XMLElement*);
    Material getMaterial(tinyxml2::XMLElement*);
    float getShini(tinyxml2::XMLElement*);
    Point getAmbient(tinyxml2::XMLElement*);
    Point getDiffuse(tinyxml2::XMLElement*);
    Point getSpecular(tinyxml2::XMLElement*);
    Point getEmission(tinyxml2::XMLElement*);

  public:
    XMLParser();
    XMLParser(char*);
    XMLParser(XMLParser*);
    XMLParser* deepCopy(XMLParser*);
    tinyxml2::XMLElement* getElem() const;
    tinyxml2::XMLDocument* getDoc() const;
    void FirstChildGroup();
    void NextSiblingGroup();
    vector<File> extractFiles();
    vector<Light> getLights();
    bool isBackground();
    Point getScale();
    Rotation* getRotation();
    Translation* getTranslation();

};

#endif
