#include "Asteroid.hpp"
#include "RNG.hpp"
#include <cmath>

Asteroid::Asteroid(SDL_Renderer* renderer, Player* player):
	health(2),
	renderer(renderer)
{
	dest.w = dest.h = 64;

	//zones 0 and 1 are, respectively, top and bottom borders of the window.
	//zones 2 and 3 are, respectively, left and right borders of the window.

	int zone = RNG::generate<0, 3>(); //randomly choose a zone to spawn in
	switch (zone)
	{
	case 0:
		dest.x = RNG::generate<0, 1080>();
		dest.y = RNG::generate<0, 20>();
		break;
	case 1:
		dest.x = RNG::generate<0, 1080>();
		dest.y = RNG::generate<700, 720>();
		break;
	case 2:
		dest.x = RNG::generate<0, 20>();
		dest.y = RNG::generate<0, 720>();
		break;
	case 3:
		dest.x = RNG::generate<1060, 1080>();
		dest.y = RNG::generate<0, 720>();
		break;
	}

	//the direction of the asteroid will be towards the player's position at the time the asteroid spawns.
	//it's necessary to normalise the vector, so that all asteroids have the same speed.
	int tempX = player->get_x() - dest.x;
	int tempY = player->get_y() - dest.y;

	int modulus = sqrt(tempX * tempX + tempY * tempY);

	dirX = tempX * 75 / modulus;
	dirY = tempY * 75 / modulus;

	angle = RNG::generate<0, 359>();

	SDL_Surface* surface = IMG_Load("media/asteroid.png");
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

void Asteroid::update()
{
	dest.x += dirX / 10;
	dest.y += dirY / 10;

	if (dest.x + 64 < 0 || dest.x - 64 > 1080 ||
		dest.y + 64 < 0 || dest.y - 64 > 720 ||
		health <= 0)
	{
		active = false; //disable asteroid if it gets out of bounds or has no health left
	}
	else
	{
		active = true;
	}

	angle += 2; //slightly rotate it
}

void Asteroid::draw()
{
	SDL_RenderCopyEx(renderer, texture, &src, &dest, angle, NULL, SDL_FLIP_NONE);
}

SDL_Rect Asteroid::get_dest()
{
	return dest;
}