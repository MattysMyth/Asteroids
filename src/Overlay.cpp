#include"Overlay.h"

Overlay::Overlay(int screenWidth, int screenHeight)
{
	elapsedTime = new Element();
	asteroidsDestroyed = new Element();

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	elapsedTime->x = screenWidth / 8;
	elapsedTime->y = screenHeight - (screenHeight / 8);
	elapsedTime->font = GLUT_BITMAP_TIMES_ROMAN_24;
	elapsedTime->string = "Elapsed Time: 0";

	asteroidsDestroyed->x = screenWidth - 2 * (screenWidth / 8);
	asteroidsDestroyed->y = screenHeight - (screenHeight / 8);
	asteroidsDestroyed->font = GLUT_BITMAP_TIMES_ROMAN_24;
	asteroidsDestroyed->string = "Asteroids Destroyed: 0";

	elements.push_back(elapsedTime);
	elements.push_back(asteroidsDestroyed);
}

Overlay::~Overlay()
{
	for (Element* element : elements)
	{
		delete element;
	}
	elements.clear();
}

void Overlay::update(int asteroidsDestroyed, int elapsedTime)
{
	std::string test = "First thing";
	test = "Second thing";
	this->asteroidsDestroyed->string = "Asteroids Destroyed: " + std::to_string(asteroidsDestroyed);
	this->elapsedTime->string = "Elapsed Time: " + std::to_string(elapsedTime);
}

void Overlay::render()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// Set the projection - use this to render text
	gluOrtho2D(0, screenWidth, 0, screenHeight);
	glMatrixMode(GL_MODELVIEW);

	for (Element* element : elements)
	{
		glRasterPos2f(element->x, element->y);
		int len = element->string.length();
		for (int i = 0; i < len; i++)
		{
			glutBitmapCharacter(element->font, element->string[i]);
		}
	}

	glMatrixMode(GL_PROJECTION);

	// Restore the original projection matrix for rendering everything else
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
}