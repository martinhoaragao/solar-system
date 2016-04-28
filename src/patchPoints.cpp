#include "patchPoints.h"

PatchPoints::PatchPoints(){
	nextPatch = 0; nextCP = 0;
}

int PatchPoints::getNrPatches() {return nrPatches;}

int PatchPoints::getNrCP() {return nrCP;}

int PatchPoints::getNextPatch(){return nextPatch;}

int PatchPoints::getNextCP(){return nextCP;}

int* PatchPoints::getPatch(int nrPatch){ return patches[nrPatch];}

float* PatchPoints::getCP(int nrCP){return controlPoints[nrCP];}

void PatchPoints::setNrPatches(int _nrPatches) {nrPatches = _nrPatches;}

void PatchPoints::setNrCP(int _nrCP) {nrCP = _nrCP;}

void PatchPoints::addNextPatch(){nextPatch++;}

void PatchPoints::addNextCP(){nextCP++;}

void PatchPoints::addPatch(int* points){
	//index to write the patch control points indices
	int patchIndex = getNextPatch();
	//next time it will write in +1 position
	addNextPatch();

	for(int i = 0; i<16; i++) {
		patches[patchIndex][i] = points[i];
	}
}

void PatchPoints::addCP(float* points){
	//index to write the control points 
	int cpIndex = getNextCP();
	//next time it will write in +1 position
	addNextCP();

	controlPoints[cpIndex][0] = points[0];
	controlPoints[cpIndex][1] = points[1];
	controlPoints[cpIndex][2] = points[2];
} 