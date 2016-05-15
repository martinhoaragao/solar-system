#ifndef PATCHPOINTS_H
#define PATCHPOINTS_H

class PatchPoints {
  private:
    int nrPatches;
    int nrCP;
    int nextPatch;
    int nextCP;
    int patches[1000][16];
    float controlPoints[10000][3];

  public:

    PatchPoints();
    int getNrPatches();
    int getNrCP();
    int getNextPatch();
    int getNextCP();
    int* getPatch(int);
    float* getCP(int);
    void setNrPatches(int);
    void setNrCP(int);
    void addNextPatch();
    void addNextCP();
    void addPatch(int*);
    void addCP(float*);
};

#endif