#define _USE_MATH_DEFINES 

#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <vector>

#include "Button.h"
#include "spline.h"
#include "types.cpp"



//Buttons
Button splineButton(-9, 8, "B-Spline");
Button splineButton2(-9, 6.75, "Knots");
Button splineButton3(-9, 5.5, "NURBS");

SurfaceTypes current = NONE;


using namespace std;

void getSpline() {
	current = SPLINE;
}

void getKnots() {
	current = KNOTS;
}

void getNURBS() {
	current = NURBS;
}

//Globals

bool isMainVector = true;
bool isGrid = true;
bool isPoints = true;
bool isSelectedPoint = true;
bool isSelectedVector = true;

float camX = 5.0f, camY = 5.0f, camZ = 5.0f; 
float centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f; 
float upX = 0.0f, upY = 1.0f, upZ = 0.0f;

int N = 4 , M = 4;
int nP = 0, mP = 0;

//matrix for points
std::vector<std::vector<Point3D>> grid;
std::vector<std::vector<Point3D>> surfaceGrid(40, std::vector<Point3D>(40));

// Angles to rotate
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; 


void drawSphere(float x, float y, float z, float radius) {
	glPushMatrix();

	glTranslatef(x, y, z);

	GLUquadric* quadric = gluNewQuadric();
	gluSphere(quadric, radius, 20, 20);
	gluDeleteQuadric(quadric);

	glPopMatrix();
}


// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);


	grid.resize(N);
	for (int i = 0; i < N; ++i) {
		grid[i].resize(M);
	}

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			grid[i][j] = { static_cast<float>((i*8.0f) / N), 0.0f ,static_cast<float>((j*8.0f) / M)}; // x, y, z coordinates
		}
	}

}

// Drawing routine.
void drawScene(void)
{

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	glPushMatrix();
	splineButton.draw();
	splineButton2.draw();
	splineButton3.draw();
	glPopMatrix();

	gluLookAt(
		camX, camY, camZ,    
		centerX, centerY, centerZ, 
		upX, upY, upZ
	);


	glRotatef(Zangle, 0.0, 0.0, 1.0);
	glRotatef(Yangle, 0.0, 1.0, 0.0);
	glRotatef(Xangle, 1.0, 0.0, 0.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glColor3f(0.0, 0.0, 0.0);

	if (isMainVector) {
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(10.0f, 0.0f, 0.0f);
		glEnd();

		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 10.0f, 0.0f);
		glEnd();

		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 10.0f);
		glEnd();

		glTranslatef(1.0, 0.0, 1.0);

		glColor3f(0.0, 0.0, 0.0);
	}

	if (isSelectedVector) {

		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_LINES);
		glVertex3f(grid[nP][mP].x, grid[nP][mP].y, grid[nP][mP].z);
		glVertex3f(grid[nP][mP].x + 1.0f, grid[nP][mP].y, grid[nP][mP].z);
		glEnd();

		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_LINES);
		glVertex3f(grid[nP][mP].x, grid[nP][mP].y, grid[nP][mP].z);
		glVertex3f(grid[nP][mP].x, grid[nP][mP].y + 1.0f, grid[nP][mP].z);
		glEnd();

		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_LINES);
		glVertex3f(grid[nP][mP].x, grid[nP][mP].y, grid[nP][mP].z);
		glVertex3f(grid[nP][mP].x, grid[nP][mP].y, grid[nP][mP].z + 1.0f);
		glEnd();

		glColor3f(0.0, 0.0, 0.0);
	}

	if (isGrid) {

		glColor3f(0.0, 0.0, 0.0);

		for (int i = 0; i < N; ++i) {

			glBegin(GL_LINE_STRIP);

			for (int j = 0; j < M; ++j) {
				glVertex3f(grid[i][j].x, grid[i][j].y, grid[i][j].z);
			}

			glEnd();
		}

		for (int i = 0; i < M; ++i) {

			glBegin(GL_LINE_STRIP);

			for (int j = 0; j < N; ++j) {
				glVertex3f(grid[j][i].x, grid[j][i].y, grid[j][i].z);
			}

			glEnd();
		}

		glColor3f(0.0, 0.0, 0.0);
	}

	if (isPoints) {
		glColor3f(1.0, 0.0, 1.0);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				drawSphere(grid[i][j].x, grid[i][j].y, grid[i][j].z, 0.1f);
			}
		}

		glColor3f(0.0, 0.0, 0.0);
	}

	if (isSelectedPoint) {
		glColor3f(0.0, 1.0, 1.0);

		drawSphere(grid[nP][mP].x, grid[nP][mP].y, grid[nP][mP].z, 0.1f);

		glColor3f(0.0, 0.0, 0.0);
	}

	if (current == SPLINE) {
		drawBSplineSurface(grid, 100);
	}
	else if (current == NURBS) {
		
	}
	else if (current == KNOTS) {

	}


	glFlush();
}

void resize(int w, int h)
{
	
	if (h == 0) h = 1; 

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();

	float aspect = static_cast<float>(w) / h;

	
	if (w <= h) {
		glOrtho(-10.0, 10.0, -10.0 / aspect, 10.0 / aspect, -100.0, 100.0);
	}
	else {
		glOrtho(-10.0 * aspect, 10.0 * aspect, -10.0, 10.0, -100.0, 100.0);
	}

	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
}

// OpenGL window reshape routine.
/*
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
*/

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;

	case 'W':
		Xangle += 5.0;
		if (Xangle > 360.0) Xangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'S':
		Xangle -= 5.0;
		if (Xangle < 0.0) Xangle += 360.0;
		glutPostRedisplay();
		break;
	case 'A':
		Yangle += 5.0;
		if (Yangle > 360.0) Yangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'D':
		Yangle -= 5.0;
		if (Yangle < 0.0) Yangle += 360.0;
		glutPostRedisplay();
		break;
	case 'Q':
		Zangle += 5.0;
		if (Zangle > 360.0) Zangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'E':
		Zangle -= 5.0;
		if (Zangle < 0.0) Zangle += 360.0;
		glutPostRedisplay();
		break;
	case 'n':
		if (nP > 0) {
			nP--;
			glutPostRedisplay();
		}
		break;
	case 'N':
		if (nP < N-1) {
			nP++;
			glutPostRedisplay();
		}
		break;
	case 'm':
		if (mP > 0) {
			mP--;
			glutPostRedisplay();
		}
		break;
	case 'M':
		if (mP < M-1) {
			mP++;
			glutPostRedisplay();
		}
		break;
	case 'x':
		grid[nP][mP].x = grid[nP][mP].x - 0.1f;
		glutPostRedisplay();
		break;
	case 'X':
		grid[nP][mP].x = grid[nP][mP].x + 0.1f;
		glutPostRedisplay();
		break;
	case 'y':
		grid[nP][mP].y = grid[nP][mP].y - 0.1f;
		glutPostRedisplay();
		break;
	case 'Y':
		grid[nP][mP].y = grid[nP][mP].y + 0.1f;
		glutPostRedisplay();
		break;
	case 'z':
		grid[nP][mP].z = grid[nP][mP].z - 0.1f;
		glutPostRedisplay();
		break;
	case 'Z':
		grid[nP][mP].z = grid[nP][mP].z + 0.1f;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void mouseCallback(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// Get the window height
		int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
		int windowWidhth = glutGet(GLUT_WINDOW_WIDTH);

		float oglX = (static_cast<float>(x) / windowWidhth) * 20.0f - 10.0f; // Normalize to [-10, 10]
		float oglY = (static_cast<float>(windowHeight - y) / windowHeight) * (10.0f - (-10.0f)) - 10.0f; // Normalize to [-10, 10]

		splineButton.isButtonPressed(oglX, oglY, getSpline);
		splineButton2.isButtonPressed(oglX, oglY, getKnots);
		splineButton3.isButtonPressed(oglX, oglY, getNURBS);
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
	glutMouseFunc(mouseCallback);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}

