#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "Math.h"

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

struct Transform {
	Point* position;
	float rotation;
	Scale* scale;

	Transform()
	{
		position = new Point();
		rotation = 0;
		scale = new Scale();
	}

	~Transform()
	{
		delete position;
		delete scale;
	}
};

enum class Tag {ASTEROID, PLAYER, BULLET, ARENA, MISSILE};
enum class Result {DELETED, GAMEOVER, NOTHING};
enum class Direction {TOP, BOTTOM, LEFT, RIGHT};

class Entity
{
public:
	Entity();
	virtual ~Entity();

	void init();
	virtual void update(float deltaTime);
	virtual void render();
	virtual void keyPress(int keyCode);
	virtual void keyRelease(int keyCode);
	virtual void mousePressed(int button, int state);
	virtual Result onCollision(Entity* entity);
	
	Transform* transform;
	Tag tag;
	Result collisionResult;

	void moveForward(float speed);
	void moveBackward(float speed);
	void rotate(float rotation, float x, float y, float z);

	float speed;
	float collisionRadius;

	float PI = 3.14159265359f;
};