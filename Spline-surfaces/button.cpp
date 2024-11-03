#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Button.h"


using namespace std;


const float Button::width = 4;
const float Button::height = 1.25;

	Button::Button(float x, float y, string title)
		: xCoordinate(x), yCoordinate(y), title(title) 
    {
    }


	float Button::getX() const{
		return xCoordinate;
	}

	float Button::getY() const{
		return yCoordinate;
	}

    bool Button::contains(float x, float y, float width, float height) const {
        return x >= xCoordinate && x <= xCoordinate + width &&
            y >= yCoordinate && y <= yCoordinate + height;
    }

	void Button::isButtonPressed(float mouseX, float mouseY, std::function<void()> func) {
        if (Button::contains(mouseX, mouseY)) {
            func();
            cout << "Button " << title << " is pressed!" << endl;
        }
	}

    void Button::draw(float width, float height) {
        // Draw button rectangle
        glColor3f(0.7f, 0.7f, 0.7f); 
        glBegin(GL_QUADS);
        glVertex2f(xCoordinate, yCoordinate);
        glVertex2f(xCoordinate + width, yCoordinate);
        glVertex2f(xCoordinate + width, yCoordinate + height);
        glVertex2f(xCoordinate, yCoordinate + height);
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f);  // Black border
        glBegin(GL_LINE_LOOP);
        glVertex2f(xCoordinate, yCoordinate);
        glVertex2f(xCoordinate + width, yCoordinate);
        glVertex2f(xCoordinate + width, yCoordinate + height);
        glVertex2f(xCoordinate, yCoordinate + height);
        glEnd();

        // Draw button title
        glColor3f(0.0f, 0.0f, 0.0f); 
        glRasterPos2f(xCoordinate + width / 6, yCoordinate + height / 2);  // Position text

        for (char c : title) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);  // Render each character
        }
    }