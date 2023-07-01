#pragma once
#include "Entity.h"

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

class Arena : public Entity
{
public:
	Arena(float arenaWidth, float arenaHeight);
	~Arena();
	void update(float deltaTime) override;
	void render() override;
	Result onCollision(Entity* entity) override;
	void colourChange(Direction direction, bool toggle);

	bool topTrigger;
	bool bottomTrigger;
	bool leftTrigger;
	bool rightTrigger;
};