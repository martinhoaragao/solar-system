#include "rotation.h"

Rotation::Rotation() {
  angle = 0;
  coordinate = Point();
}

Rotation::Rotation(float _angle, Point point) {
  angle = _angle;
  coordinate = point;
}

Rotation::Rotation(float _angle, float x, float y, float z) {
  angle = _angle;
  coordinate = Point(x, y, z);
}

float Rotation::getAngle() { return angle; }

float Rotation::getX() { return coordinate.getX(); }

float Rotation::getY() { return coordinate.getY(); }

float Rotation::getZ() { return coordinate.getZ(); }
