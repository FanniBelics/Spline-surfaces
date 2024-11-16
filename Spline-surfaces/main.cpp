#define _USE_MATH_DEFINES 

#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <vector>

#include "Button.h"
#include "SurfaceTypes.h"
#include "spline.h"
#include "bezier.h"
#include "nurbs.h"
#include "convert.h"

using namespace std;

//Buttons
Button splineButton(-9, 8, "Bezier");
Button splineButton2(-9, 6.75, "B-spline");
Button splineButton3(-9, 5.5, "Nurbs");

SurfaceType current = NONE;

void getBezier() {
	current = current == BEZIER ? NONE : BEZIER;
	glutPostRedisplay();
}
void getBspline() {
	current = current == BSPLINE ? NONE : BSPLINE;
	glutPostRedisplay();
}
void getNurbs() {
	current = current == NURBS ? NONE : NURBS;
	glutPostRedisplay();
}

//Globals

bool isStats = true; //l
bool isMainVector = true; //0
bool isGrid = true; //1
bool isPoints = true; //2
bool isSelectedPoint = true; //3
bool isSelectedVector = true; //4
bool isSurfaceGrid = true; //5
bool isSurface = true; //6

bool isClamped = true;

float camX = 5.0f, camY = 5.0f, camZ = 5.0f; 
float centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f; 
float upX = 0.0f, upY = 1.0f, upZ = 0.0f;

int N = 4 , M = 4;
int nP = 0, mP = 0;

//matrix for points
std::vector<std::vector<Point3D>> grid;
std::vector<std::vector<Point3D>> surfaceGrid(8, std::vector<Point3D>(8));

// Angles to rotate
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; 

void writeStats(float x, float y) {
	string content = "Current point:\n(x: " + convert(grid[nP][mP].x) +
		", y: " + convert(grid[nP][mP].y) +
		", z: " + convert(grid[nP][mP].z) + ")\n" +
		"Weight: " + convert(grid[nP][mP].w) + "\n" +
		"Angles: (" + convert(Xangle) + ", " + convert(Yangle) + ", " + convert(Zangle) + ")\n" + 
		"Current gridpoint: (" + convertInt(nP+1) + ", " + convertInt(mP+1) + ")";

	glColor3f(0.0f, 0.0f, 0.0f);

	vector<string> lines;
	size_t start = 0, end;
	while ((end = content.find('\n', start)) != string::npos) {
		lines.push_back(content.substr(start, end - start));
		start = end + 1;
	}
	lines.push_back(content.substr(start));

	glRasterPos2f(x, y);

	for (const auto& line : lines) {
		for (char c : line) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
		}
		y -= 0.7f; 
		glRasterPos2f(x, y);
	}
}


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
			float gridX = static_cast<float>((i * 8.0f) / (N - 1));
			float gridZ = static_cast<float>((j * 8.0f) / (M - 1));
			float gridY = sqrt( ((4.0f - abs(4.0f - gridX)) * (4.0f - abs(4.0f-gridZ))) + ((4.0f - abs(4.0f - gridX)) + (4.0f - abs(4.0f - gridZ))));

			grid[i][j] = {gridX , gridY ,gridZ}; // x, y, z coordinates
		}
	}

}

// Drawing routine.
void drawScene(void)
{

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glPushMatrix();
	splineButton.draw(current == BEZIER);
	splineButton2.draw(current == BSPLINE);
	splineButton3.draw(current == NURBS);
	glPopMatrix();

	glPushMatrix();
	if (isStats)
		writeStats(6.0f, 9.0f);
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

	switch (current)
	{
	case(BEZIER):
		surfaceGrid = BezierSurface(grid,8);
		break;
	case(BSPLINE):
		surfaceGrid = BSplineSurface(grid, 8,8, N-1, M-1, isClamped);
		break;
	case(NURBS):
		surfaceGrid = NURBSSurface(grid,8,8,N-1,M-1, isClamped);
		break;
	default:
		for (int i = 0; i < surfaceGrid.size(); i++) {
			for (int j = 0; j < surfaceGrid[i].size(); j++) {
				surfaceGrid[i][j] = { static_cast<float>((i * 8.0f) / (surfaceGrid.size() - 1)), 0.0f ,static_cast<float>((j * 8.0f) / (surfaceGrid[i].size() - 1)) };
			}
		}
		break;
	}


	if (isMainVector) {
		glDisable(GL_LIGHTING);
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
		glEnable(GL_LIGHTING);
	}

	if (isSurfaceGrid) {
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(0.5, 0.5, 0.5);

		if (surfaceGrid.size() == 1) {
			glBegin(GL_LINES);

			for (int j = 0; j < surfaceGrid[0].size(); j++) {
				glVertex3f(surfaceGrid[0][j].x, surfaceGrid[0][j].y, surfaceGrid[0][j].z);
			}

			glEnd();
		}
		else {
			for (int i = 0; i < surfaceGrid.size() - 1; i++) {
				glBegin(GL_TRIANGLE_STRIP);

				for (int j = 0; j < surfaceGrid[i].size(); j++) {
					glVertex3f(surfaceGrid[i][j].x, surfaceGrid[i][j].y, surfaceGrid[i][j].z);
					glVertex3f(surfaceGrid[i + 1][j].x, surfaceGrid[i + 1][j].y, surfaceGrid[i + 1][j].z);
				}

				glEnd();
			}
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(0.0, 0.0, 0.0);
		glEnable(GL_LIGHTING);
	}

	if (isSurface) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(1.0, 0.5, 0.0);

		if (surfaceGrid.size() == 1) {
			glBegin(GL_LINES);

			for (int j = 0; j < surfaceGrid[0].size(); j++) {
				glVertex3f(surfaceGrid[0][j].x, surfaceGrid[0][j].y, surfaceGrid[0][j].z);
			}

			glEnd();
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			for (int i = 0; i < surfaceGrid.size() - 1; i++) {
				glBegin(GL_TRIANGLE_STRIP);

				for (int j = 0; j < surfaceGrid[i].size(); j++) {

					// Normálvektor kiszámítása az adott csúcs körül
					Point3D normal = calculateNormal(surfaceGrid, i, j);

					glNormal3f(normal.x, normal.y, normal.z); 
					glVertex3f(surfaceGrid[i + 1][j].x, surfaceGrid[i + 1][j].y, surfaceGrid[i + 1][j].z);

					glNormal3f(normal.x, normal.y, normal.z); //ugyan az a normál
					glVertex3f(surfaceGrid[i][j].x, surfaceGrid[i][j].y, surfaceGrid[i][j].z);

				}

				glEnd();
			}

			

		}	
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(0.0, 0.0, 0.0);

	}

	if (isGrid) {

		glDisable(GL_LIGHTING);
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
		glEnable(GL_LIGHTING);
	}

	if (isPoints) {

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(1.0, 0.0, 1.0);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				drawSphere(grid[i][j].x, grid[i][j].y, grid[i][j].z, 0.1f);
			}
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(0.0, 0.0, 0.0);
	}

	if (isSelectedPoint) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(0.0, 1.0, 1.0);

		drawSphere(grid[nP][mP].x, grid[nP][mP].y, grid[nP][mP].z, 0.12f);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(0.0, 0.0, 0.0);
	}

	if (isSelectedVector) {
		glDisable(GL_LIGHTING);
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
		glEnable(GL_LIGHTING);
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
		glOrtho(-7.5 , 10.0, -10.0 / aspect, 10.0 / aspect, -100.0, 100.0);
	}
	else {
		glOrtho(-7.5 * aspect, 10.0 * aspect, -10.0, 10.0, -100.0, 100.0);
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

	case 'w':
		Xangle += 5.0;
		if (Xangle > 360.0) Xangle -= 360.0;
		glutPostRedisplay();
		break;
	case 's':
		Xangle -= 5.0;
		if (Xangle < 0.0) Xangle += 360.0;
		glutPostRedisplay();
		break;
	case 'a':
		Yangle += 5.0;
		if (Yangle > 360.0) Yangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'd':
		Yangle -= 5.0;
		if (Yangle < 0.0) Yangle += 360.0;
		glutPostRedisplay();
		break;
	case 'q':
		Zangle += 5.0;
		if (Zangle > 360.0) Zangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'e':
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
	case 'v':
		grid[nP][mP].w = grid[nP][mP].w - 0.1f;
		glutPostRedisplay();
		break;
	case 'V':
		grid[nP][mP].w = grid[nP][mP].w + 0.1f;
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
	case '0':
		isMainVector = !isMainVector;
		glutPostRedisplay();
		break;
	case '1':
		isGrid = !isGrid;
		glutPostRedisplay();
		break;
	case '2':
		isPoints = !isPoints;
		glutPostRedisplay();
		break;
	case '3':
		isSelectedPoint = !isSelectedPoint;
		glutPostRedisplay();
		break;
	case '4':
		isSelectedVector = !isSelectedVector;
		glutPostRedisplay();
		break;
	case '5':
		isSurfaceGrid = !isSurfaceGrid;
		glutPostRedisplay();
		break;
	case '6':
		isSurface = !isSurface;
		glutPostRedisplay();
		break;
	case '7':
		getBezier();
		break;
	case '8':
		getBspline();
		break;
	case '9':
		getNurbs();
		break;
	case 'l':
		isStats = !isStats;
		glutPostRedisplay();
		break;
	case 'c':
		isClamped = !isClamped;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void mouseCallback(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
		int windowWidhth = glutGet(GLUT_WINDOW_WIDTH);

		float oglX = (static_cast<float>(x) / windowWidhth) * 20.0f - 10.0f; // Normalize to [-10, 10]
		float oglY = (static_cast<float>(windowHeight - y) / windowHeight) * (10.0f - (-10.0f)) - 10.0f; // Normalize to [-10, 10]

		splineButton.isButtonPressed(oglX, oglY, getBezier);
		splineButton2.isButtonPressed(oglX, oglY, getBspline);
		splineButton3.isButtonPressed(oglX, oglY, getNurbs);
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

void setupLighting() {
	
	GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 0.9f }; 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	float modelTwoside[] = { GL_TRUE };
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, modelTwoside);

	GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 0.9f }; 
	GLfloat lightPosition[] = { 10.0f, 10.0f, 10.0f, 1.0f }; 

	glEnable(GL_LIGHT0);                 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void setupMaterial() {
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 0.9f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);


	GLfloat materialShininess = 50.0f;
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialShininess);
}


// Main routine.
int main(int argc, char** argv)
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(800, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Spline Surfaces");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutMouseFunc(mouseCallback);

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);  
	glEnable(GL_LIGHTING);    
	setupLighting();
	setupMaterial();
	glShadeModel(GL_SMOOTH);

	setup();

	glutMainLoop();
}

