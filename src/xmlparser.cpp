#include "xmlparser.h"

#include <iostream>
/*-----------------------------------------------------------------------------------
	API
-----------------------------------------------------------------------------------*/

XMLParser::XMLParser() {
  char configFileName[] = "config.xml";

  doc.LoadFile(configFileName);

  elem = doc.FirstChildElement();
}

XMLParser::XMLParser(char *configFileName) {
  doc.LoadFile(configFileName);

  elem = doc.FirstChildElement();
}

tinyxml2::XMLElement* XMLParser::getElem() {
  return elem;
}

void XMLParser::FirstChildGroup() {
  elem = elem->FirstChildElement("group");
}

void XMLParser::NextSiblingGroup() {
  elem = elem->NextSiblingElement();
}

vector<string> XMLParser::extractFileNames() {
  cout << elem->FirstChildElement("models")->Name() << endl;
  tinyxml2::XMLElement* temp = elem->FirstChildElement("models")->FirstChildElement();

  vector<string> fileNames;
  while(temp != NULL) {
    fileNames.push_back(temp->Attribute("file"));

    temp = temp->NextSiblingElement();
  }

  return fileNames;
}

Point XMLParser::getScale() {
  tinyxml2::XMLElement *temp = elem->FirstChildElement("scale");
  float x, y, z;
  x = 1; y = 1; z = 1;

  temp->QueryFloatAttribute( "x", &x );
  temp->QueryFloatAttribute( "y", &y );
  temp->QueryFloatAttribute( "z", &z );

  return Point(x, y, z);
}

Rotation XMLParser::getRotation() {
  tinyxml2::XMLElement *temp = elem->FirstChildElement("rotate");
  float angle, x, y, z;
  angle = 0, x = 0; y = 0; z = 0;

  temp->QueryFloatAttribute( "angle", &angle );
  temp->QueryFloatAttribute( "x", &x );
  temp->QueryFloatAttribute( "y", &y );
  temp->QueryFloatAttribute( "z", &z );

  return Rotation(angle, x, y, z);
}

Point XMLParser::getTranslation() {
  tinyxml2::XMLElement *temp = elem->FirstChildElement("translate");
  float x, y, z;
  x = 0; y = 0; z = 0;

  temp->QueryFloatAttribute( "x", &x );
  temp->QueryFloatAttribute( "y", &y );
  temp->QueryFloatAttribute( "z", &z );

  return Point(x, y, z);
}

/*-----------------------------------------------------------------------------------
	Helpers
-----------------------------------------------------------------------------------*/
