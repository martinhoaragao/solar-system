#ifndef GROUP_H
#define GROUP_H

#include "file.h"
#include "xmlparser.h"

using namespace std;

class Group {
  private:
    bool isBackground;
    Point scale;
    Rotation *rotation;
    Translation *translation;
    vector<File> files;
    vector<Light> lights;
    vector<Group *> groups;

    void init(XMLParser*);

  public:
    Group();
    Group(char*);
    Group(XMLParser*);

    void draw(Point);


};

#endif
