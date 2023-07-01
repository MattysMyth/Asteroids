#pragma once
#include "Entity.h"

class Bullet : public Entity
{
public:
	Bullet(Transform* playerTransform);
	~Bullet();

	void update(float deltaTime) override;
	void render() override;

private:
	Result onCollision(Entity* entity) override;
	Point* moveVector;
	Math* math;
};