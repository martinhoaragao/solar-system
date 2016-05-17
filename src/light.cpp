#include "light.h"

/*------------------------------------------------------------------------------
	API
------------------------------------------------------------------------------*/

Light::Light(Point point, bool _isVector) {
  position = point;
  isVector = _isVector;
}

void Light::render() {
  GLfloat amb[4] = {0.2, 0.2, 0.2, 1.0};
  GLfloat diff[4] = {1.0, 1.0, 1.0, 1.0};
  GLfloat pos[4] = {position.x, position.y , position.z, (float) isVector};

  // light position
  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  // light colors
  glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
}
