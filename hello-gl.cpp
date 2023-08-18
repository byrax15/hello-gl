// hello-gl.cpp : Defines the entry point for the application.
//

#include "GL/glut.h"

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	{
		glVertex3f(.25, .25, .0);
		glVertex3f(.75, .25, .0);
		glVertex3f(.75, .75, .0);
		glVertex3f(.25, .75, .0);
	}
	glEnd();
	glFlush();
}

void init(void) {
	glClearColor(0, 0, 0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(250, 250);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("hello-gl");
	init();
	glutDisplayFunc(display);
	glutMainLoop();
}
