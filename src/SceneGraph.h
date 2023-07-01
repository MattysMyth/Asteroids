#pragma once
#include <vector>
#include <iostream>
#include <random>
#include "Spaceship.h"
#include "Asteroid.h"
#include "Arena.h"
#include "Overlay.h"

class SceneGraph
{
public:
	SceneGraph(float arenaRadius);
	~SceneGraph();
	void update(float deltaTime);
	void render();
	void renderGameOver();
	void keyPress(int keyCode);
	void keyRelease(int keyCode);
	void mouseClick(int button, int state);
private:
	std::vector<Entity*> Objects;
	std::vector<Missile*> Missiles;
	std::vector<int> keyPresses;
	std::vector<int> keyReleases;

	Spaceship* player;
	Arena* arena;
	Math* math;
	Overlay* overlay;

	int difficultyLevel;
	int asteroidsDestroyed;
	int elapsedTimeOffset;
	float difficultyIncreaseTime;
	float nextLevelUp;
	float arenaRadius;
	float arenaWidth;
	float arenaHeight;
	float deltaTime;
	bool GameOver;

	void init();
	void reset();
	void createObject(Tag type);
	void createAsteroid();
	void destroyObject(Entity* entity);
	bool checkCollision(Entity* e1, Entity* e2);
	bool checkArenaCollision(Entity* entity);
	void playerWallCheck();
	void processCollision(Entity* entity);
	void updateMissileTarget(Missile* missile);
};