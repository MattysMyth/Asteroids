#include "Entity.h"

Entity::Entity()
{
	transform = new Transform();
	init();
}

Entity::~Entity()
{
	delete transform;
}

// Called by SceneGraph.createObject() when this object is created. Use to Initialise this object.
void Entity::init()
{
	transform->position->x = 0.0;
	transform->position->y = 0.0;
	transform->rotation = 0.0;
	transform->scale->width = 1.0;
	transform->scale->height = 1.0;
	collisionRadius = 0.0;
}

// Called by SceneGraph.update(). Use to update this objects logic. Virtual function to be overridden
void Entity::update(float deltaTime)
{

}

// Called by SceneGraph.render(). Use to draw this object (GL Calls). Virtual function to be overridden
void Entity::render()
{

}

// Called by SceneGraph.keyPress(). Use to handle key press logic on objects. Virtual function to be overridden
void Entity::keyPress(int keyCode)
{

}

// Called by SceneGraph.keyRelease(). Use to handle key release logic on objects. Virtual function to be overridden
void Entity::keyRelease(int keyCode)
{

}

void Entity::mousePressed(int button, int state)
{

}

/*
* Transforms the object forward to a new position in world space according to the objects rotation.
* Speed includes the Delta Time adjustment as well as a scalar (speed) to increase or decrease the rate of movement.
*/
void Entity::moveForward(float speed)
{
	float angleRad = transform->rotation * (PI/180);
	transform->position->x = transform->position->x + (cos(angleRad) * speed);
	transform->position->y = transform->position->y + (sin(angleRad) * speed);
}

/*
* Transforms the object backward to a new position in world space according to the objects rotation.
* Speed includes the Delta Time adjustment as well as a scalar (speed) to increase or decrease the rate of movement.
*/
void Entity::moveBackward(float speed)
{
	float angleRad = transform->rotation * (PI / 180);
	transform->position->x = transform->position->x + (cos(angleRad) * speed * -1);
	transform->position->y = transform->position->y + (sin(angleRad) * speed * -1);
}

// Rotate the object around the x, y point given
void Entity::rotate(float rotation, float x, float y, float z)
{
	transform->rotation = transform->rotation + rotation;
	if (transform->rotation < -360)
	{
		transform->rotation = transform->rotation + 360;
	}
	else if (transform->rotation > 360)
	{
		transform->rotation = transform->rotation - 360;
	}
}

Result Entity::onCollision(Entity* entity)
{
	return Result::NOTHING;
}