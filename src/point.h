#ifndef POINT_H
#define POINT_H

class point {
  private:
    float x, y, z;

  public:
    point(float, float, float);
    float getX();
    float getY();
    float getZ();
};

#endif
