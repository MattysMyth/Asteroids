#include "SceneGraph.h"

SceneGraph::SceneGraph(float arenaRadius)
{
	difficultyIncreaseTime = 500.0f;
	this->arenaRadius = arenaRadius;
	float screenWidth = glutGet(GLUT_SCREEN_WIDTH);
	float screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
	float aspect = screenWidth / screenHeight;

	if (screenWidth <= screenHeight)
	{
		arenaHeight = arenaRadius * aspect;
		arenaWidth = arenaRadius;
	}
	else
	{
		arenaHeight = arenaRadius;
		arenaWidth = arenaRadius * aspect;
	}

	init();
}

SceneGraph::~SceneGraph()
{
	for (Entity* object : Objects)
	{
		delete object;
	}
	Objects.clear();
	Missiles.clear();
	keyPresses.clear();
	keyReleases.clear();
	delete arena;
	delete math;
	delete overlay;
	arena = nullptr;
	math = nullptr;
	overlay = nullptr;
}

void SceneGraph::init()
{
	arena = new Arena(arenaWidth, arenaHeight);
	player = new Spaceship();
	math = new Math();
	overlay = new Overlay(arenaWidth, arenaHeight);
	Objects.push_back(player);

	difficultyLevel = 1;
	nextLevelUp = 0;
	asteroidsDestroyed = 0;
	elapsedTimeOffset = glutGet(GLUT_ELAPSED_TIME) / 1000;
	GameOver = false;
}

void SceneGraph::reset()
{
	for (Entity* object : Objects)
	{
		delete object;
	}
	Objects.clear();
	Missiles.clear();
	keyPresses.clear();
	keyReleases.clear();
	delete arena;
	delete math;
	delete overlay;
	init();
}

// Called by main.on_idle(), loop all objects and call object.update(float deltaTime)
void SceneGraph::update(float deltaTime)
{
	this->deltaTime = deltaTime;
	float timeElapsed = glutGet(GLUT_ELAPSED_TIME) / 10;

	// Check elapsed time, increase difficulty if time has passed
	if (timeElapsed >= nextLevelUp)
	{
		// Launch Asteroids
		for (int i = 0; i < difficultyLevel; i++)
		{
			createObject(Tag::ASTEROID);
		}

		nextLevelUp = timeElapsed + difficultyIncreaseTime;
		difficultyLevel++;
	}

	if (player->isLeftClickDown() &&
		player->gunEquipped &&
		!GameOver)
	{
		if (timeElapsed  > player->nextBulletFire)
		{
			player->nextBulletFire = timeElapsed + player->bulletFireRate;
			Transform* trans = new Transform();
			trans->position->x = player->transform->position->x;
			trans->position->y = player->transform->position->y;
			trans->rotation = player->transform->rotation;
			trans->scale->height = player->transform->scale->height;
			trans->scale->width = player->transform->scale->width;
			Objects.push_back(new Bullet(trans));
			trans = nullptr;
		}
	}
	else if (player->isLeftClickDown() && !player->gunEquipped)
	{
		if (timeElapsed > player->nextMissileFire)
		{
			player->nextMissileFire = timeElapsed + player->missileFireRate;
			Transform* trans = new Transform();
			trans->position->x = player->transform->position->x;
			trans->position->y = player->transform->position->y;
			trans->rotation = player->transform->rotation;
			trans->scale->height = player->transform->scale->height;
			trans->scale->width = player->transform->scale->width;

			Missile* missile = new Missile(trans);

			updateMissileTarget(missile);

			Objects.push_back(missile);
			Missiles.push_back(missile);

			trans = nullptr;
			missile = nullptr;
		}
	}

	// Update Objects
	for (Entity* element : Objects)
	{
		element->update(deltaTime);
	}

	// Check if player is near a wall
	if (!GameOver)
	{
		playerWallCheck();
	}

	// Check Asteroid and Bullet positions, destroy if out of the arena
	for (Entity* entity : Objects)
	{
		if (entity->tag == Tag::ASTEROID)
		{
			Asteroid* asteroid = (Asteroid*)entity;
			if (asteroid->inArena == false)
			{
				if (asteroid->transform->position->x > (-arenaWidth + asteroid->collisionRadius) &&
					asteroid->transform->position->x < (arenaWidth - asteroid->collisionRadius) &&
					asteroid->transform->position->y > (-arenaHeight + asteroid->collisionRadius) &&
					asteroid->transform->position->y < (arenaHeight - asteroid->collisionRadius))
				{
					asteroid->inArena = true;
				}
			}

			if (checkArenaCollision(asteroid) &&
				asteroid->inArena == true)
			{
				destroyObject(entity);
			}

			asteroid = nullptr;
		}

		if (entity->tag == Tag::BULLET)
		{
			if (checkArenaCollision(entity))
			{
				destroyObject(entity);
			}
		}

	}

	// Check collisions -- Doubling collisions
	for (Entity* entity1 : Objects)
	{
		// Cycle through all other entity objects and check for collions
		for (Entity* entity2 : Objects)
		{
			if (entity1 != entity2 &&
				entity1->tag != entity2->tag)
			{
				if (checkCollision(entity1, entity2))
				{
					entity1->collisionResult = entity1->onCollision(entity2);
					//entity2->collisionResult = entity2->onCollision(entity1);
				}
			}
		}
	}

	if (!GameOver)
	{
		if (checkArenaCollision(player))
		{
			player->collisionResult = Result::GAMEOVER;
		}
	}

	// Process all collisions
	for (Entity* entity : Objects)
	{
		processCollision(entity);
	}

	for (Entity* entity : Objects)
	{
		for (int key : keyPresses)
		{
			entity->keyPress(key);
		}
		keyPresses.clear();
		for (int key : keyReleases)
		{
			entity->keyRelease(key);
		}
		keyReleases.clear();
	}

	for (Missile* missile : Missiles)
	{
		if (missile->target == nullptr)
		{
			updateMissileTarget(missile);
		}
	}

	overlay->update(asteroidsDestroyed, (glutGet(GLUT_ELAPSED_TIME) / 1000) - elapsedTimeOffset);
}

bool SceneGraph::checkCollision(Entity* e1, Entity* e2)
{
	const float dx = e2->transform->position->x - e1->transform->position->x;
	const float dy = e2->transform->position->y - e1->transform->position->y;
	const float radii = e1->collisionRadius + e2->collisionRadius;

	if ( (radii * radii) >= (dx * dx) + (dy * dy))
	{
		return true;
	}

	return false;
}

bool SceneGraph::checkArenaCollision(Entity* entity)
{
	float xPos = entity->transform->position->x;
	float yPos = entity->transform->position->y;

	// Top Wall
	if (yPos + entity->collisionRadius > arenaHeight)
	{
		return true;
	}

	// Bottom Wall
	if (yPos - entity->collisionRadius < -arenaHeight)
	{
		return true;
	}

	// Left Wall
	if (xPos - entity->collisionRadius < -arenaWidth)
	{
		return true;
	}

	//Right Wall
	if (xPos + entity->collisionRadius > arenaWidth)
	{
		return true;
	}

	return false;
}

void SceneGraph::playerWallCheck()
{
	//left wall check
	if ((player->transform->position->x - (player->collisionRadius * 8)) < -arenaWidth)
	{
		arena->colourChange(Direction::LEFT, true);
	}
	else
	{
		arena->colourChange(Direction::LEFT, false);
	}
	//right wall check
	if ((player->transform->position->x + (player->collisionRadius * 8)) > arenaWidth)
	{
		arena->colourChange(Direction::RIGHT, true);
	}
	else
	{
		arena->colourChange(Direction::RIGHT, false);
	}
	//top wall check
	if ((player->transform->position->y + (player->collisionRadius * 8)) > arenaHeight)
	{
		arena->colourChange(Direction::TOP, true);
	}
	else
	{
		arena->colourChange(Direction::TOP, false);
	}
	//bottom wall check
	if ((player->transform->position->y - (player->collisionRadius * 8)) < -arenaHeight)
	{
		arena->colourChange(Direction::BOTTOM, true);
	}
	else
	{
		arena->colourChange(Direction::BOTTOM, false);
	}
}

// Called by main.display(), loop all objects and call object.render()
void SceneGraph::render()
{
	for (Entity* element : Objects)
	{
		element->render();
	}
	arena->render();
	overlay->render();
	if (GameOver)
	{
		renderGameOver();
	}
}

void SceneGraph::renderGameOver()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// Set the projection - use this to render text
	gluOrtho2D(0, arenaWidth, 0, arenaHeight);
	glMatrixMode(GL_MODELVIEW);

	int x = arenaWidth / 3;
	int y = arenaHeight / 2;
	std::string string = "Game Over. Press any key to play again...except ESC, that will end things real quick";

	glRasterPos2f(x, y);
	int len = string.length();
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}

	glMatrixMode(GL_PROJECTION);

	// Restore the original projection matrix for rendering everything else
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
}

// Called by main when user presses a key, loop all objects and call object.keyPress()
void SceneGraph::keyPress(int keyCode)
{
	if (GameOver)
	{
		GameOver = false;
		reset();
	}
	keyPresses.push_back(keyCode);
}

void SceneGraph::keyRelease(int keyCode)
{
	keyReleases.push_back(keyCode);
}

void SceneGraph::mouseClick(int button, int state)
{
	for (Entity* element : Objects)
	{
		element->mousePressed(button, state);
	}
}

// Creates a new Object and adds to Obejcts list, call entity.init()
void SceneGraph::createObject(Tag type)
{
	switch (type)
	{
	case Tag::ASTEROID:
		createAsteroid();
		break;
	case Tag::PLAYER:
		break;
	case Tag::BULLET:
		break;
	case Tag::ARENA:
		break;
	default:
		break;
	}
}

void SceneGraph::createAsteroid()
{
	Point spawn;

	int randomQuadrant = math->randomInt(0, 3);

	if (randomQuadrant == 0)
	{
		// Top Quad
		spawn.x = math->randomInt(-arenaWidth, arenaWidth);
		spawn.y = arenaHeight + 5;
	}
	else if (randomQuadrant == 1)
	{
		// Bottom Quad
		spawn.x = math->randomInt(-arenaWidth, arenaWidth);
		spawn.y = (arenaHeight * -1) - 5;
	}
	else if (randomQuadrant == 2)
	{
		// Left Quad
		spawn.x = (arenaWidth * -1) - 5;
		spawn.y = math->randomInt(-arenaHeight, arenaHeight);
	}
	else
	{
		// Right Quad
		spawn.x = arenaWidth + 5;
		spawn.y = math->randomInt(-arenaHeight, arenaHeight);
	}
	if (GameOver)
	{
		Point* point = new Point();
		point->x = 0;
		point->y = 0;
		Objects.push_back(new Asteroid(*point, spawn));
		delete point;
		point = nullptr;
	}
	else
	{
		Objects.push_back(new Asteroid(*player->transform->position, spawn));
	}
}

// Removes an object from the Objects list, call object deconstructor via destroy(Object)
void SceneGraph::destroyObject(Entity* entity)
{
	Entity* temporaryPtr = entity;

	if (entity->tag == Tag::MISSILE)
	{
		Missiles.erase(std::remove(Missiles.begin(), Missiles.end(), entity), Missiles.end());
	}

	Objects.erase(std::remove(Objects.begin(), Objects.end(), entity), Objects.end());

	for (Missile* missile : Missiles)
	{
		if (missile->target == temporaryPtr)
		{
			updateMissileTarget(missile);
		}
	}

	delete temporaryPtr;
	temporaryPtr = nullptr;
}

void SceneGraph::processCollision(Entity* entity)
{
	switch (entity->collisionResult)
	{
	case Result::DELETED:
		if (entity->tag == Tag::ASTEROID)
		{
			asteroidsDestroyed++;
		}
		destroyObject(entity);
		break;
	case Result::GAMEOVER:
		GameOver = true;
		destroyObject(player);
		break;
	case Result::NOTHING:
		break;
	default:
		break;
	}
}

void SceneGraph::updateMissileTarget(Missile* missile)
{
	Entity* target = nullptr;
	float targetVectorLength;

	for (Entity* entity : Objects)
	{
		if (entity->tag == Tag::ASTEROID)
		{
			float vectorLength = math->vectorLength(entity->transform->position->x - missile->transform->position->x, entity->transform->position->y - missile->transform->position->y);
			if (target == nullptr)
			{
				target = entity;
				targetVectorLength = vectorLength;
			}
			else if (vectorLength < targetVectorLength)
			{
				target = entity;
				targetVectorLength = vectorLength;
			}
		}
	}

	missile->target = target;
}