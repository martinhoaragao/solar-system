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

  parser->FirstChildGroup();
  cout << "Parser address copied: ";
  cout << (parser->getElem()) << endl;
  cout << (parser->getElem()->Value()) << endl;


  XMLParser* child = parser->deepCopy(parser);
  cout << "Parser address changed? ";
  cout << (parser->getElem()) << endl;
  cout << (parser->getElem()->Value()) << endl;
  cout << "Child address copied: ";
  cout << (child->getElem()) << endl;
  cout << (child->getElem()->Value()) << endl;

  cout << child << " vs " << parser << endl;


  while(parser->getElem() != NULL) {
    groups.push_back(new Group(child));
    cout << "pushed 1" << endl;

  if (parser->getElem() == NULL)
    cout << "PARSER IS NULL NOW" << endl;
    parser->NextSiblingGroup();
    cout << "went to sibling" << endl;

  }
  cout << "THE END" << endl;

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
  XMLParser* child = parser;
  while(parser->getElem() != NULL) {
    groups.push_back(new Group(child));

    parser->NextSiblingGroup();
  }
  cout << "THE END" << endl;

}

Group::Group(XMLParser* parser) {
  scale = Point(1, 1, 1);
  rotation = Rotation(0, 1, 1, 1);
  translation = Point(0, 0, 0);

  cout << "Error 1" << endl;
  scale = parser->getScale();
  cout << "Error 1.1" << endl;

  rotation = parser->getRotation();
  translation = parser->getTranslation();

  cout << "Error 2" << endl;

  vector<string> fileNames = parser->extractFileNames();
  for(int i = 0; i < fileNames.size(); i++) {
    File file(fileNames.at(i));
    files.push_back(file);
  }
  cout << "Error 3" << endl;

  parser->FirstChildGroup();
  XMLParser* child = parser;
  while(parser->getElem() != NULL) {
    groups.push_back(new Group(child));
    cout << "Error 4" << endl;

    parser->NextSiblingGroup();
    if (parser == NULL)
      cout << "IT IS NULL" << endl;
  }
  cout << "Error 5" << endl;

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
