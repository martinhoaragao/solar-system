#include <iostream> // Needed to write to files.
#include <fstream>  // Needed to write to files.
#include <stdio.h>
#include <string.h>

using namespace std;

/* Functions prototypes. */
int plane(int, char **);

int main (int argc, char ** argv) {

  // Check for arguments.
  // Only 'plane', 'box', 'sphere' and 'cone' are possible.
  if (argc > 1) {
    if (strcmp(argv[1], "plane") == 0) {
      plane(argc - 2, &argv[2]) != 0 ? printf("Error!\n") : printf("Done\n");
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
  if (argc != 2) { return 1; }
  else {
    // Get the square width.
    float width     = stof(parameters[0]);
    float d         = width/2.0;

    // Create/Open output file.
    char * fileName = parameters[1];
    FILE * file     = fopen(fileName, "w+");

    // Write to file.
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
