#include <iostream> // Needed to write to files.
#include <fstream>  // Needed to write to files.
#include <cstdlib>
#include <stdio.h>
#include <sstream>  
#include <string.h>
#include <math.h>
#include <deque>
#include "Point.h"
#include "patchPoints.h"

using namespace std;

PatchPoints patchPoints;

/* Functions prototypes. */
int bezier(int, char **);
int plane(int, char **);
int box(int, char **);
int sphere(int, char **);
int cone(int, char **);
deque<Point>* circle(int, float, float, int);
int torus(int, char **);
void writeVerticesToFile(char **, int, FILE *);
void renderBezierCurve(char *, char * );

int main (int argc, char ** argv) {
  // Check for arguments.
  // Only 'plane', 'box', 'sphere', 'cone' and 'torus' are possible.
  if (argc > 1) {
    if (strcmp(argv[1], "plane") == 0) {
      plane(argc - 2, &argv[2]) != 0 ? printf("Error!\n") : printf("Done\n");
    }
    else if (strcmp(argv[1], "box") == 0) {
      box(argc - 2, &argv[2])   != 0 ? printf("Error!\n") : printf("Done\n");
    }
    else if (strcmp(argv[1], "sphere") == 0) {
      sphere(argc - 2, &argv[2]) != 0 ? printf("Error!\n") : printf("Done\n");
    }
    else if (strcmp(argv[1], "cone") == 0) {
      cone(argc - 2, &argv[2]) != 0 ? printf("Error!\n") : printf("Done\n");
    }
    else if (strcmp(argv[1], "torus") == 0) {
      torus(argc - 2, &argv[2]) != 0 ? printf("Error!\n") : printf("Done\n");
    }
    else {
      bezier(argc -1, &argv[1]) != 0 ? printf("Error!\n") : printf("Done\n");
    }
  } else {
    printf("Error! Arguments needed!\n");
  }

  return 0;
}


/* Ouput the vertices needed to create a square in the XZ plane centered in the origin
 * made with 2 triangles.
 * The number of arguments should be 2 which are the square width and the output file name. */
int plane(int argc, char ** parameters) {
  // Check number of arguments.
  if (argc != 2) { return -1; }
  else {
    // get the square width.
    float width     = stof(parameters[0]);
    float d         = width/2.0;

    // create/open output file.
    char filename[100];
    sprintf(filename, "%s.3d", parameters[1]);
    FILE * file     = fopen(filename, "w+");

    // write to file.
    fprintf(file, "%f 0.0 %f\n", -d, -d);
    fprintf(file, "%f 0.0 %f\n", -d,  d);
    fprintf(file, "%f 0.0 %f\n",  d,  d);
    fprintf(file, "%f 0.0 %f\n",  d, -d);
    fprintf(file, "%f 0.0 %f\n", -d, -d);
    fprintf(file, "%f 0.0 %f\n",  d,  d);

    fclose(file);
  }

  return 0;
}

int box(int argc, char ** parameters) {
  // Check number of arguments.
  if (argc != 4) { return -1; }
  else {
    // Get the box dimensions.
    float x = stof(parameters[0])/2.0;
    float y = stof(parameters[1]);
    float z = stof(parameters[2])/2.0;

    // Create vertices.
    char A[50], B[50], C[50], D[50], E[50], F[50], G[50], H[50];
    sprintf(A, "%f %f %f", -x, y, -z);
    sprintf(B, "%f %f %f",  x, y, -z);
    sprintf(C, "%f %f %f", -x, y,  z);
    sprintf(D, "%f %f %f",  x, y,  z);
    sprintf(E, "%f 0.0 %f", -x, -z);
    sprintf(F, "%f 0.0 %f",  x, -z);
    sprintf(G, "%f 0.0 %f", -x,  z);
    sprintf(H, "%f 0.0 %f",  x,  z);
    char * vertices[36] = {
      E, H, G, F, H, E, // Base.
      A, C, D, B, A, D, // Top.
      C, G, H, D, C, H, // Front.
      D, H, F, B, D, F, // Right.
      A, B, E, B, F, E, // Back.
      C, A, E, C, E, G  // Left.
    };

    // Open/Create file and write number of vertices to the file.
    char filename[100];
    sprintf(filename, "%s.3d", parameters[3]);
    FILE * file = fopen(filename, "w+");

    // Write vertices to file.
    writeVerticesToFile(vertices, 36, file);
  }

  return 0;
}

int sphere(int argc, char ** parameters) {
  // Check number of arguments.
  if (argc != 4) { return -1; }
  else {
    // Parse sphere radius, slices and stacks.
    float radius  = stof(parameters[0]);
    int slices    = stof(parameters[1]);
    int stacks    = stof(parameters[2]);
    float beta    = 0.0;
    float bInc    = M_PI / stacks;
    float currY   = 1.0, yDec = 1.0f/stacks;
    float angleA  = M_PI / 2;
    float incA    = M_PI / stacks;
    float angleB  = 0.0;
    float incB    = (2 * M_PI)/ slices;
    float r, h;

    // Open/Create file and write number of vertices to the file.
    char filename[100];
    sprintf(filename, "%s.3d", parameters[3]);
    FILE * file = fopen(filename, "w+");

    for (int stack = 0; stack < stacks; stack++, angleA += incA) {
      r = radius * cos(angleA);
      h = radius * sin(angleA);
      angleB  = 0.0;

      for (int slice = 0; slice < slices; slice++, angleB += incB) {
        Point p = Point(r * sin(angleB), h, r * cos(angleB));

        fprintf(file, "%f %f %f\n", p.getX(), p.getY(), p.getZ());
        fprintf(file, "%f %f %f\n",
            (radius * cos(angleA + incA)) * sin(angleB),
            (radius * sin(angleA + incA)),
            (radius * cos(angleA + incA)) * cos(angleB));
        fprintf(file, "%f %f %f\n",
            (radius * cos(angleA + incA)) * sin(angleB + incB),
            (radius * sin(angleA + incA)),
            (radius * cos(angleA + incA)) * cos(angleB + incB));

        fprintf(file, "%f %f %f\n", p.getX(), p.getY(), p.getZ());
        fprintf(file, "%f %f %f\n",
            r * sin(angleB - incB),
            h,
            r * cos(angleB - incB));
        fprintf(file, "%f %f %f\n",
            (radius * cos(angleA + incA)) * sin(angleB),
            (radius * sin(angleA + incA)),
            (radius * cos(angleA + incA)) * cos(angleB));
      }
    }
  }

  return 0;
}

/** Draw a cone centered in the origin. */
int cone(int argc, char ** parameters) {
  // Check the number of arguments.
  if (argc != 5) { return -1; }
  else {
    // Parse base radius, height, slices and stacks.
    float radius  = stof(parameters[0]);
    float height  = stof(parameters[1]);
    int slices    = stoi(parameters[2]);
    int stacks    = stoi(parameters[3]);
    float angle   = 0.0;
    float inc     = (2 * M_PI)/slices;
    float h       = 0.0;
    float hInc    = (height)/stacks;
    float r       = ((height - h) * radius)/height;

    // Open/Create file.
    char filename[100];
    sprintf(filename, "%s.3d", parameters[4]);
    FILE * file = fopen(filename, "w+");

    // Draw base circunference.
    for (int slice = 0; slice < slices; slice++, angle += inc) {
      Point p = Point(r * sin(angle), h, r * cos(angle));

      fprintf(file, "0.0 0.0 0.0\n");
      fprintf(file, "%f 0.0 %f\n", r * sin(angle + inc), r * cos(angle + inc));
      fprintf(file, "%f 0.0 %f\n", p.getX(), p.getZ());
    }

    // Create faces.
    h += hInc;

    for (int stack = 0; stack < stacks; stack++, h += hInc) {
      r     = ((height - h) * radius) / height;
      angle = 0.0;

      for (int slice = 0; slice < slices; slice++, angle += inc) {
        Point p = Point(r * sin(angle), h, r * cos(angle));

        fprintf(file, "%f %f %f\n", p.getX(), p.getY(), p.getZ());
        fprintf(file, "%f %f %f\n",
            (((height - (h - hInc)) * radius)/height) * sin(angle),
            h - hInc,
            (((height - (h - hInc)) * radius)/height) * cos(angle));
        fprintf(file, "%f %f %f\n",
            (((height - (h - hInc)) * radius)/height) * sin(angle + inc),
            h - hInc,
            (((height - (h - hInc)) * radius)/height) * cos(angle + inc));

        fprintf(file, "%f %f %f\n", p.getX(), p.getY(), p.getZ());
        fprintf(file, "%f %f %f\n",
            r * sin(angle - inc),
            h,
            r * cos(angle - inc));
        fprintf(file, "%f %f %f\n",
            (((height - (h - hInc)) * radius)/height) * sin(angle),
            h - hInc,
            (((height - (h - hInc)) * radius)/height) * cos(angle));
      }
    }


  }

  return 0;
}

float sliceAngle(int slice, int slices) {
  return slice * ((2 * M_PI) / slices);
}

deque<Point>* circunference(int slices, float radius, float z) {
  deque<Point>* circunference = new deque<Point>();
  float x, y, angle;
  Point origin = Point(0, 0, 0);

  for(int i = 0; i < slices; i++) {
    angle = sliceAngle(i, slices);
    x = radius * sin(angle);
    y = radius * cos(angle);
    Point point = Point(x, y, z);
    circunference->push_back(point);
  }

  return circunference;
}

deque<Point>* innerCircle(int slices, float circunferenceRadius, float z) {
  deque<Point>* circle = new deque<Point>();

  deque<Point>* centerCircunference = circunference(slices, circunferenceRadius, z);
  while(centerCircunference->size() > 0) {
    Point point = centerCircunference->front();
    centerCircunference->pop_front();
    circle->push_back(point);
    circle->push_back(Point(0,0,0));
    circle->push_back(point);
  }
  circle->push_back(circle->front());
  circle->pop_front();

  return circle;
}

/* Draw triangles connecting 2 lines */
deque<Point>* triangulateLines(deque<Point>* innerCircunference, deque<Point>* outerCircunference, deque<Point>* lines) {
  for(int i = 0; i < innerCircunference->size(); i++) {
    int nextPoint = (i +1) % innerCircunference->size();

    Point innerPoint1 = innerCircunference->at(i);
    Point outerPoint1 = outerCircunference->at(i);
    Point innerPoint2 = innerCircunference->at(nextPoint);
    Point outerPoint2 = outerCircunference->at(nextPoint);

    lines->push_back(innerPoint1);
    lines->push_back(innerPoint2);
    lines->push_back(outerPoint1);

    lines->push_back(innerPoint2);
    lines->push_back(outerPoint2);
    lines->push_back(outerPoint1);
  }

  return lines;
}

deque<Point>* circle(int slices, float radius, float z, int stacks) {
  float circunferenceRadius = radius/stacks;

  deque<Point>* circle = innerCircle(slices, circunferenceRadius, z);

  for(int i = 1; i < stacks; i ++) {
    deque<Point>* innerCircunference = circunference(slices, i * circunferenceRadius, z);
    deque<Point>* outerCircunference = circunference(slices, (i + 1) * circunferenceRadius, z);

    circle = triangulateLines(innerCircunference, outerCircunference, circle);
  }

  return circle;
}

int torus(int argc, char ** parameters) {
  deque<Point>* torus = new deque<Point>();
  float sliceHeight, radiusDeviation, stackRadius;
  deque< deque<Point>* > borders;

  if (argc != 5) { return -1; }
  else {
    // Parse base radius, innerRadius, sides and rings.
    float radius      = stof(parameters[0]);
    float innerRadius = stof(parameters[1]);
    int sides         = stoi(parameters[2]);
    int rings         = stoi(parameters[3]);

  for(int side = 0; side < sides; side ++) {
    float angle = sliceAngle(side, sides);
    sliceHeight = innerRadius * sin(angle);
    radiusDeviation = innerRadius * cos(angle);
    stackRadius = radius + radiusDeviation;

    borders.push_back(circunference(rings, stackRadius, sliceHeight));
  }

  /* Triangulate the sides of the torus */
  for(int side = 0; side < sides; side++) {
    int nextLine = (side +1) % sides;
    torus = triangulateLines(borders.at(side), borders.at(nextLine), torus);
  }

  // Open/Create file.
  char filename[100];
  sprintf(filename, "%s.3d", parameters[4]);
  FILE * file = fopen(filename, "w+");

  for (int i = 0; i < torus->size(); i++) {
    Point p = torus->at(i);

    fprintf(file, "%f %f %f\n", p.getX(), p.getY(), p.getZ());
  }
  }
  return 0;
}

/* Write a list of ordered vertices with the format "x y z" to the
 * specified file. */
void writeVerticesToFile(char ** vertices, int num, FILE * file) {
  for (int i = 0; i < num; i++)
    fprintf(file, "%s\n", vertices[i]);
}

int bezier(int argc, char ** parameters){
  string aux, token;
  char aux2[1000000], * tab;
  int nr, patch[16], * patches;
  float cp[3];
  float * cp2;
  // Check number of arguments.
  if (argc < 2) { return -1; }

  // Test if the file exists
  ifstream file(parameters[0]);
  if(!file.good()) {return -1;}
  //Number of patches
  getline(file,aux);
  nr = stoi(aux);
  patchPoints.setNrPatches(nr);

  //Add patches to patchPoints
  for(int j = 0; j < nr; j++){  
    getline(file,aux);
    strncpy(aux2, aux.c_str(), sizeof(aux2));
    tab = strtok(aux2,",");
    patch[0] = stoi(tab);
    for(int i = 1; i < 16; i++){
      tab = strtok (NULL, " ,.-");
      patch[i] = stoi(tab);
    }
    patchPoints.addPatch(patch);
  }

  //Add control point to patchPoints
  getline(file,aux);
  nr = stoi(aux);
  patchPoints.setNrCP(nr);
  for(int i = 0; i < nr; i++){
    getline(file,aux);
    std::istringstream iss (aux);
    for(int j=0;std::getline(iss, token,',');j++) {
      strncpy(aux2, token.c_str(), sizeof(aux2));
      tab = strtok(aux2,",");
      cp[j] = atof(tab);
    }
    patchPoints.addCP(cp);
  }

  renderBezierCurve(parameters[1],parameters[2]);

  return 0;
}

float getBezierPoint(float u, float v, float m[4][4] , float p[4][4]) {
  float pointValue = 0;
  float aux[4], aux2[4];
  
  //bu*M
  for(int i = 0; i<4; i++){
    aux[i] = (powf(u,3.0)*m[0][i]) + (powf(u,2.0)*m[1][i]) + (u*m[2][i]) + m[3][i];
  }


  //(bu*M)*P
  for(int i = 0; i<4; i++){
    aux2[i] = (aux[0]*p[0][i]) + (aux[1]*p[1][i]) + (aux[2]*p[2][i]) + (aux[3]*p[3][i]);
  }

  //((bu*M)*P)*MT
  for(int i = 0; i<4; i++){
    aux[i] = (aux2[0]*m[0][i]) + (aux2[1]*m[1][i]) + (aux2[2]*m[2][i]) + (aux2[3]*m[3][i]);
  }  

  //(((bu*M)*P)*MT)*bv
  pointValue = aux[0] * powf(v,3.0);
  pointValue += aux[1] * powf(v,2.0);
  pointValue += aux[2] * v;
  pointValue += aux[3];

  return pointValue; 
}

void renderBezierCurve(char * tessellation, char * newFile){
  int * patchIndice, aux;
  float * ma[16], mT[3][16], px[4][4], py[4][4], pz[4][4], res[3];
  char filename[100];
  float u, v, level = (float)1/atoi(tessellation);
  //Create new file .3d
  sprintf(filename, "%s.3d", newFile);
  FILE * file = fopen(filename, "w+");
  for (int patch = 0; patch < patchPoints.getNrPatches(); patch++) {
    aux = 0;
    patchIndice = patchPoints.getPatch(patch);
    //Fill matrix ma with the control points of the patch
    for(int i = 0; i < 16; i++) {
      ma[i] = patchPoints.getCP(patchIndice[i]);
    }
    
    //Matrix with all the Pix, Piy, Piz
    for(int i = 0; i < 4; i++){
      for(int j = 0; j < 4; j++, aux++){
        px[i][j] = ma[aux][0];
        py[i][j] = ma[aux][1];
        pz[i][j] = ma[aux][2];
      }
    }

    //Matrix M 
    float m[4][4] = { {-1, 3, -3, 1},
                    {3, -6, 3, 0 },
                    {-3, 3, 0, 0},
                    {1, 0, 0, 0}
                  };

    //Getting the points
    for(u = 0.0f; u<1 ; u += level){
      for(v = 0.0f; v<1 ; v += level){
          res[0] = getBezierPoint(u, v, m, px);
          res[1] = getBezierPoint(u, v, m, py);
          res[2] = getBezierPoint(u, v, m, pz);
          fprintf(file, "%f %f %f\n", res[0],res[1],res[2]);
          
          res[0] = getBezierPoint (u+level,v+level, m, px);
          res[1] = getBezierPoint (u+level,v+level, m, py);
          res[2] = getBezierPoint (u+level,v+level, m, pz);
          fprintf(file, "%f %f %f\n", res[0],res[1],res[2]);
        
          res[0] = getBezierPoint (u+level,v, m, px);
          res[1] = getBezierPoint (u+level,v, m, py);
          res[2] = getBezierPoint (u+level,v, m, pz);
          fprintf(file, "%f %f %f\n", res[0],res[1],res[2]);

          res[0] = getBezierPoint(u, v, m, px);
          res[1] = getBezierPoint(u, v, m, py);
          res[2] = getBezierPoint(u, v, m, pz);
          fprintf(file, "%f %f %f\n", res[0],res[1],res[2]);

          res[0] = getBezierPoint (u,v+level, m, px);
          res[1] = getBezierPoint (u,v+level, m, py);
          res[2] = getBezierPoint (u,v+level, m, pz);
          fprintf(file, "%f %f %f\n", res[0],res[1],res[2]);

          res[0] = getBezierPoint (u+level,v+level, m, px);
          res[1] = getBezierPoint (u+level,v+level, m, py);
          res[2] = getBezierPoint (u+level,v+level, m, pz);
          fprintf(file, "%f %f %f\n", res[0],res[1],res[2]);
          
      }
    }     
  }
}
