#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Button.h"
#include "SurfaceTypes.h"

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

    void Button::draw(bool isPressed, float width, float height) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_LIGHTING);


        // Draw button title
        glColor3f(0.0f, 0.0f, 0.0f);
        float textWidth = 0.0f;
        for (char c : title) {
            textWidth += glutBitmapWidth(GLUT_BITMAP_HELVETICA_12, c); // Accumulate width for each character
        }

        float centerX = xCoordinate + (width - textWidth / glutGet(GLUT_WINDOW_WIDTH) * 20.0f) / 2.0f;
        float centerY = yCoordinate + (height / 2.0f - 0.075f); // Adjust to align vertically

        glRasterPos2f(centerX, centerY); // Place text

        // Render each character
        for (char c : title) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }

        // Set button color based on pressed state
        if (isPressed) {
            glColor3f(0.0f, 0.46f, 0.25f);
        }
        else {
            glColor3f(0.6f, 1.0f, 0.6f);
        }

        // Draw button background (rectangle)
        glBegin(GL_QUADS);
        glVertex2f(xCoordinate, yCoordinate);
        glVertex2f(xCoordinate + width, yCoordinate);
        glVertex2f(xCoordinate + width, yCoordinate + height);
        glVertex2f(xCoordinate, yCoordinate + height);
        glEnd();

        // Draw button border in black
        glColor3f(0.0f, 0.0f, 0.0f);
        glLineWidth(2.0f); // Make the border slightly thicker for better visibility
        glBegin(GL_LINE_LOOP);
        glVertex2f(xCoordinate, yCoordinate);
        glVertex2f(xCoordinate + width, yCoordinate);
        glVertex2f(xCoordinate + width, yCoordinate + height);
        glVertex2f(xCoordinate, yCoordinate + height);
        glEnd();

        glEnable(GL_LIGHTING);
    }