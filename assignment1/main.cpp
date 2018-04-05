// Athabasca University
// Unit 5 Assignment 1

#include "GL/glut.h"

void initialize(int argc, char * argv[]) {

  // set background color
  glClearColor(0.3, 0.3, 0.7, 0.0);

  // enable depth test
  glEnable(GL_DEPTH_TEST);
}

void drawPicture() {

  glColor3f(1.0, 1.0, 1.0);

  // wall 1
  glColor3f(0.3, 0.3, 0.3);
  glBegin( GL_POLYGON );
    glVertex3f(-8.0, -6.0, 0.0);
    glVertex3f(-8.0, 3.0, 0.0);
    glVertex3f(8.0, 3.0, 0.0);
    glVertex3f(8.0, -6.0, 0.0);
  glEnd();

  // wall 2
  glColor3f(0.4, 0.4, 0.4);
  glBegin( GL_POLYGON );
    glVertex3f(8.0, -6.0, 0.0);
    glVertex3f(8.0, 3.0, 0.0);
    glVertex3f(8.0, 3.0, 6.0);
    glVertex3f(8.0, -6.0, 6.0);
  glEnd();

  // floor
  glColor3f(0.5, 0.5, 0.2);
  glBegin( GL_POLYGON );
    glVertex3i(-8.0, -6.0, 6.0);
    glVertex3i(-8.0, -6.0, 0.0);
    glVertex3i(10.0, -6.0, 0.0);
    glVertex3i(10.0, -6.0, 6.0);
  glEnd();

  // frame 1
  glColor3f(0.2, 0.6, 0.2);
  glBegin( GL_POLYGON );
    glVertex3f(-5.5, -0.854, 0.05);
    glVertex3f(-5.5, 1.0, 0.05);
    glVertex3f(-2.5, 1.0, 0.05);
    glVertex3f(-2.5, -0.854, 0.05);
  glEnd();

  // frame 2
  glBegin( GL_POLYGON );
    glVertex3f(1.5, -0.854, 0.05);
    glVertex3f(1.5, 1.0, 0.05);
    glVertex3f(4.5, 1.0, 0.05);
    glVertex3f(4.5, -0.854, 0.05);
  glEnd();

  // frame 3
  glBegin( GL_POLYGON );
    glVertex3f(7.95, -2.0, 1.5);
    glVertex3f(7.95, 1.0, 1.5);
    glVertex3f(7.95, 1.0, 2.646);
    glVertex3f(7.95, -2.0, 2.646);
  glEnd();
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  gluLookAt(-3.0, 0.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  drawPicture();

  glutSwapBuffers();
}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 40.0);
  glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char * argv[])
{
  glutInit( &argc, argv );
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH) ;
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);

  int windowHandle = glutCreateWindow("Comp393 U5 Assignment 1");
  glutSetWindow(windowHandle);

  glutDisplayFunc( display );
  glutReshapeFunc( reshape );

  initialize(argc, argv);

  glutMainLoop();

  return 0;
}