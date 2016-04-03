#include "group.h"

/*-----------------------------------------------------------------------------------
	API
-----------------------------------------------------------------------------------*/

Group::Group() {
  XMLParser* parser = new XMLParser();
  parser->FirstChildGroup();

  init(parser);
}

Group::Group(char* configFileName) {

  XMLParser* parser = new XMLParser(configFileName);
  parser->FirstChildGroup();

  init(parser);
}

Group::Group(XMLParser* parser) {
  init(parser);
}

void Group::init(XMLParser* parser) {
  scale = Point(1, 1, 1);
  rotation = Rotation(0, 1, 1, 1);
  translation = Point(0, 0, 0);

  scale = parser->getScale();
  rotation = parser->getRotation();
  translation = parser->getTranslation();

  vector<string> fileNames = parser->extractFileNames();
  for(int i = 0; i < fileNames.size(); i++) {
    File file(fileNames.at(i));
    files.push_back(file);
  }

  parser->FirstChildGroup();

  while(parser->getElem() != NULL) {
    groups.push_back(new Group(parser->deepCopy(parser)));
    parser->NextSiblingGroup();
  }
}

void Group::draw() {
  glPushMatrix();

  glTranslatef(translation.getX(), translation.getY(), translation.getZ());
  glScalef(scale.getX(), scale.getY(), scale.getZ());
  glRotatef(rotation.getAngle(), rotation.getX(), rotation.getY(), rotation.getZ());

  for(int i = 0; i < files.size(); i++) {
    files.at(i).draw();
  }

  // Recursive draw other groups
  for(int i = 0; i < groups.size(); i++) {
    groups.at(i)->draw();
  }

  glPopMatrix();
}

/*-----------------------------------------------------------------------------------
	Helpers
-----------------------------------------------------------------------------------*/

vector<string> Group::extractFileNames (char* configFileName) {
  tinyxml2::XMLDocument doc;
  vector<string> fileNames;

  doc.LoadFile(configFileName);
  tinyxml2::XMLElement* elem = doc.FirstChildElement()->FirstChildElement();

  while(elem != NULL) {
    fileNames.push_back(elem->Attribute("file"));

    elem = elem->NextSiblingElement();
  }

  return fileNames;
}
