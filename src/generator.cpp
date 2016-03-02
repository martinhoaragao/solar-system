#include <iostream> // Needed to write to files.
#include <fstream>  // Needed to write to files.
#include <stdio.h>
#include <string.h>
#include "Vertex.h"

using namespace std;

/* Functions prototypes. */
int plane(int, char **);
int box(int, char **);
int sphere(int, char **);
void writeVerticesToFile(char **, int, FILE *);

int main (int argc, char ** argv) {
  // Check for arguments.
  // Only 'plane', 'box', 'sphere' and 'cone' are possible.
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
    else {
      printf("Invalid argument.\n");
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
    char * filename = parameters[1];
    FILE * file     = fopen(filename, "w+");

    // write to file.
    fprintf(file, "4\n");
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
      E, G, H, F, E, H, // Base.
      A, C, D, B, A, D, // Top.
      C, G, H, D, C, H, // Front.
      D, H, F, B, D, F, // Right.
      A, B, E, B, F, E, // Back.
      C, A, E, C, E, G  // Left.
    };

    // Open/Create file and write number of vertices to the file.
    FILE * file = fopen(parameters[3], "w+");
    fprintf(file, "%d\n", 8);

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
    float radius = stof(parameters[0]);
    int slices = stof(parameters[1]);
    int stacks = stof(parameters[2]);

    // Open/Create file and write number of vertices to the file.
    FILE * file = fopen(parameters[3], "w+");

  }
  return 0;
}

/* Write a list of ordered vertices with the format "x y z" to the
 * specified file. */
void writeVerticesToFile(char ** vertices, int num, FILE * file) {
  for (int i = 0; i < num; i++)
    fprintf(file, "%s\n", vertices[i]);
}
