#include "Arena.h"

Arena::Arena(float arenaWidth, float arenaHeight)
{
	tag = Tag::ARENA;
	collisionResult = Result::NOTHING;

	transform->position->x = 0.0f;
	transform->position->y = 0.0f;

	transform->rotation = 0.0f;

	transform->scale->width = arenaWidth;
	transform->scale->height = arenaHeight;

	topTrigger = false;
	bottomTrigger = false;
	leftTrigger = false;
	rightTrigger = false;
}

Arena::~Arena()
{

}

void Arena::update(float deltaTime)
{

}

void Arena::render()
{
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(transform->position->x, transform->position->y, 0.0f);
	glRotatef(transform->rotation, 0.0f, 0.0f, 1.0f);
	glScalef(transform->scale->width, transform->scale->height, 0.0f);

	// Arena Top Boundry
	if (topTrigger)
	{
		glColor3f(1.0, 0.0, 0.0);
	}
	else
	{
		glColor3f(1.0, 1.0, 1.0);
	}
	glLineWidth(3.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-1.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, 0.0);
	glEnd();

	// Arena Right Boundry
	if (rightTrigger)
	{
		glColor3f(1.0, 0.0, 0.0);
	}
	else
	{
		glColor3f(1.0, 1.0, 1.0);
	}
	glLineWidth(3.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glEnd();

	// Arena Bottom Boundry
	if (bottomTrigger)
	{
		glColor3f(1.0, 0.0, 0.0);
	}
	else
	{
		glColor3f(1.0, 1.0, 1.0);
	}
	glLineWidth(3.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-1.0, -1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glEnd();

	// Arena Left Boundry
	if (leftTrigger)
	{
		glColor3f(1.0, 0.0, 0.0);
	}
	else
	{
		glColor3f(1.0, 1.0, 1.0);
	}
	glLineWidth(3.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-1.0, -1.0, 0.0);
	glVertex3f(-1.0, 1.0, 0.0);
	glEnd();

	glPopMatrix();
}

Result Arena::onCollision(Entity* entity)
{
	return Result::NOTHING;
}

void Arena::colourChange(Direction direction, bool toggle)
{
	switch (direction)
	{
	case Direction::TOP:
		topTrigger = toggle;
		break;
	case Direction::BOTTOM:
		bottomTrigger = toggle;
		break;
	case Direction::LEFT:
		leftTrigger = toggle;
		break;
	case Direction::RIGHT:
		rightTrigger = toggle;
		break;
	default:
		break;
	}
}