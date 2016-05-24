#include "material.h"

/*------------------------------------------------------------------------------
	API
------------------------------------------------------------------------------*/

Material::Material() {}

Material::Material(Point amb, Point diff, Point spec, Point emi, float shini) {
  ambient[0] = amb.x;
  ambient[1] = amb.y;
  ambient[2] = amb.z;

  diffuse[0] = diff.x;
  diffuse[1] = diff.y;
  diffuse[2] = diff.z;

  specular[0] = spec.x;
  specular[1] = spec.y;
  specular[2] = spec.z;

  emission[0] = emi.x;
  emission[1] = emi.y;
  emission[2] = emi.z;

  shininess[0] = shini;
}

void Material::draw() {
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
}
