#ifndef POINT_H
#define POINT_H

class Vertex {
  private:
    float x, y, z;

  public:
    Vertex(float, float, float);
    float getX();
    float getY();
    float getZ();
};

#endif
