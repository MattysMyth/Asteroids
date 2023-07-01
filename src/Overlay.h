#pragma once
#include "UI.h"
#include <vector>

struct Overlay : UI
{
	Overlay(int screenWidth, int screenHeight);
	~Overlay();

	void render() override;
	void update(int asteroidsDestroyed, int elapsedTime) override;

	std::vector<Element*> elements;

	int screenWidth;
	int screenHeight;

	Element* elapsedTime;
	Element* asteroidsDestroyed;
};