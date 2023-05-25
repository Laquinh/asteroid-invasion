#include "Bullet.hpp"
#include <cmath>

Bullet::Bullet(SDL_Renderer* renderer, Player* player, int inputX, int inputY):
	renderer(renderer)
{
	dest.w = 64;
	dest.h = 4;

	dest.x = player->get_x();
	dest.y = player->get_y();

	//normalise the vector first, as we want all bullets to be shot at the same speed
	int modulus = sqrt(inputX * inputX + inputY * inputY);

	dirX = inputX * 600 / modulus;
	dirY = inputY * 600 / modulus;

	if (dirX == 0)
	{
		angle = 90;
	}
	else
	{
		angle = atan(double(dirY) / double(dirX)) * 180 / M_PI;
	}

	SDL_Surface* surface = IMG_Load("media/bullet.png");
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

void Bullet::update()
{
	dest.x += dirX / 10;
	dest.y += dirY / 10;

	if (dest.x + 64 < 0 || dest.x - 64 > 1080 || //if the bullet is out of bounds, inactivate it
		dest.y + 64 < 0 || dest.y - 64 > 720)
	{
		active = false;
	}
	else
	{
		active = true;
	}
}

void Bullet::draw()
{
	SDL_RenderCopyEx(renderer, texture, &src, &dest, angle, NULL, SDL_FLIP_NONE);
}

bool Bullet::collide(const SDL_Rect& asteroid)
{
	if (dest.x < asteroid.x + asteroid.w && //check AABB collision with an asteroid
		dest.x + dest.w > asteroid.x &&
		dest.y < asteroid.y + asteroid.h &&
		dest.y + dest.h > asteroid.y)
	{
		active = false; //if a collision is found, then the bullet gets deactivated
		return true;
	}

	return false;
}

