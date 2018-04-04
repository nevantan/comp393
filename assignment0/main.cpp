/* Athabasca University

   Comp390 - Introduction to Computer Graphics

   Version 4

   Unit 1 Section 2 Objective 3

   First OpenGL program



   @author: Steve Leung

   @date: Dec 6, 2011

*/



#include <iostream>

#include <math.h>

#include <GL/glut.h>



//-------------------------------------------------------

  GLuint myList;



  typedef GLint vertex3i [3];

  vertex3i pt [8]  = { {0,0,0}, {0,1,0}, {1,0,0}, {1,1,0}, {0,0,1}, {0,1,1}, {1,0,1}, {1,1,1} };

  GLint clr_v[8];



  typedef GLfloat color3f [3];

  color3f colors [4] = { {1,0,0},   // red

            {1,1,0},    // yellow

            {0,1,0},    // green

            {0,0,1} };    // blue





//-------------------------------------------------------

void quad(GLint n1, GLint n2, GLint n3, GLint n4) {

  glBegin(GL_QUADS);

    glColor3fv (colors[clr_v[n1]]);

    glVertex3iv (pt[n1]);

    glColor3fv (colors[clr_v[n2]]);

    glVertex3iv (pt[n2]);

    glColor3fv (colors[clr_v[n3]]);

    glVertex3iv (pt[n3]);

    glColor3fv (colors[clr_v[n4]]);

    glVertex3iv (pt[n4]);

  glEnd();

}





void setColors(GLint c1, GLint c2, GLint c3, GLint c4, GLint c5, GLint c6, GLint c7, GLint c8) {

  clr_v[0] = c1;

  clr_v[1] = c2;

  clr_v[2] = c3;

  clr_v[3] = c4;

  clr_v[4] = c5;

  clr_v[5] = c6;

  clr_v[6] = c7;

  clr_v[7] = c8;

}



void cube() {

  quad(2,0,1,3);

  quad(6,2,3,7);

  quad(4,0,2,6);

  quad(5,1,0,4);

  quad(7,3,1,5);

  quad(7,5,4,6);

}



// initialize

void initialize() {

  // set background color

  glClearColor(0.5, 0.7, 0.5, 0.0);



  // depth test

  glEnable(GL_DEPTH_TEST);

}



// render the floor

void floor() {

  for (int i=-12; i < 12; i++) {

    for (int j=-10; j < 10; j++) {

      if ( ((i+j) % 2) == 0 )

        glColor3f(0.9, 0.9, 0.9);

      else

        glColor3f(0.2, 0.2, 0.2);



      glBegin(GL_POLYGON);

        glVertex3f(i, -0.01, j);

        glVertex3f(i+1, -0.01, j);

        glVertex3f(i+1, -0.01, j+1);

        glVertex3f(i, -0.01, j+1);

      glEnd();

    }

  }

}



// render

void render() {

  // floor

  floor();



  // cube 1

  glPushMatrix();

    setColors(0,1,2,3,3,2,1,0);

    glTranslatef(-1.0, 0.0, 1.0);

    glRotatef(45.0, 0.0, 1.0, 0.0);

    glScalef(2.0, 2.0, 2.0);

    cube();

  glPopMatrix();



  // cube 2

  glPushMatrix();

    setColors(3,3,1,1,0,0,2,2);

    glTranslatef(-6.0, 0.0, 1.0);

    glRotatef(30.0, 0.0, 1.0, 0.0);

    glScalef(2.0, 2.0, 2.0);

    cube();

  glPopMatrix();



  // cube 3

  glPushMatrix();

    setColors(0,2,1,3,0,3,2,1);

    glTranslatef(4.0, 0.0, 1.0);

    glRotatef(60.0, 0.0, 1.0, 0.0);

    glScalef(2.0, 2.0, 2.0);

    cube();

  glPopMatrix();

}



// display registry

void display(void) {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();



  gluLookAt(0.0, 6.0, 16.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);



  render();



  glutSwapBuffers();

}



// reshape registry

void reshape(int w, int h) {

  glViewport(0, 0, (GLsizei) w, (GLsizei) h);

  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();

  glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 50.0);

  glMatrixMode(GL_MODELVIEW);

}



// main program

int main(int argc, char **argv)

{

  glutInit( &argc, argv );

  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH) ;

  glutInitWindowSize(500, 500);

    glutInitWindowPosition(100, 100);



  int windowHandle = glutCreateWindow("Athabasca University - Unit 1 Section 2 Objective 3");

  glutSetWindow(windowHandle);



  glutDisplayFunc( display );

  glutReshapeFunc( reshape );



  initialize();



    glutMainLoop();

    return 0;

}