#ifndef GROUP_H
#define GROUP_H

#include "file.h"
#include "xmlparser.h"

using namespace std;

class Group {
  private:
    Point scale;
    Rotation *rotation;
    Translation *translation;
    vector<File> files;
    vector<Group *> groups;

    void init(XMLParser*);
    vector<string> extractFileNames(char*);

  public:
    Group();
    Group(char*);
    Group(XMLParser*);

    void draw();


};

#endif
