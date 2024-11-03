#ifndef BUTTON_H
#define BUTTON_H
#include <functional>

class Button {
private: 
	float xCoordinate;
	float yCoordinate;
	std::string title;

	static const float width;
	static const float height;

public: 
	Button(float x, float y, std::string title);

	float getX() const;
	float getY() const;
	void isButtonPressed(float mouseX, float mouseY, std::function<void()> func);
	bool contains(float x, float y, float width = width, float height = height) const;
	void draw(float width = width, float height = height);

};


#endif // BUTTON_H