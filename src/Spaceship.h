#pragma once
#include "Entity.h"
#include "Bullet.h"
#include "Missile.h"
#include <vector>

class Spaceship : public Entity
{
public:
	Spaceship();
	~Spaceship();

	void update(float deltaTime) override;
	void render() override;
	void keyPress(int keyCode) override;
	void keyRelease(int keyCode) override;
	void mousePressed(int button, int state) override;
	bool isLeftClickDown();

	float bulletFireRate;
	float nextBulletFire;
	float missileFireRate;
	float nextMissileFire;
	bool gunEquipped;

private:
	bool upKeyPressed;
	bool downKeyPressed;
	bool leftKeyPressed;
	bool rightKeyPressed;
	bool leftClickPressed;
	bool zKeyPressed;

	Result onCollision(Entity* entity) override;
};