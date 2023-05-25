#pragma once
#include "SDL.h"
#include "Player.hpp"
#include "Asteroid.hpp"

class Bullet
{
	friend Asteroid;
public:
	Bullet(SDL_Renderer* renderer, Player* player, int dirX, int dirY);
	void update();
	void draw();

	bool collide(const SDL_Rect& asteroid);

	bool active = true;

	static inline SDL_Texture* texture;
private:
	SDL_Renderer* renderer;

	const SDL_Rect src{ 0, 0, 16, 16 };
	SDL_Rect dest;
	int dirX;
	int dirY;
	
	double angle;
};

