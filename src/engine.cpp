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

#include "group.h"

using namespace std;

// Camera variables and coordinates.
float alpha     = (M_PI/2.0), beta  = 0.0, r = 550.0;
float fastZoom  = 1.0, slowZoom     = 0.5;
float camX, camY, camZ;
int extraSpeed  = 0;

int menuID;
float timebase = 0;
int frame = 0;

Group *group;

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

/* Calculate the camera coordinates. */
void calculateCamCoordinates() {
  camX = r * cos(beta) * sin(alpha);
  camY = r * sin(beta);
  camZ = r * cos(beta) * cos(alpha);
}

/* Move camera position when keyboard arrows are pressed. */
void arrowPressed(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_RIGHT:
      alpha -= 0.1;
      break;
    case GLUT_KEY_LEFT:
      alpha += 0.1;
      break;
    case GLUT_KEY_UP:
      beta += 0.1f;
      if (beta > 1.5f) beta = 1.5f;
      break;
    case GLUT_KEY_DOWN:
      beta -= 0.1f;
      if (beta < -1.5f) beta = -1.5f;
      break;
    default: break;
  }

  calculateCamCoordinates();
  glutPostRedisplay();
}

/*-----------------------------------------------------------------------------------
	Parser.
-----------------------------------------------------------------------------------*/


void renderScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // set the camera
  glLoadIdentity();
  gluLookAt(camX,camY,camZ,
      0.0,0.0,0.0,
      0.0,1.0,0.0);

  group->draw();

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
  switch(key) {
    // 'f'.
    case 102: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
    // 'F'.
    case 70: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
    // 'l'.
    case 108: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
    // 'L'.
    case 76: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
    // 'p'.
    case 112: glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;
    // 'P'.
    case 80: glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;
    // 'b'.
    case 98: glColor3f(0, 0, 1); break;
    // 'B'.
    case 66: glColor3f(0, 0, 1); break;
    // 'g'.
    case 103: glColor3f(0, 1, 0); break;
    // 'G'.
    case 71: glColor3f(0, 1, 0); break;
    // 'r'.
    case 114: glColor3f(1, 0, 0); break;
    // 'R'.
    case 82: glColor3f(1, 0, 0); break;
    // 'q'.
    case 113: group = new Group(); break;
    // 'Q'.
    case 81: group = new Group(); break;
    // 'w'.
    case 119: // Faster Zoom In.
      r -= fastZoom * pow(5.0, extraSpeed); if (r < 0.0) r = 0.0;
      calculateCamCoordinates(); break;
    // 'W'.
    case 87:  // Slower Zoom In.
      r -= slowZoom * pow(5.0, extraSpeed); if (r < 0.0) r = 0.0;
      calculateCamCoordinates(); break;
    // 's'.
    case 115: // Faster Zoom Out.
      r += fastZoom * pow(5.0, extraSpeed);
      calculateCamCoordinates(); break;
    // 'S'.
    case 83:  // Slower Zoom Out.
      r += slowZoom * pow(5.0, extraSpeed);
      calculateCamCoordinates(); break;
    // Space ' '.
    case 32:
      extraSpeed == 1 ? extraSpeed = 0 : extraSpeed = 1;
      break;
    default: break;
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
  group = new Group();

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

  // Calculate initial camera coordinates before starting to render.
  calculateCamCoordinates();

  // enter GLUT's main cycle
  glutMainLoop();

  return 1;
}
