#include "Spaceship.h"

Spaceship::Spaceship()
{
	tag = Tag::PLAYER;
	collisionResult = Result::NOTHING;

	transform->position->x = 0.0f;
	transform->position->y = 0.0f;

	transform->rotation = 90.0f;

	transform->scale->width = 2.0f;
	transform->scale->height = 2.0f;

	speed = 10.0f;
	collisionRadius = 0.5f * transform->scale->width;

	bulletFireRate = 25.0f;
	nextBulletFire = 0;

	missileFireRate = 100.0f;
	nextMissileFire = 0;

	upKeyPressed = false;
	downKeyPressed = false;
	leftKeyPressed = false;
	rightKeyPressed = false;
	zKeyPressed = false;

	gunEquipped = true;
}

Spaceship::~Spaceship()
{
	
}

// Called by SceneGraph.update(). Use to update this objects logic
void Spaceship::update(float deltaTime)
{
	// Check if keys are pressed
	if (upKeyPressed)
	{
		moveForward(speed * deltaTime);
	}
	if (downKeyPressed)
	{
		moveBackward(speed * deltaTime);
	}
	if (leftKeyPressed)
	{
		rotate(180.0f * deltaTime, 0.0f, 0.0f, 1.0f);
	}
	if (rightKeyPressed)
	{
		rotate(-180.0f * deltaTime, 0.0f, 0.0f, 1.0f);
	}
	if (zKeyPressed)
	{
		if (gunEquipped)
		{
			gunEquipped = false;
		}
		else
		{
			gunEquipped = true;
		}
		zKeyPressed = false;
	}
}

// Called by SceneGraph.render(). Use to draw this object (GL Calls)
void Spaceship::render()
{
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(transform->position->x, transform->position->y, 0.0f);
	glRotatef(transform->rotation - 90, 0.0f, 0.0f, 1.0f);
	glScalef(transform->scale->width, transform->scale->height, 0.0f);

	// Spaceship Nose
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.05f, 0.5f, 0.0f);
	glVertex3f(0.05f, 0.5f, 0.0f);
	glVertex3f(0.15f, 0.0f, 0.0f);
	glVertex3f(-0.15f, 0.0f, 0.0f);
	glEnd();

	// Spaceship Right wing
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.15f, 0.0f, 0.0f);
	glVertex3f(0.5f, -0.15f, 0.0f);
	glVertex3f(0.5f, -0.3f, 0.0f);
	glVertex3f(0.15f, -0.5f, 0.0f);
	glEnd();

	// Spaceship Body
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.15, 0.0, 0.0);
	glVertex3f(0.15, -0.5, 0.0);
	glVertex3f(-0.15, -0.5, 0.0);
	glVertex3f(-0.15, 0.0, 0.0);
	glEnd();

	// Spaceship Left Wing
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.15, 0.0, 0.0);
	glVertex3f(-0.5, -0.15, 0.0);
	glVertex3f(-0.5, -0.3, 0.0);
	glVertex3f(-0.15, -0.5, 0.0);
	glEnd();

	if (gunEquipped)
	{
		// Gun Barrel
		glColor3f(0.39, 0.0, 0.0);
		glBegin(GL_POLYGON);
		glVertex3f(-0.03, 0.5, 0.0);
		glVertex3f(-0.03, 0.75, 0.0);
		glVertex3f(0.03, 0.75, 0.0);
		glVertex3f(0.03, 0.5, 0.0);
		glEnd();

		// Gun Left chamber
		glColor3f(0.39, 0.39, 0.39);
		glBegin(GL_POLYGON);
		glVertex3f(-0.065, 0.5, 0.0);
		glVertex3f(-0.065, 0.75, 0.0);
		glVertex3f(-0.03, 0.75, 0.0);
		glVertex3f(-0.03, 0.5, 0.0);
		glEnd();

		// Gun Right chamber
		glColor3f(0.39, 0.39, 0.39);
		glBegin(GL_POLYGON);
		glVertex3f(0.065, 0.5, 0.0);
		glVertex3f(0.065, 0.75, 0.0);
		glVertex3f(0.03, 0.75, 0.0);
		glVertex3f(0.03, 0.5, 0.0);
		glEnd();
	}
	else
	{
		// Launcher Barrel
		glColor3f(0.39, 0.0, 0.0);
		glBegin(GL_POLYGON);
		glVertex3f(-0.03, 0.5, 0.0);
		glVertex3f(-0.03, 0.75, 0.0);
		glVertex3f(0.03, 0.75, 0.0);
		glVertex3f(0.03, 0.5, 0.0);
		glEnd();

		// Launcher Left chamber
		glColor3f(0.39, 0.39, 0.39);
		glBegin(GL_POLYGON);
		glVertex3f(-0.065, 0.5, 0.0);
		glVertex3f(-0.065, 0.75, 0.0);
		glVertex3f(-0.03, 0.75, 0.0);
		glVertex3f(-0.03, 0.5, 0.0);
		glEnd();

		// Launcher Left Wing
		glColor3f(0.39, 0.39, 0.39);
		glBegin(GL_TRIANGLES);
		glVertex3f(-0.065, 0.75, 0.0);
		glVertex3f(-0.065, 0.5, 0.0);
		glVertex3f(-0.25, 0.5, 0.0);
		glEnd();

		// Launcher Right chamber
		glColor3f(0.39, 0.39, 0.39);
		glBegin(GL_POLYGON);
		glVertex3f(0.065, 0.5, 0.0);
		glVertex3f(0.065, 0.75, 0.0);
		glVertex3f(0.03, 0.75, 0.0);
		glVertex3f(0.03, 0.5, 0.0);
		glEnd();

		// Launcher Right Wing
		glColor3f(0.39, 0.39, 0.39);
		glBegin(GL_TRIANGLES);
		glVertex3f(0.065, 0.75, 0.0);
		glVertex3f(0.065, 0.5, 0.0);
		glVertex3f(0.25, 0.5, 0.0);
		glEnd();
	}

	glPopMatrix();
}

// Takes in keys pressed and stores into a vector to be actioned on frame updates
void Spaceship::keyPress(int keyCode)
{
	if (keyCode == 'w')
	{
		upKeyPressed = true;
	}
	else if (keyCode == 's')
	{
		downKeyPressed = true;
	}
	else if (keyCode == 'a')
	{
		leftKeyPressed = true;
	}
	else if (keyCode == 'd')
	{
		rightKeyPressed = true;
	}
	else if (keyCode == 'z')
	{
		if (zKeyPressed)
		{
			zKeyPressed = false;
		}
		else
		{
			zKeyPressed = true;
		}
	}
}

void Spaceship::keyRelease(int keyCode)
{
	if (keyCode == 'w')
	{
		upKeyPressed = false;
	}
	else if (keyCode == 's')
	{
		downKeyPressed = false;
	}
	else if (keyCode == 'a')
	{
		leftKeyPressed = false;
	}
	else if (keyCode == 'd')
	{
		rightKeyPressed = false;
	}
}

void Spaceship::mousePressed(int button, int state)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		leftClickPressed = true;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		leftClickPressed = false;
	}
}

bool Spaceship::isLeftClickDown()
{
	return leftClickPressed;
}

// Logic to be performed on collision with the given Entity
Result Spaceship::onCollision(Entity* entity)
{
	if (entity->tag == Tag::ASTEROID)
	{
		return Result::GAMEOVER;
	}
	return Result::NOTHING;
}