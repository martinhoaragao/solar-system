#include "group.h"

/*-----------------------------------------------------------------------------------
	API
-----------------------------------------------------------------------------------*/

Group::Group() {
  XMLParser* parser = new XMLParser();

  init(parser);
}

Group::Group(char* configFileName) {
  XMLParser* parser = new XMLParser(configFileName);

  init(parser);
}

Group::Group(XMLParser* parser) {
  init(parser);
}

void Group::init(XMLParser* parser) {

  translation = parser->getTranslation();
  scale = parser->getScale();
  rotation = parser->getRotation();

  vector<string> fileNames = parser->extractFileNames();

  for(int i = 0; i < fileNames.size(); i++) {
    File file(fileNames.at(i));
    files.push_back(file);
  }

  lights = parser->getLights();

  parser->FirstChildGroup();

  while(parser->getElem() != NULL) {
    groups.push_back(new Group(parser->deepCopy(parser)));
    parser->NextSiblingGroup();
  }
}

void Group::draw() {
  glPushMatrix();

  for(int i = 0; i < lights.size(); i++) {
    lights.at(i).draw();
  }

  translation->glTranslate();

  rotation->glRotate();

  glScalef(scale.getX(), scale.getY(), scale.getZ());

  // Light inside
  if (lights.size() > 0)
    glDisable(GL_LIGHTING);

  for(int i = 0; i < files.size(); i++) {
    files.at(i).draw();
  }

  glEnable(GL_LIGHTING);

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
