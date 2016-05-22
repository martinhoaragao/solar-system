#include "group.h"

/*------------------------------------------------------------------------------
	API
------------------------------------------------------------------------------*/

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

  isBackground = parser->isBackground();
  translation = parser->getTranslation();
  scale = parser->getScale();
  rotation = parser->getRotation();
  files = parser->extractFiles();
  lights = parser->getLights();

  parser->FirstChildGroup();

  while(parser->getElem() != NULL) {
    groups.push_back(new Group(parser->deepCopy(parser)));
    parser->NextSiblingGroup();
  }
}

void Group::draw(Point camera) {
  glPushMatrix();

  if (isBackground) {
    glDisable(GL_CULL_FACE);
    glTranslatef(camera.getX(), camera.getY(), camera.getZ());
  }

  translation->glTranslate();
  rotation->glRotate();
  glScalef(scale.getX(), scale.getY(), scale.getZ());

  for(int i = 0; i < lights.size(); i++) {
    lights.at(i).draw();
  }
  
  for(int i = 0; i < files.size(); i++) {
    files.at(i).draw();
  }

  // Recursive draw other groups
  for(int i = 0; i < groups.size(); i++) {
    groups.at(i)->draw(camera);
  }

  if (isBackground) {
    glEnable(GL_CULL_FACE);
  }

  glPopMatrix();
}
