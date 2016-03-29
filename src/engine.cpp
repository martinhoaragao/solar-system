// Include GLUT header based on operating system.
#ifdef __APPLE__
  #include <GLUT/glut.h>
#elif _WIN32
  #include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <queue>
#include <fstream>
#include <iostream>

#include "tinyxml2.h"
#include "point.h"

using namespace std;

// Global variables.
float alpha = 0.0,  beta = 0.0, r = 5.0;
float camX  = r * cos(beta) * cos(alpha),
      camY  = r * sin(beta),
      camZ  = r * cos(beta) * sin(alpha);
int menuID;
float timebase = 0;
int frame = 0;
int verticesCount = 0;

deque<string> fileNames;
GLuint buffers[100];
int bufferSize[100];


/*-----------------------------------------------------------------------------------
	Display FPS.
-----------------------------------------------------------------------------------*/

void displayFPS() {
  int   time;
  char  title[20];

  frame++;
  time = glutGet(GLUT_ELAPSED_TIME);
  if (time - timebase > 1000) {
    float fps = frame * 1000.0/(time - timebase);
    timebase  = time;
    frame     = 0;
    sprintf(title,"%.2f FPS",fps);
    glutSetWindowTitle(title);
  }
}

// Points to array
vector<float>* pointsToVector(queue<Point>* points) {
  vector<float>* floatVector = new vector<float>();

  while(points->size() > 0 ) {
    Point point = points->front();
    points->pop();
    floatVector->push_back(point.getX());
    floatVector->push_back(point.getY());
    floatVector->push_back(point.getZ());
  }

  return floatVector;
}


/* Move camera position when keyboard arrows are pressed. */
void arrowPressed(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_RIGHT:
      alpha += ((M_PI)/2.0)/36.0;
      camX  = r * cos(beta) * cos(alpha);
      camZ  = r * cos(beta) * sin(alpha);
      break;
    case GLUT_KEY_LEFT:
      alpha -= ((M_PI)/2.0)/36.0;
      camX  = r * cos(beta) * cos(alpha);
      camZ  = r * cos(beta) * sin(alpha);
      break;
    case GLUT_KEY_UP:
      if (beta <= (M_PI)/2.0) {
        beta  += ((M_PI)/2.0)/36.0;
        camX  = r * cos(beta) * cos(alpha);
        camY  = r * sin(beta);
        camZ  = r * cos(beta) * sin(alpha);
      }
      break;
    case GLUT_KEY_DOWN:
      if (beta >= (-M_PI)/2.0) {
        beta  -= ((M_PI)/2.0)/36.0;
        camX  = r * cos(beta) * cos(alpha);
        camY  = r * sin(beta);
        camZ  = r * cos(beta) * sin(alpha);
      }
      break;
    default: break;
  }

  glutPostRedisplay();
}

void extractFileNames (char* configFileName) {
  tinyxml2::XMLDocument doc;

  doc.LoadFile(configFileName);
  tinyxml2::XMLElement * elem = doc.FirstChildElement()->FirstChildElement();

  while(elem != NULL) {
    fileNames.push_back(elem->Attribute("file"));

    elem = elem->NextSiblingElement();
  }
}

queue<Point>* extractPoints(string fileName) {
  queue<Point>* points = new queue<Point>();
  ifstream pointsFile;
  string line;
  float x, y, z;

  pointsFile.open(fileName);

  while (getline(pointsFile, line)) {
    istringstream iss(line);
    verticesCount ++;

    iss >> x >> y >> z;
    Point point = Point(x,y,z);

    points->push(point);
  }

  pointsFile.close();
  return points;
}



void drawTrianglesFromFile() {
  char configFileName[] = "config.xml";
  extractFileNames(configFileName);

  // Generate Vertex Buffer Objects
  glGenBuffers(fileNames.size(), buffers);

  for(int i = 0; i < fileNames.size(); i++) {
    queue<Point>* points = extractPoints(fileNames.at(i));
    vector<float>* pointsVector = pointsToVector(points);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
    glBufferData(GL_ARRAY_BUFFER, pointsVector->size() * sizeof(float), &pointsVector->front(), GL_STATIC_DRAW);
    bufferSize[i] = pointsVector->size();

  }
}

void renderScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // set the camera
  glLoadIdentity();
  gluLookAt(camX,camY,camZ,
      0.0,0.0,0.0,
      0.0,1.0,0.0);


  for(int i = 0; i < fileNames.size(); i++) {
    glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);

    //Draw Triangle from VBO - do each time window, view point or data changes
    glVertexPointer(3, GL_FLOAT, 0, NULL);

    // Enable buffer
    glEnableClientState(GL_VERTEX_ARRAY);

    glDrawArrays(GL_TRIANGLES, 0, bufferSize[i]);

    //Force display to be drawn now
    glFlush();
    glDisableClientState(GL_VERTEX_ARRAY);
  }

  displayFPS();

  // End of frame
  glutSwapBuffers();
}

void changeSize(int width, int height) {

  // Prevent a divide by zero, when window is too short
  if(height == 0)
    height = 1;

  float windowAspectRatio = width / height;

  // Set the projection matrix as current
  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();

  // Set the viewport to be the entire window
  glViewport(0, 0, width, height);

  gluPerspective(45.0f, windowAspectRatio, 1.0f ,1000.0);

  // return to the model view matrix mode
  glMatrixMode(GL_MODELVIEW);
}

// function to process keyboard events
void keyboardNormal(unsigned char key, int x, int y) {
  if (key == 'f' || key == 'F') glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  else if (key == 'l' || key == 'L') glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  else if (key == 'p' || key == 'P') glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
  else if (key == 'b' || key == 'B') {
    glColor3f(0,0,1);
  }
  else if (key == 'g' || key == 'G') {
    glColor3f(0,1,0);
  }
  else if (key == 'r' || key == 'R') {
    glColor3f(1,0,0);
  }


  glutPostRedisplay();
}

// function to process menu events
void newMenu (int id_op){
        keyboardNormal((unsigned char)id_op, 0, 0);
}

int main (int argc, char** argv) {

  // init GLUT and the window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
  glutInitWindowPosition(100,100);
  glutInitWindowSize(800,800);
  glutCreateWindow("CG-first-phase");

  // Required callback registry
  drawTrianglesFromFile();
  glutDisplayFunc(renderScene);
  glutReshapeFunc(changeSize);

  // Menu callback
  glutKeyboardFunc(keyboardNormal);

  // Menu definition
  menuID = glutCreateMenu(newMenu);
  glutAddMenuEntry("Turn on GL_FILL polygon mode",'f');
  glutAddMenuEntry("Turn on GL_LINE polygon mode",'l');
  glutAddMenuEntry("Turn on GL_POINT polygon mode",'p');
  glutAddMenuEntry("Change color to red",'r');
  glutAddMenuEntry("Change color to green",'g');
  glutAddMenuEntry("Change color to blue",'b');
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  //  OpenGL settings
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Keyboard callbacks.
  glutSpecialFunc(arrowPressed);

  // enter GLUT's main cycle
  glutMainLoop();

  return 1;
}
