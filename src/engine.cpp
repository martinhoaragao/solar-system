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
#include <fstream>
#include <iostream>

#include "group.h"

using namespace std;

// Camera variables and coordinates.
float alpha     = (M_PI/2.0), beta  = 0.0, r = 550.0;
float fastZoom  = 1.0, slowZoom     = 0.5;
float camAlpha  = -2.40;
float camX      = 200.0, camY = 0.0, camZ = 300.0;
float lookX     = 0.0, lookZ = 0.0;
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

/* Move camera position when keyboard arrows are pressed. */
void arrowPressed(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_RIGHT:
      camAlpha -= 0.1;
      break;
    case GLUT_KEY_LEFT:
      camAlpha += 0.1;
      break;
    case GLUT_KEY_UP:
      camY += 1;
      break;
    case GLUT_KEY_DOWN:
      camY -= 1;
      break;
    default: break;
  }

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
      camX + 300 * sin(camAlpha), camY, camZ + 300 * cos(camAlpha),
      0.0,1.0,0.0);

  float pos[4] = {0, 0, 0, 1};
  float amb[4] = {0.2, 0.2, 0.2, 1.0};
  glPushMatrix();
  glEnable( GL_LIGHT0 );
  glEnable( GL_LIGHTING );
  glPopMatrix();

  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  glLightfv(GL_LIGHT0, GL_AMBIENT, amb);

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
    // 'd'.
    case 100:
      camX -= cos(camAlpha);
      camZ += sin(camAlpha);
      break;
    // 'a'.
    case 97:
      camX += cos(camAlpha);
      camZ -= sin(camAlpha);
      break;
    // 'w'.
    case 119:
      camZ += cos(camAlpha);
      camX += sin(camAlpha);
      break;
    // 's'.
    case 115:
      camZ -= cos(camAlpha);
      camX -= sin(camAlpha);
      break;
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
  glutIdleFunc(renderScene);

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
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

  // Material.
  float red[4] = {0.8f, 0.2f, 0.2f, 1.0f};
  glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
  glMaterialf(GL_FRONT, GL_SHININESS, 128.0);

  // Keyboard callbacks.
  glutSpecialFunc(arrowPressed);

  // enter GLUT's main cycle
  glutMainLoop();

  return 1;
}
