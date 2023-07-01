#include "Asteroid.h"

Asteroid::Asteroid(Point playerPosition, Point spawnLocation)
{
	tag = Tag::ASTEROID;
	collisionResult = Result::NOTHING;
	math = new Math();

	transform->position->x = spawnLocation.x;
	transform->position->y = spawnLocation.y;

	transform->rotation = 0.0f;

	int scaleFactor = math->randomInt(1, 3);
	numberOfPoints = math->randomInt(12, 24);
	for (int i = 0; i < numberOfPoints; i++)
	{
		radiusVector.push_back(math->randomFloat(0.5, 1.0));
	}

	maxHitpoints = scaleFactor;
	hitpoints = scaleFactor;

	rotationMin = -90.0f;
	rotationMax = 90.0f;

	rotationSpeed = math->randomFloat(rotationMin, rotationMax);

	transform->scale->width = scaleFactor;
	transform->scale->height = scaleFactor;

	speed = math->randomInt(3, 8);
	collisionRadius = 1.0f * scaleFactor;

	inArena = false;

	moveVector = new Point();
	setTarget(playerPosition);
}

Asteroid::~Asteroid()
{
	delete moveVector;
	moveVector = nullptr;
}

// Called by SceneGraph.update(). Use to update this objects logic
void Asteroid::update(float deltaTime)
{
	moveAsteroid(deltaTime);
}

// Called by SceneGraph.render(). Use to draw this object (GL Calls)
void Asteroid::render()
{
	float x, y;
	float theta;

	glPushMatrix();
	glLoadIdentity();

	glTranslatef(transform->position->x, transform->position->y, 0.0f);
	glRotatef(transform->rotation, 0.0f, 0.0f, 1.0f);
	glScalef(transform->scale->width, transform->scale->height, 0.0f);

	switch (hitpoints % maxHitpoints)
	{
	case 0:
		glColor3f(1.0, 1.0, 1.0); // Max Hitpoints
		break;
	case 1:
		glColor3f(1.0, 0.0, 0.0); // 1 Hitpoint Left
		break;
	case 2:
		glColor3f(0.7, 0.7, 0.0); // 2 Hitpoints Left
		break;
	default:
		break;
	}

	glLineWidth(2.0);
	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < numberOfPoints; i++)
	{
		theta = i / (float)numberOfPoints * 2.0 * M_PI;
		x = radiusVector[i] * cosf(theta);
		y = radiusVector[i] * sinf(theta);
		glVertex2f(x, y);
	}
	glEnd();

	glPopMatrix();
}

// Logic to be performed on collision with the given Entity
Result Asteroid::onCollision(Entity* entity)
{
	if (entity->tag == Tag::BULLET)
	{
		if (hitpoints > 1)
		{
			hitpoints = hitpoints - 1;
			return Result::NOTHING;
		}
		else
		{
			return Result::DELETED;
		}
	}
	else if (entity->tag == Tag::MISSILE)
	{
		if (hitpoints > 2)
		{
			hitpoints = hitpoints - 2;
			return Result::NOTHING;
		}
		else
		{
			return Result::DELETED;
		}
	}
	return Result::NOTHING;
}

void Asteroid::setTarget(Point target)
{
	float xLength = target.x - transform->position->x;
	float yLength = target.y - transform->position->y;

	float vecLength = math->vectorLength(xLength, yLength);

	if (vecLength == 0)
	{
		vecLength = 0.001;
	}

	moveVector->x = (target.x - transform->position->x)/vecLength;
	moveVector->y = (target.y - transform->position->y)/vecLength;
}

void Asteroid::moveAsteroid(float deltaTime)
{
	transform->position->x = transform->position->x + moveVector->x * speed * deltaTime;
	transform->position->y = transform->position->y + moveVector->y * speed * deltaTime;
	transform->rotation = transform->rotation + rotationSpeed * deltaTime;
}