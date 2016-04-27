#include "translationsimple.h"


TranslationSimple::TranslationSimple() : Translation() {
  point = Point();
}

TranslationSimple::TranslationSimple(Point _point) : Translation() {
  point = _point;
}

TranslationSimple::TranslationSimple( float x
                                    , float y
                                    , float z
                                  ) : Translation () {
  point = Point(x, y, z);
}

void TranslationSimple::glTranslate() {
  glTranslatef(point.getX(), point.getY(), point.getZ());
}
