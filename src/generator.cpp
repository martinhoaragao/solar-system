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
deque<Point>* derivada;

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
void bezierTangent(char * tessellation);
deque<Point>* circunference(int, float, float);
deque<Point>* circunferenceHorizontal(int, float, float);
float sliceAngle(int, int);
deque<Point>* triangulateLines(deque<Point>*, deque<Point>*, deque<Point>*);

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
    char path[100] = "shapes/";
    sprintf(filename, "%s.3d", parameters[1]);
    FILE * file     = fopen(strcat(path,filename), "w+");

    // write to file.
    fprintf(file, "6\n");
    fprintf(file, "%f 0.0 %f\n", -d, -d);
    fprintf(file, "%f 0.0 %f\n", -d,  d);
    fprintf(file, "%f 0.0 %f\n",  d,  d);
    fprintf(file, "%f 0.0 %f\n",  d, -d);
    fprintf(file, "%f 0.0 %f\n", -d, -d);
    fprintf(file, "%f 0.0 %f\n",  d,  d);

    //normals

    fprintf(file, "6\n");
    fprintf(file, "0 1.0 0\n");
    fprintf(file, "0 1.0 0\n");
    fprintf(file, "0 1.0 0\n");
    fprintf(file, "0 1.0 0\n");
    fprintf(file, "0 1.0 0\n");


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
    char AA[50], BB[50], CC[50], DD[50], EE[50], FF[50];
    sprintf(A, "%f %f %f", -x, y, -z);
    sprintf(B, "%f %f %f",  x, y, -z);
    sprintf(C, "%f %f %f", -x, y,  z);
    sprintf(D, "%f %f %f",  x, y,  z);
    sprintf(E, "%f 0.0 %f", -x, -z);
    sprintf(F, "%f 0.0 %f",  x, -z);
    sprintf(G, "%f 0.0 %f", -x,  z);
    sprintf(H, "%f 0.0 %f",  x,  z);
    sprintf(AA, "0 0 1");
    sprintf(BB, "0 0 -1");
    sprintf(CC, "-1 0 0");
    sprintf(DD, "1 0 0");
    sprintf(EE, "0 1 0");
    sprintf(FF, "0 -1 0");
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
    char path[100] = "shapes/";
    sprintf(filename, "%s.3d", parameters[3]);
    FILE * file     = fopen(strcat(path,filename), "w+");
    fprintf(file, "%d\n",36);

    // Write vertices to file.
    writeVerticesToFile(vertices, 36, file);
    fprintf(file, "36\n");
    fprintf(file, "%s\n",AA);
    fprintf(file, "%s\n",AA);
    fprintf(file, "%s\n",AA);
    fprintf(file, "%s\n",AA);
    fprintf(file, "%s\n",AA);
    fprintf(file, "%s\n",AA);
    fprintf(file, "%s\n",DD);
    fprintf(file, "%s\n",DD);
    fprintf(file, "%s\n",DD);
    fprintf(file, "%s\n",DD);
    fprintf(file, "%s\n",DD);
    fprintf(file, "%s\n",DD);
    fprintf(file, "%s\n",BB);
    fprintf(file, "%s\n",BB);
    fprintf(file, "%s\n",BB);
    fprintf(file, "%s\n",BB);
    fprintf(file, "%s\n",BB);
    fprintf(file, "%s\n",BB);
    fprintf(file, "%s\n",FF);
    fprintf(file, "%s\n",FF);
    fprintf(file, "%s\n",FF);
    fprintf(file, "%s\n",FF);
    fprintf(file, "%s\n",FF);
    fprintf(file, "%s\n",FF);
    fprintf(file, "%s\n",EE);
    fprintf(file, "%s\n",EE);
    fprintf(file, "%s\n",EE);
    fprintf(file, "%s\n",EE);
    fprintf(file, "%s\n",EE);
    fprintf(file, "%s\n",EE);
    fprintf(file, "%s\n",CC);
    fprintf(file, "%s\n",CC);
    fprintf(file, "%s\n",CC);
    fprintf(file, "%s\n",CC);
    fprintf(file, "%s\n",CC);
    fprintf(file, "%s\n",CC);
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
    deque< deque<Point>* > sides;
    deque<Point>* sphere = new deque<Point>();

    // Open/Create file and write number of vertices to the file.
    char filename[100];
    char path[100] = "shapes/";
    sprintf(filename, "%s.3d", parameters[3]);
    FILE * file     = fopen(strcat(path,filename), "w+");

    for (int stack = 0; stack < stacks; stack++) {
      float angle = (sliceAngle(stack, stacks - 1) / 2) - (M_PI/2);
      float stackHeight = radius * sin(angle);
      float stackRadius = radius * cos(angle);

      sides.push_back(circunferenceHorizontal(slices, stackRadius, stackHeight));
    }

    /* Triangulate the sides of the sphere */
    for(int stack = 0; stack < stacks; stack++) {
      int nextLine = (stack +1) % stacks;
      sphere = triangulateLines(sides.at(stack), sides.at(nextLine), sphere);
    }

    fprintf(file, "%d\n",(stacks)*(slices)*6 );
    for (int i = 0; i < sphere->size(); i++) {
      Point p = sphere->at(i);

      fprintf(file, "%.10f %.10f %.10f\n", p.getX(), p.getY(), p.getZ());
    }

    fprintf(file, "%d\n",(stacks)*(slices)*6 );
    for (int i = 0; i < sphere->size(); i++) {
      Point p = sphere->at(i);

      fprintf(file, "%.10f %.10f %.10f\n", p.getX(), p.getY(), p.getZ());
    }

    fprintf(file, "%d\n",(stacks)*(slices)*6 );
    for (int i = 0; i < sphere->size(); i+=3) {
      double normalizedX, normalizedY, normalizedX2, normalizedY2, normalizedX3, normalizedY3;

      Point p = sphere->at(i);
      normalizedX = (-atan2(-p.getX(), p.getZ())  + M_PI) / (2*M_PI);
      normalizedY = ((-(p.getY()/radius)) + 1) / 2.0;


      p = sphere->at(i+1);
      normalizedX2 = (-atan2(-p.getX(), p.getZ())  + M_PI) / (2*M_PI);
      normalizedY2 = ((-(p.getY()/radius)) + 1) / 2.0;

      p = sphere->at(i+2);
      normalizedX3 = (-atan2(-p.getX(), p.getZ())  + M_PI) / (2*M_PI);
      normalizedY3 = ((-(p.getY()/radius)) + 1) / 2.0;

      if(normalizedX > normalizedX2 && fabs(normalizedX - normalizedX2) > 0.8)
        normalizedX2 = 1.0;
      if(normalizedX > normalizedX3 && fabs(normalizedX - normalizedX3) > 0.8)
        normalizedX3 = 1.0;
      if (normalizedX2 > normalizedX && fabs(normalizedX2 - normalizedX) > 0.8)
        normalizedX = 1.0;
      if (normalizedX2 > normalizedX3 && fabs(normalizedX2 - normalizedX3) > 0.8)
        normalizedX3 = 1.0;
      if (normalizedX3 > normalizedX && fabs(normalizedX3 - normalizedX) > 0.8)
        normalizedX = 1.0;
      if (normalizedX3 > normalizedX2 && fabs(normalizedX3 - normalizedX2) > 0.8)
        normalizedX2 = 1.0;

      fprintf(file, "%.10f %.10f\n", normalizedX, normalizedY);
      fprintf(file, "%.10f %.10f\n", normalizedX2, normalizedY2);
      fprintf(file, "%.10f %.10f\n", normalizedX3, normalizedY3);
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
    float static radius  = stof(parameters[0]);
    float static height  = stof(parameters[1]);
    int static slices    = stoi(parameters[2]);
    int static stacks    = stoi(parameters[3]);
    float angle   = 0.0;
    float inc     = (2 * M_PI)/slices;
    float h       = 0.0;
    float hInc    = (height)/stacks;
    float r       = ((height - h) * radius)/height;

    // Open/Create file.
    char filename[100];
    char path[100] = "shapes/";
    sprintf(filename, "%s.3d", parameters[4]);
    FILE * file     = fopen(strcat(path,filename), "w+");

    fprintf(file,"%d\n",slices*3+stacks*slices*6);
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
    //normals
    h = 0.0f;
    fprintf(file,"%d\n",slices*3+stacks*slices*6);
    for (int slice = 0; slice < slices; slice++, angle += inc) {
      Point p = Point(r * sin(angle), height/sqrtf((height * height) + (radius * radius)), r * cos(angle));

      fprintf(file, "0.0 0.0 0.0\n");
      fprintf(file, "%f 0.0 %f\n", r * sin(angle + inc), r * cos(angle + inc));
      fprintf(file, "%f 0.0 %f\n", p.getX(), p.getZ());
    }

    h += hInc;

    for (int stack = 0; stack < stacks; stack++, h += hInc) {
      r     = ((height - h)) / height;
      angle = 0.0;

      for (int slice = 0; slice < slices; slice++, angle += inc) {
        Point p = Point(r * sin(angle), height/sqrtf((height * height) + (radius * radius)), r * cos(angle));

        fprintf(file, "%f %f %f\n", p.getX(), p.getY(), p.getZ());
        fprintf(file, "%f %f %f\n",
            (((height - (h - hInc)))/height) * sin(angle),
            height/sqrtf((height * height) + (radius * radius)),
            (((height - (h - hInc)))/height) * cos(angle));
        fprintf(file, "%f %f %f\n",
            (((height - (h - hInc)))/height) * sin(angle + inc),
            height/sqrtf((height * height) + (radius * radius)),
            (((height - (h - hInc)))/height) * cos(angle + inc));

        fprintf(file, "%f %f %f\n", p.getX(), p.getY(), p.getZ());
        fprintf(file, "%f %f %f\n",
            r * sin(angle - inc),
            height/sqrtf((height * height) + (radius * radius)),
            r * cos(angle - inc));
        fprintf(file, "%f %f %f\n",
            (((height - (h - hInc)))/height) * sin(angle),
            height/sqrtf((height * height) + (radius * radius)),
            (((height - (h - hInc)))/height) * cos(angle));
      }
    }



  }

  return 0;
}

float sliceAngle(int slice, int slices) {
  return slice * ((2 * M_PI) / slices);
}

deque<Point>* circunferenceNormals(int slices, float z){
  deque<Point>* circunference = new deque<Point>();
  float x, y, angle;
  Point origin = Point(0,0,0);

  for(int i = 0; i < slices; i++){
    angle = sliceAngle(i, slices);
    x = sin(angle);
    y = cos(angle);
    Point point = Point(x, y, z);
    circunference->push_back(point);
  }

  return circunference;
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

deque<Point>* circunferenceHorizontal(int slices, float radius, float y) {
  deque<Point>* circunference = new deque<Point>();
  float x, z, angle;
  Point origin = Point(0, 0, 0);

  for(int i = 0; i < slices; i++) {
    angle = sliceAngle(i, slices);
    x = radius * sin(angle);
    z = radius * cos(angle);
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
  deque<Point>* normals = new deque<Point>();
  float sliceHeight, radiusDeviation, stackRadius;
  deque< deque<Point>* > borders;
  deque< deque<Point>* > normalsBorders;

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
    normalsBorders.push_back(circunferenceNormals(rings,sliceHeight));
  }

  /* Triangulate the sides of the torus */
  for(int side = 0; side < sides; side++) {
    int nextLine = (side +1) % sides;
    torus = triangulateLines(borders.at(nextLine), borders.at(side), torus);
    normals = triangulateLines(normalsBorders.at(nextLine), normalsBorders.at(side),normals);
  }

  // Open/Create file.
  char filename[100];
  char path[100] = "shapes/";
  sprintf(filename, "%s.3d", parameters[4]);
  FILE * file     = fopen(strcat(path,filename), "w+");
  int size = torus->size();
  fprintf(file,"%d\n",size);
  for (int i = 0; i < torus->size(); i++) {
    Point p = torus->at(i);

    fprintf(file, "%f %f %f\n", p.getX(), p.getY(), p.getZ());
  }
  size = normals->size();
  fprintf(file,"%d\n",size);
  for (int i = 0; i < normals->size(); i++) {
    Point p = normals->at(i);

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
  char aux2[1000000], * tab, fileName[1000] = "shapes/";
  int nr, patch[16], * patches;
  float cp[3];
  float * cp2;
  deque<Point> * derivada;
  // Check number of arguments.
  if (argc < 2) { return -1; }

  // Test if the file exists
  ifstream file(strcat(fileName,parameters[0]));
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

  bezierTangent(parameters[1]);
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
  float u, v, level = (float)1/atoi(tessellation);
  //Create new file .3d
  char filename[100];
  char path[100] = "shapes/";
  sprintf(filename, "%s.3d", newFile);
  FILE * file     = fopen(strcat(path,filename), "w+");
  float size = patchPoints.getNrPatches() * 6 * (1/level) * (1/level);
  fprintf(file, "%f\n",size );
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
      for(v = 0.0f; v < 1 ; v += level){
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
  fprintf(file, "%f\n",size );
  for(int i = 0; i < size;i++){
    Point p = derivada->at(i);

    fprintf(file, "%f %f %f\n", p.getX(), p.getY(), p.getZ());
  }
}

float getBezierPointTangent(float u, float v, float m[4][4] , float p[4][4], int derivada) {
  float pointValue = 0;
  float aux[4], aux2[4];

  //derivada de u
  if(derivada == 0){
    //bu*M
    for(int i = 0; i<4; i++){
      aux[i] = (3 * powf(u,2.0)*m[0][i]) + (2*u*m[1][i]) + (1*m[2][i]);
    }
  } else {
    for(int i = 0; i<4; i++){
      aux[i] = (powf(u,3.0)*m[0][i]) + (powf(u,2.0)*m[1][i]) + (u*m[2][i]) + m[3][i];
    }
  }

  //(bu*M)*P
  for(int i = 0; i<4; i++){
    aux2[i] = (aux[0]*p[0][i]) + (aux[1]*p[1][i]) + (aux[2]*p[2][i]) + (aux[3]*p[3][i]);
  }

  //((bu*M)*P)*MT
  for(int i = 0; i<4; i++){
    aux[i] = (aux2[0]*m[0][i]) + (aux2[1]*m[1][i]) + (aux2[2]*m[2][i]) + (aux2[3]*m[3][i]);
  }

  if(derivada == 0) {
    //(((bu*M)*P)*MT)*bv
    pointValue = aux[0] * powf(v,3.0);
    pointValue += aux[1] * powf(v,2.0);
    pointValue += aux[2] * v;
    pointValue += aux[3];
  //derivada de v
  } else {
    //(((bu*M)*P)*MT)*bv
    pointValue = aux[0] * (3 * powf(v,2.0));
    pointValue += aux[1] * (2 * v);
    pointValue += aux[2];
  }

  return pointValue;
}

void cross(float *a, float *b, float *res) {
  res[0] = a[1]*b[2] - a[2]*b[1];
  res[1] = a[2]*b[0] - a[0]*b[2];
  res[2] = a[0]*b[1] - a[1]*b[0];
}

void normalize(float *a) {
  float l = sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
  if(l!=0) {
    a[0] = a[0]/l;
    a[1] = a[1]/l;
    a[2] = a[2]/l;
  } else {
    a[0] = 0;
    a[1] = 0;
    a[2] = 0;
  }
}

void bezierTangent(char * tessellation){
  derivada = new deque<Point>();
  int * patchIndice, aux;
  float * ma[16], mT[3][16], px[4][4], py[4][4], pz[4][4], resU[3], resV[3], res[3];
  float u, v, level = (float)1/atoi(tessellation);

  for(int patch = 0; patch < patchPoints.getNrPatches(); patch++){ //first for
    aux = 0;
    patchIndice = patchPoints.getPatch(patch);
    //Fill matrix ma with the control points of the patch
    for(int i = 0; i < 16; i++) { //second for
      ma[i] = patchPoints.getCP(patchIndice[i]);
    }//end of second for

    //Matrix with all the Pix, Piy, Piz
    for(int i = 0; i < 4; i++){ //third
      for(int j = 0; j < 4; j++, aux++){ //fourth
        px[i][j] = ma[aux][0];
        py[i][j] = ma[aux][1];
        pz[i][j] = ma[aux][2];
      }//fourth
    } //third

      //Matrix M
    float m[4][4] = { {-1, 3, -3, 1},
                    {3, -6, 3, 0 },
                    {-3, 3, 0, 0},
                    {1, 0, 0, 0}
                  };

    for(u = 0.0f; u<1 ; u += level){ //fifth
      for(v = 0.0f; v<1 ; v += level){ //sixth
          resU[0] = getBezierPointTangent(u, v, m, px,0);
          resU[1] = getBezierPointTangent(u, v, m, py,0);
          resU[2] = getBezierPointTangent(u, v, m, pz,0);
          normalize(resU);
          resV[0] = getBezierPointTangent(u, v, m, px,1);
          resV[1] = getBezierPointTangent(u, v, m, py,1);
          resV[2] = getBezierPointTangent(u, v, m, pz,1);
          normalize(resV);
          cross(resV,resU,res);
          derivada->push_back(Point(res[0],res[1], res[2]));

          resU[0] = getBezierPointTangent (u+level,v+level, m, px,0);
          resU[1] = getBezierPointTangent (u+level,v+level, m, py,0);
          resU[2] = getBezierPointTangent (u+level,v+level, m, pz,0);
          normalize(resU);
          resV[0] = getBezierPointTangent (u+level,v+level, m, px,1);
          resV[1] = getBezierPointTangent (u+level,v+level, m, py,1);
          resV[2] = getBezierPointTangent (u+level,v+level, m, pz,1);
          normalize(resV);
          cross(resV,resU,res);
          derivada->push_back(Point(res[0],res[1], res[2]));

          resU[0] = getBezierPointTangent (u+level,v, m, px,0);
          resU[1] = getBezierPointTangent (u+level,v, m, py,0);
          resU[2] = getBezierPointTangent (u+level,v, m, pz,0);
          normalize(resU);
          resV[0] = getBezierPointTangent (u+level,v, m, px,1);
          resV[1] = getBezierPointTangent (u+level,v, m, py,1);
          resV[2] = getBezierPointTangent (u+level,v, m, pz,1);
          normalize(resV);
          cross(resV,resU,res);
          derivada->push_back(Point(res[0],res[1], res[2]));

          resU[0] = getBezierPointTangent(u, v, m, px,0);
          resU[1] = getBezierPointTangent(u, v, m, py,0);
          resU[2] = getBezierPointTangent(u, v, m, pz,0);
          normalize(resU);
          resV[0] = getBezierPointTangent(u, v, m, px,1);
          resV[1] = getBezierPointTangent(u, v, m, py,1);
          resV[2] = getBezierPointTangent(u, v, m, pz,1);
          normalize(resV);
          cross(resV,resU,res);
          derivada->push_back(Point(res[0],res[1], res[2]));

          resU[0] = getBezierPointTangent (u,v+level, m, px,0);
          resU[1] = getBezierPointTangent (u,v+level, m, py,0);
          resU[2] = getBezierPointTangent (u,v+level, m, pz,0);
          normalize(resU);
          resV[0] = getBezierPointTangent (u,v+level, m, px,1);
          resV[1] = getBezierPointTangent (u,v+level, m, py,1);
          resV[2] = getBezierPointTangent (u,v+level, m, pz,1);
          normalize(resV);
          cross(resV,resU,res);
          derivada->push_back(Point(res[0],res[1], res[2]));

          resU[0] = getBezierPointTangent (u+level,v+level, m, px,0);
          resU[1] = getBezierPointTangent (u+level,v+level, m, py,0);
          resU[2] = getBezierPointTangent (u+level,v+level, m, pz,0);
          normalize(resU);
          resV[0] = getBezierPointTangent (u+level,v+level, m, px,1);
          resV[1] = getBezierPointTangent (u+level,v+level, m, py,1);
          resV[2] = getBezierPointTangent (u+level,v+level, m, pz,1);
          normalize(resV);
          cross(resV,resU,res);
          derivada->push_back(Point(res[0],res[1], res[2]));
      }//sixth
    }//fifth
  }//end of first for
} //end of bezierTangent
