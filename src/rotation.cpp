#include "rotation.h"

Rotation::Rotation() {
  coordinate = Point();
}

Rotation::Rotation(Point point) {
  coordinate = point;
}

Rotation::Rotation(float x, float y, float z) {
  coordinate = Point(x, y, z);
}

void Rotation::glRotate(){};
