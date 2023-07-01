#include "Math.h"

Math::Math()
{

}

Math::~Math()
{

}

float Math::dotProduct(float x1, float x2, float y1, float y2)
{
	float dx = x1 * x2;
	float dy = y1 * y2;
	return dx + dy;
}

float Math::vectorLength(float x, float y)
{
	float normalX = sqrt(x * x);
	float normalY = sqrt(y * y);

	return normalX + normalY;
}

float Math::directionVector(float destinationCoordinate, float originCoordinate, float vectorLength)
{
	return (destinationCoordinate - originCoordinate) / vectorLength;
}

int Math::randomInt(int min, int max)
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> UniDis(min, max);

	return UniDis(rng);
}

float Math::randomFloat(float min, float max)
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_real_distribution<float> UniDis(min, max);

	return UniDis(rng);
}