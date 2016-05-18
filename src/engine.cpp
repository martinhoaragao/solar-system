// Include GLUT header based on operating system.
#ifdef __APPLE__
  #include <GLUT/glut.h>
#elif _WIN32
  #include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#define DEFAULT_CAM_RADIUS  10.0f
#include <math.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "group.h"

using namespace std;


int menuID;
float timebase = 0;
int frame = 0;

// Camera variables
bool freeCamera = false;
float defAlpha = 0, defBeta = 0, defRadius = DEFAULT_CAM_RADIUS;
float alpha = 0, freeAlpha = 0;
float beta = 0, freeBeta = 0;
float radius = DEFAULT_CAM_RADIUS;
float freeCamSpeed = 0.5;
float px = 0.0f;
float py = 0.0f;
float pz = 0.0f;
float rx = 1.0f;
float ry = 0.0f;
float rz = 0.0f;
int xOri = -1;
int yOri = -1;


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

void set_camera(float a, float b, float r) {
  defAlpha = (a * M_PI) / 180;
  defBeta = (b * M_PI) / 180;
    defRadius = r < 1 ? DEFAULT_CAM_RADIUS : r;
  alpha = (a * M_PI) / 180;
  beta = (b * M_PI) / 180;
  radius = defRadius;
}



/*-----------------------------------------------------------------------------------
	Parser.
-----------------------------------------------------------------------------------*/


void renderScene() {
  // set the camera
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
if (freeCamera){
gluLookAt(  px, py, pz,
          px + rx, py + ry, pz + rz,
          0.0f, 1.0f, 0.0f);
}
else{
gluLookAt(  radius*cos(beta)*sin(alpha), radius*sin(beta), radius*cos(beta)*cos(alpha),
          0.0f, 0.0f, 0.0f,
          0.0f, 1.0f, 0.0f);
  }

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


// Mouse button callback
static void mousePress(int button, int state, int x, int y) {
// Camera only moves while the left mouse button is pressed
if (button == GLUT_LEFT_BUTTON) {
// If the button is released, origin coordinates reset.
if (state == GLUT_UP) {
      xOri = -1;
      yOri = -1;
    }
// Else, button is pressed, origin coordinates are updated.
else  {
      xOri = x;
      yOri = y;
    }
  }
}

// Motion while mouse button is pressed
static void mouseMotion(int x, int y) {
if (xOri >= 0) {
int xDiff = x - xOri;
int yDiff = y - yOri;
if (freeCamera){
      freeAlpha -= xDiff * 0.01f;
      freeBeta -= yDiff * 0.01f;
if (freeBeta > (M_PI / 2) - 0.001) freeBeta = (M_PI / 2) - 0.001;
if (freeBeta < -(M_PI / 2) + 0.001) freeBeta = -(M_PI / 2) + 0.001;
      rx = cos(freeBeta)*sin(freeAlpha);
      ry = sin(freeBeta);
      rz = cos(freeBeta)*cos(freeAlpha);
    }
else{
      alpha -= xDiff * 0.01f;
      beta -= yDiff * 0.01f;
if (beta > (M_PI / 2) - 0.001) beta = (M_PI / 2) - 0.001;
if (beta < -(M_PI / 2) + 0.001) beta = -(M_PI / 2) + 0.001;
    }
  }
  xOri = x;
  yOri = y;
}

// function to process keyboard events
void keyboardNormal(unsigned char key, int x, int y) {
  switch(key) {
    // 'f'.
    case 'f': glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
    // 'F'.
    case 'F': glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
    // 'l'.
    case 'l': glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
    // 'L'.
    case 'L': glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
    // 'p'.
    case 'p': glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;
    // 'P'.
    case 'P': glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;

    case 'q': group = new Group(); break;
    // 'Q'.
    case 'Q': group = new Group(); break;

    case 'n': freeCamera = !freeCamera;
    case 'm':{
              if (freeCamera){
                    px = 0.0f;
                    py = 0.0f;
                    pz = 0.0f;
                    freeAlpha = defAlpha;
                    freeBeta = defBeta;
                  }
              else if (!freeCamera){
                    alpha = defAlpha;
                    beta = defBeta;
                    radius = defRadius;
              }
    }
    case 'w': {
      if(freeCamera){
        pz += rz*freeCamSpeed;
        px += rx*freeCamSpeed;
        py += ry*freeCamSpeed;
      }
      break;
    }
    case 's': {
      if(freeCamera){
        pz -= rz*freeCamSpeed;
        px -= rx*freeCamSpeed;
        py -= ry*freeCamSpeed;
      }
      break;
    }
    case 'd': {
      if(freeCamera){
        pz += rx*freeCamSpeed;
        px -= rz*freeCamSpeed;
      }
      break;
    }
    case 'a': {
      if(freeCamera){
        pz -= rx*freeCamSpeed;
        px += rz*freeCamSpeed;
      }
      break;
    }
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

  //Camera
  glutMouseFunc(mousePress);
  glutMotionFunc(mouseMotion);

  //  OpenGL settings
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

  // Light
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH);
  // Refresh normals after scale
  glEnable(GL_NORMALIZE);

  // Menu definition
  menuID = glutCreateMenu(newMenu);
  glutAddMenuEntry("Turn on GL_FILL polygon mode",'f');
  glutAddMenuEntry("Turn on GL_LINE polygon mode",'l');
  glutAddMenuEntry("Turn on GL_POINT polygon mode",'p');
  glutAddMenuEntry("Change color to red",'r');
  glutAddMenuEntry("Change color to green",'g');
  glutAddMenuEntry("Change color to blue",'b');
  glutAttachMenu(GLUT_RIGHT_BUTTON);


  // enter GLUT's main cycle
  glutMainLoop();

  return 1;
}
