#pragma once
#include "SDL.h"
#include "Player.hpp"

class Asteroid
{
public:
	Asteroid(SDL_Renderer* renderer, Player* player);
	void update();
	void draw();

	SDL_Rect get_dest();

	bool active = true;
	int health;

	static inline SDL_Texture* texture;
private:
	SDL_Renderer* renderer;

	const SDL_Rect src{ 0, 0, 32, 32 };
	SDL_Rect dest;

	double angle;
	int dirX;
	int dirY;
};

