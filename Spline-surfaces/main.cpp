#define _USE_MATH_DEFINES 

#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <vector>

//Dff 3D points
struct Point3D {
	float x;
	float y;
	float z;
};


using namespace std;

int N =4 , M = 4;


//matrix for points
std::vector<std::vector<Point3D>> grid;


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
			grid[i][j] = { static_cast<float>(i*10), static_cast<float>(j*10) , 0.0f}; // x, y, z coordinates
		}
	}
}

// Drawing routine.
void drawScene(void)
{

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef(5.0, 5.0, 0.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

