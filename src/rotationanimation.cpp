#include "rotationanimation.h"

RotationAnimation::RotationAnimation() : Rotation() {
  time = 0;
}

RotationAnimation::RotationAnimation( float _time
                                    , Point point
                                    ) : Rotation(point) {
  time = _time;
}

RotationAnimation::RotationAnimation( float _time
                                    , float x
                                    , float y
                                    , float z
                                    ) : Rotation (x, y, z) {
  time = _time;
}

void RotationAnimation::glRotate() {
  float now = glutGet(GLUT_ELAPSED_TIME);
  float angle = fmod(now, time) / time * 360.0;

  glRotatef(angle, coordinate.getX(), coordinate.getY(), coordinate.getZ());
}
