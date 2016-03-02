#include "Vertex.h"

Vertex::Vertex(float X, float Y, float Z) {
  x = X; y = Y; z = Z;
}

float Vertex::getX() { return x; }

float Vertex::getY() { return y; }

float Vertex::getZ() { return z; }
