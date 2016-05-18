#include "material.h"

/*------------------------------------------------------------------------------
	API
------------------------------------------------------------------------------*/

Material::Material() {
  ambient = Point(0.2, 0.2, 0.2);
  diffuse = Point(0.8, 0.8, 0.8);
  specular = Point(0, 0, 0);
  emission = Point(0, 0, 0);
}

Material::Material(Point amb, Point diff, Point spec, Point emi) {
  ambient = amb;
  diffuse = diff;
  specular = spec;
  emission = emi;
}

void Material::draw() {
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient.toArray());
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse.toArray());
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular.toArray());
  glMaterialfv(GL_FRONT, GL_EMISSION, emission.toArray());
}
