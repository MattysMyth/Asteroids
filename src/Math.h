#pragma once
#include <math.h>
#include <random>

struct Scale {
	float width;
	float height;
};

struct Point
{
	float x;
	float y;
};

struct Vector
{
	float x;
	float y;
};

struct Math
{
	Math();
	~Math();

	float dotProduct(float x1, float x2, float y1, float y2);
	float vectorLength(float x, float y);
	float directionVector(float destinationCoordinate, float originCoordinate, float vectorLength);
	int randomInt(int min, int max);
	float randomFloat(float min, float max);
};