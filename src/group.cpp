#include "group.h"

#include <iostream>


/*-----------------------------------------------------------------------------------
	API
-----------------------------------------------------------------------------------*/

Group::Group() {

  XMLParser* parser = new XMLParser();

  parser->FirstChildGroup();

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
  cout << "PLEASE3" <<endl;

  parser->FirstChildGroup();
  cout << "maybe?" << endl;
  while(parser->getElem() != NULL) {
    groups.push_back(new Group(parser));
    parser->NextSiblingGroup();
  }
  cout << "PLEASE4" <<endl;

}
Group::Group(char* configFileName) {

  XMLParser* parser = new XMLParser(configFileName);
  parser->FirstChildGroup();

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
  while(parser != NULL) {
    groups.push_back(new Group(parser));
    parser->NextSiblingGroup();
  }
}

Group::Group(XMLParser* parser) {
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
  while(parser != NULL) {
    groups.push_back(new Group(parser));
    parser->NextSiblingGroup();
  }
}

void Group::draw() {
  glPushMatrix();

  glScalef(scale.getX(), scale.getY(), scale.getZ());
  glRotatef(rotation.getAngle(), rotation.getX(), rotation.getY(), rotation.getZ());
  glTranslatef(translation.getX(), translation.getY(), translation.getZ());

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
