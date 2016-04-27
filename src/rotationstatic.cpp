#include "rotationstatic.h"


RotationStatic::RotationStatic() : Rotation() {
  angle = 0;
}

RotationStatic::RotationStatic( float _angle
                              , Point point
                              ) : Rotation(point) {
  angle = _angle;
}

RotationStatic::RotationStatic( float _angle
                              , float x
                              , float y
                              , float z
                              ) : Rotation (x, y, z) {
  angle = _angle;
}

void RotationStatic::glRotate() {
  glRotatef(angle, coordinate.getX(), coordinate.getY(), coordinate.getZ());
}
