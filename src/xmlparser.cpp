#include "xmlparser.h"

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

vector<string> XMLParser::extractFileNames() {
  tinyxml2::XMLElement* temp = elem->FirstChildElement("models");
  if(temp != NULL)
    temp = temp->FirstChildElement();

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

Point XMLParser::getTranslation() {
  tinyxml2::XMLElement *temp = elem->FirstChildElement("translate");
  float x, y, z;
  x = 0; y = 0; z = 0;

  if (temp != NULL) {
    temp->QueryFloatAttribute( "x", &x );
    temp->QueryFloatAttribute( "y", &y );
    temp->QueryFloatAttribute( "z", &z );
  }

  return Point(x, y, z);
}

/*-----------------------------------------------------------------------------------
	Helpers
-----------------------------------------------------------------------------------*/

tinyxml2::XMLNode* XMLParser::deepCopy(tinyxml2::XMLNode *src, tinyxml2::XMLDocument *destDoc) {
  tinyxml2::XMLNode *current = src->ShallowClone( destDoc );
  for( tinyxml2::XMLNode *child=src->FirstChild(); child; child=child->NextSibling() )
  {
    current->InsertEndChild( deepCopy( child, destDoc ) );
  }

  return current;
}
