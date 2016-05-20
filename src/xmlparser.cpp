#include "xmlparser.h"
#include <iostream>

/*-----------------------------------------------------------------------------------
	API
-----------------------------------------------------------------------------------*/

XMLParser::XMLParser() {
  char configFileName[] = "config.xml";
  doc = new tinyxml2::XMLDocument();
  doc->LoadFile(configFileName);

  elem = doc->FirstChildElement();
}

XMLParser::XMLParser(char *configFileName) {
  doc = new tinyxml2::XMLDocument();
  doc->LoadFile(configFileName);

  elem = doc->FirstChildElement();
}

XMLParser::XMLParser(XMLParser* cSource) {
  elem = cSource->getElem();
  doc = cSource->getDoc();
}

XMLParser* XMLParser::deepCopy(XMLParser* cSource) {
  XMLParser* _parser = new XMLParser(cSource);

  tinyxml2::XMLNode *current = elem->ShallowClone( _parser->doc );
  for( tinyxml2::XMLNode *child=elem->FirstChild(); child; child=child->NextSibling() )
  {
    current->InsertEndChild( deepCopy( child, _parser->doc ) );
  }

  _parser->elem = current->ToElement();

  return _parser;
}


tinyxml2::XMLElement* XMLParser::getElem() const {
  return elem;
}

tinyxml2::XMLDocument* XMLParser::getDoc() const {
  return doc;
}

void XMLParser::FirstChildGroup() {
  elem = elem->FirstChildElement("group");
}

void XMLParser::NextSiblingGroup() {
  elem = elem->NextSiblingElement();
}

vector<File> XMLParser::extractFiles() {
  tinyxml2::XMLElement* temp = elem->FirstChildElement("models");
  if(temp != NULL)
    temp = temp->FirstChildElement();

  vector<File> files;
  while(temp != NULL) {
    string fileName = temp->Attribute("file");
    Material material = getMaterial(temp);

    files.push_back(File(fileName, material));

    temp = temp->NextSiblingElement();
  }

  return files;
}

vector<Light> XMLParser::getLights() {
  tinyxml2::XMLElement* temp = elem->FirstChildElement("lights");
  if(temp != NULL)
    temp = temp->FirstChildElement();

  vector<Light> lights;
  bool isPoint;
  while(temp != NULL) {
    Point position = getPoint(temp);

    if(strcmp(temp->Attribute("type"),"point") == 0)
      isPoint = true;
    else
      isPoint = false;

    lights.push_back(Light(position, isPoint));

    temp = temp->NextSiblingElement();
  }

  return lights;
}


Point XMLParser::getScale() {
  tinyxml2::XMLElement *temp = elem->FirstChildElement("scale");

  float x, y, z;
  x = 1; y = 1; z = 1;
  if (temp != NULL) {
    temp->QueryFloatAttribute( "x", &x );
    temp->QueryFloatAttribute( "y", &y );
    temp->QueryFloatAttribute( "z", &z );
  }

  return Point(x, y, z);
}

Rotation* XMLParser::getRotation() {
  Rotation *rotation;
  float angle, time, x, y, z;
  angle = 0, time = 0, x = 0; y = 0; z = 0;

  tinyxml2::XMLElement *temp = elem->FirstChildElement("rotate");

  if (temp != NULL) {
    temp->QueryFloatAttribute( "angle", &angle );
    temp->QueryFloatAttribute( "time", &time );

    temp->QueryFloatAttribute( "x", &x );
    temp->QueryFloatAttribute( "y", &y );
    temp->QueryFloatAttribute( "z", &z );
  }

  if (time != 0)
    rotation = new RotationAnimation(time, x, y, z);
  else
    rotation = new RotationStatic(angle, x, y, z);

  return rotation;
}

Translation* XMLParser::getTranslation() {
  Translation *translation;
  float x, y, z, time;
  x = 0; y = 0; z = 0, time = 0;

  tinyxml2::XMLElement *temp = elem->FirstChildElement("translate");

  if (temp != NULL) {
    temp->QueryFloatAttribute( "x", &x );
    temp->QueryFloatAttribute( "y", &y );
    temp->QueryFloatAttribute( "z", &z );
    temp->QueryFloatAttribute( "time", &time );
  }

  if (time != 0)
    translation = getTranslationCatmull(temp, time);
  else
    translation = new TranslationSimple(x, y, z);

  return translation;
}


/*-----------------------------------------------------------------------------------
	Helpers
-----------------------------------------------------------------------------------*/

TranslationCatmull* XMLParser::getTranslationCatmull( tinyxml2::XMLElement *temp
                                                    , float time
                                                    ) {

  temp = temp->FirstChildElement("point");
  vector<Point> *controlPoints = new vector<Point>();

  while(temp != NULL) {
    controlPoints->push_back(getPoint(temp));
    temp = temp->NextSiblingElement();
  }

  TranslationCatmull *translation = new TranslationCatmull(time, controlPoints);

  return translation;
}

tinyxml2::XMLNode* XMLParser::deepCopy(tinyxml2::XMLNode *src, tinyxml2::XMLDocument *destDoc) {
  tinyxml2::XMLNode *current = src->ShallowClone( destDoc );
  for( tinyxml2::XMLNode *child=src->FirstChild(); child; child=child->NextSibling() )
  {
    current->InsertEndChild( deepCopy( child, destDoc ) );
  }

  return current;
}

Point XMLParser::getPoint(tinyxml2::XMLElement *temp) {
  float x, y, z;
  x = 0; y = 0; z = 0;

  if (temp != NULL) {
    temp->QueryFloatAttribute( "x", &x );
    temp->QueryFloatAttribute( "y", &y );
    temp->QueryFloatAttribute( "z", &z );
  }

  return Point(x, y, z);
}

Material XMLParser::getMaterial(tinyxml2::XMLElement *temp) {
  Point amb, diff, spec, emi;

  amb = getAmbient(temp);
  diff = getDiffuse(temp);
  spec = getSpecular(temp);
  emi = getEmission(temp);

  return Material(amb, diff, spec, emi);
}

Point XMLParser::getAmbient(tinyxml2::XMLElement *temp) {
  float x, y, z;
  x = 0.2; y = 0.2; z = 0.2;
  if (temp != NULL) {
    temp->QueryFloatAttribute( "ambX", &x );
    temp->QueryFloatAttribute( "ambY", &y );
    temp->QueryFloatAttribute( "ambZ", &z );
  }

  return Point(x, y, z);
}

Point XMLParser::getDiffuse(tinyxml2::XMLElement *temp) {
  float x, y, z;
  x = 0.8; y = 0.8; z = 0.8;
  if (temp != NULL) {
    temp->QueryFloatAttribute( "diffX", &x );
    temp->QueryFloatAttribute( "diffY", &y );
    temp->QueryFloatAttribute( "diffZ", &z );
  }

  return Point(x, y, z);
}

Point XMLParser::getSpecular(tinyxml2::XMLElement *temp) {
  float x, y, z;
  x = 0; y = 0; z = 0;
  if (temp != NULL) {
    temp->QueryFloatAttribute( "specX", &x );
    temp->QueryFloatAttribute( "specY", &y );
    temp->QueryFloatAttribute( "specZ", &z );
  }

  return Point(x, y, z);
}

Point XMLParser::getEmission(tinyxml2::XMLElement *temp) {
  float x, y, z;
  x = 0.0; y = 0.0; z = 0.0;
  if (temp != NULL) {
    temp->QueryFloatAttribute( "emiX", &x );
    temp->QueryFloatAttribute( "emiY", &y );
    temp->QueryFloatAttribute( "emiZ", &z );
  }

  return Point(x, y, z);
}
