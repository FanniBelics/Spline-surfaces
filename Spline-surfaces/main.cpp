#define _USE_MATH_DEFINES 

#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;

int N =4 , M = 4;

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

// Drawing routine.
void drawScene(void)
{

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glColor3f(0.518, 0.8, 0.82);
	glBegin(GL_TRIANGLE_STRIP);

	glVertex3f(30.0, 30.0, 0.0);
	glVertex3f(20.0, 20.0, 0.0);
	glVertex3f(30.0, 70.0, 0.0);
	glVertex3f(20.0, 80.0, 0.0);
	glVertex3f(70.0, 70.0, 0.0);
	glVertex3f(80.0, 80.0, 0.0);
	glVertex3f(70.0, 30.0, 0.0);
	glVertex3f(80.0, 20.0, 0.0);
	glVertex3f(30.0, 30.0, 0.0);
	glVertex3f(20.0, 20.0, 0.0);

	glEnd();

	//Inlines
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);

	glVertex3f(70.0, 30.0, 0.0);
	glVertex3f(80.0, 20.0, 0.0);
	glVertex3f(30.0, 30.0, 0.0);
	glVertex3f(20.0, 20.0, 0.0);
	glVertex3f(30.0, 70.0, 0.0);
	glVertex3f(20.0, 80.0, 0.0);
	glVertex3f(70.0, 70.0, 0.0);
	glVertex3f(80.0, 80.0, 0.0);
	glVertex3f(70.0, 30.0, 0.0);

	glEnd();

	//Inner outline
	glColor3f(0.345, 0.557, 0.553);
	glBegin(GL_LINE_LOOP);
	glVertex3f(30.0, 30.0, 0.0);
	glVertex3f(30.0, 70.0, 0.0);
	glVertex3f(70.0, 70.0, 0.0);
	glVertex3f(70.0, 30.0, 0.0);

	glEnd();

	//Outer Outline
	glBegin(GL_LINE_LOOP);
	glVertex3f(20.0, 20.0, 0.0);
	glVertex3f(20.0, 80.0, 0.0);
	glVertex3f(80.0, 80.0, 0.0);
	glVertex3f(80.0, 20.0, 0.0);

	glEnd();

	glFlush();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Use orthographic projection (appropriate for 2D drawing)
	if (w <= h)
		glOrtho(0.0, 100.0, 0.0, 100.0 * (GLfloat)h / (GLfloat)w, -1.0, 1.0);
	else
		glOrtho(0.0, 100.0 * (GLfloat)w / (GLfloat)h, 0.0, 100.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	cout << "Please provide the N and M as the size of the pane" << endl;
	cout << "If N and M are not proper they are set 4 by default" << endl;
	cout << "N: "; cin >> N;
	if (N <= 1) N = 4;
	cout << "M: "; cin >> M;
	if (M <= 1) M = 4;
	cout << "Set, N: " << N << " M: " << M << endl;
}

// Main routine.
int main(int argc, char** argv)
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Spline Surfaces");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}

