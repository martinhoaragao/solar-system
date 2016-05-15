#ifndef POINT_H
#define POINT_H

class Point {
  private:

  public:
    float x, y, z;
    Point();
    Point(float, float, float);
    float getX();
    float getY();
    float getZ();
    float get(int);
    void setX(float);
    void setY(float);
    void setZ(float);
    float* toArray();
};

#endif
