#include "Bullet.h"

Bullet::Bullet(Transform* playerTransform)
{
	tag = Tag::BULLET;
	collisionResult = Result::NOTHING;
	math = new Math();

	float angleRad = playerTransform->rotation * (PI / 180);
	transform->position->x = playerTransform->position->x + (cos(angleRad) * 1.6f);
	transform->position->y = playerTransform->position->y + (sin(angleRad) * 1.6f);

	transform->rotation = 0.0f;

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
	moveVector->x = (transform->position->x - playerTransform->position->x)/vecLength;
	moveVector->y = (transform->position->y - playerTransform->position->y)/vecLength;

	speed = 15.0f;
	collisionRadius = 0.2f;

	delete playerTransform;
}

Bullet::~Bullet()
{
	delete moveVector;
	moveVector = nullptr;
}

void Bullet::update(float deltaTime)
{
	transform->position->x = transform->position->x + moveVector->x * speed * deltaTime;
	transform->position->y = transform->position->y + moveVector->y * speed * deltaTime;
}

void Bullet::render()
{
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(transform->position->x, transform->position->y, 0.0f);
	glRotatef(transform->rotation, 0.0f, 0.0f, 1.0f);
	glScalef(transform->scale->width, transform->scale->height, 0.0f);

	// Bullet as a point
	glColor3f(1.0, 1.0, 0.0);
	glPointSize(10.0);
	glBegin(GL_POINTS);
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();

	glPopMatrix();
}

Result Bullet::onCollision(Entity* entity)
{
	if (entity->tag  == Tag::ASTEROID)
	{
		return Result::DELETED;
	}
	return Result::NOTHING;
}