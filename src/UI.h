#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <stdbool.h>

#if _WIN32
#include <windows.h>
#endif
#if __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

struct Element
{
	float x;
	float y;
	void* font;
	std::string string;
};

struct UI
{
	UI();
	~UI();

	virtual void render();
	virtual void update(int asteroidsDestroyed, int elapsedTime);
};