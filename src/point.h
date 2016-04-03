#ifndef POINT_H
#define POINT_H

class Point {
  private:
    float x, y, z;

  public:
    Point();
    Point(float, float, float);
    float getX();
    float getY();
    float getZ();
    void setX(float);
    void setY(float);
    void setZ(float);
    float* toArray();
};

#endif
