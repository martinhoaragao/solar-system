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
float defAlpha = 0, defBeta = 0, defRadius = DEFAULT_CAM_RADIUS;
float alpha = 0;
float beta = 0;
float radius = DEFAULT_CAM_RADIUS;
float camSpeed = 1.5;
float px = 0.0f;
float py = 0.0f;
float pz = 0.0f;
float rx = 1.0f;
float ry = 0.0f;
float rz = 0.0f;
int xOri = -1;
int yOri = -1;


char configFileName[1000];
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


void arrowPressed(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_UP:
      py += camSpeed;
      break;
    case GLUT_KEY_DOWN:
      py -= camSpeed;
      break;
    default: break;
  }
  glutPostRedisplay();
}


/*-----------------------------------------------------------------------------------
	Parser.
-----------------------------------------------------------------------------------*/


void renderScene() {
  // set the camera
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(  px, py, pz,
              px + rx, py + ry, pz + rz,
              0.0f, 1.0f, 0.0f);

  group->draw();

  displayFPS();

  // End of frame
  glutSwapBuffers();
}

static void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);


	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


// Mouse button callback
void mousePress(int button, int state, int x, int y) {
  // Camera only moves while the left mouse button is pressed
  if (button == GLUT_LEFT_BUTTON) {
    // If the button is released, origin coordinates reset.
    if (state == GLUT_UP) {
      xOri = -1;
      yOri = -1;
    }
    // Else, button is pressed, origin coordinates are updated.
    else {
      xOri = x;
      yOri = y;
    }
  }
}

// Motion while mouse button is pressed
void mouseMotion(int x, int y) {
  if (xOri >= 0) {
    int xDiff = x - xOri;
    int yDiff = y - yOri;
    alpha -= xDiff * 0.004f;
    beta -= yDiff * 0.004f;
    if (beta > (M_PI / 2) - 0.001) beta = (M_PI / 2) - 0.001;
    if (beta < -(M_PI / 2) + 0.001) beta = -(M_PI / 2) + 0.001;
    rx = cos(beta)*sin(alpha);
    ry = sin(beta);
    rz = cos(beta)*cos(alpha);
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

    case 'q': group = new Group(configFileName); break;
    // 'Q'.
    case 'Q': group = new Group(configFileName); break;
    case 'w': {
        pz += rz*camSpeed;
        px += rx*camSpeed;
        py += ry*camSpeed;
        break;
    }
    case 's': {
        pz -= rz*camSpeed;
        px -= rx*camSpeed;
        py -= ry*camSpeed;
        break;
    }
    case 'd': {
        pz += rx*camSpeed;
        px -= rz*camSpeed;
        break;
    }
    case 'a': {
        pz -= rx*camSpeed;
        px += rz*camSpeed;
        break;
    }
    case '-': if (camSpeed - 0.5 > 0) camSpeed -= 0.5; break;
    case '+': camSpeed += 0.5; break;
    case '1': pz=60; px=60; py=5; break;
    case '2': pz=115; px=115; py=5; break;
    case '3': pz=175; px=175; py=10; break;
    case '4': pz=235; px=235; py=5; break;
    case '5': pz=335; px=335; py=5; break;
    case '6': pz=465; px=465; py=5; break;
    case '7': pz=575; px=575; py=5; break;
    case '8': pz=725; px=725; py=5; break;
    case '9': pz=115; px=75; py=5; break;
    case '0': pz=0; px=0; py=0; break;
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

  if (argc > 1)
    strcpy(configFileName, argv[1]);
  else
    strcpy(configFileName, "config.xml");

  group = new Group(configFileName);

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
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  // Light
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH);
  // Refresh normals after scale
  glEnable(GL_NORMALIZE);

  // Textures
  glEnable(GL_TEXTURE_2D);

  // Menu definition
  menuID = glutCreateMenu(newMenu);
  glutAddMenuEntry("Turn on GL_FILL polygon mode",'f');
  glutAddMenuEntry("Turn on GL_LINE polygon mode",'l');
  glutAddMenuEntry("Turn on GL_POINT polygon mode",'p');
  glutAddMenuEntry("Change color to red",'r');
  glutAddMenuEntry("Change color to green",'g');
  glutAddMenuEntry("Change color to blue",'b');
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  // Keyboard callbacks.
  glutSpecialFunc(arrowPressed);

  // enter GLUT's main cycle
  glutMainLoop();

  return 1;
}
