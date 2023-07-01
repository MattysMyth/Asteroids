#include "Missile.h"

Missile::Missile(Transform* playerTransform)
{
	tag = Tag::MISSILE;
	collisionResult = Result::NOTHING;
	math = new Math();

	float angleRad = playerTransform->rotation * (PI / 180);
	transform->position->x = playerTransform->position->x + (cos(angleRad) * 1.6f);
	transform->position->y = playerTransform->position->y + (sin(angleRad) * 1.6f);

	transform->rotation = 90.0f;

	transform->scale->width = 1.0f;
	transform->scale->height = 1.0f;

	float xLength = transform->position->x - playerTransform->position->x;
	float yLength = transform->position->y - playerTransform->position->y;

	float vecLength = math->vectorLength(xLength, yLength);

	if (vecLength == 0)
	{
		vecLength = 0.001;
	}

	moveVector = new Point();
	moveVector->x = (transform->position->x - playerTransform->position->x) / vecLength;
	moveVector->y = (transform->position->y - playerTransform->position->y) / vecLength;

	target = nullptr;

	speed = 10.0f;
	collisionRadius = 0.2f;

	delete playerTransform;
}

Missile::~Missile()
{
	delete moveVector;
	delete math;
	moveVector = nullptr;
	target = nullptr;
	math = nullptr;
}

void Missile::update(float deltaTime)
{
	if (target != nullptr)
	{
		float dx = target->transform->position->x - transform->position->x;
		float dy = target->transform->position->y - transform->position->y;
		double angle = atan2(dy, dx);
		angle = angle * (180 / PI);

		transform->rotation = angle;

		// assign moveVector to be the direction vector towards the target
		float lengthToTarget = math->vectorLength(target->transform->position->x - transform->position->x, target->transform->position->y - transform->position->y);
		moveVector->x = math->directionVector(target->transform->position->x, transform->position->x, lengthToTarget);
		moveVector->y = math->directionVector(target->transform->position->y, transform->position->y, lengthToTarget);
	}

	transform->position->x = transform->position->x + moveVector->x * speed * deltaTime;
	transform->position->y = transform->position->y + moveVector->y * speed * deltaTime;
}

void Missile::render()
{
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(transform->position->x, transform->position->y, 0.0f);
	glRotatef(transform->rotation - 90, 0.0f, 0.0f, 1.0f);
	glScalef(transform->scale->width, transform->scale->height, 0.0f);

	// Bullet as a point
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
	glVertex3f(-0.25, -0.25, 0.0);
	glVertex3f(0.0, 0.5, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glEnd();

	glPopMatrix();
}

Result Missile::onCollision(Entity* entity)
{
	if (entity->tag == Tag::ASTEROID)
	{
		return Result::DELETED;
	}
	return Result::NOTHING;
}