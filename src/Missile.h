#pragma once
#include "Entity.h"

class Missile : public Entity
{
public:
	Missile(Transform* playerTransform);
	~Missile();

	void update(float deltaTime) override;
	void render() override;

	Entity* target;

private:
	Result onCollision(Entity* entity) override;
	Point* moveVector;
	Math* math;
};