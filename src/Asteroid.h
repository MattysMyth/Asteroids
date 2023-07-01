#pragma once
#include "Entity.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Asteroid : public Entity
{
public:
	Asteroid(Point playerPosition, Point spawnLocation);
	~Asteroid();

	void update(float deltaTime) override;
	void render() override;

	bool inArena;

private:
	Result onCollision(Entity* entity) override;
	void setTarget(Point target);
	void moveAsteroid(float deltaTime);
	int numberOfPoints;
	int maxHitpoints;
	int hitpoints;
	std::vector<float> radiusVector;
	float rotationMin;
	float rotationMax;
	float rotationSpeed;

	Point* moveVector;
	Math* math;
};