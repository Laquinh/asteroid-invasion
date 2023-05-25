#include "Player.hpp"
#include <iostream>

Player::Player(SDL_Renderer* renderer):
	dest({ 100, 100, 32, 32 }),
	dirX(0),
	dirY(0),
	shootX(0),
	shootY(0),
	health(3),
	renderer(renderer)
{
	SDL_Surface* surface = IMG_Load("media/player.png");
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

Player::~Player()
{
	SDL_DestroyTexture(texture);
}

void Player::set_direction(int dirX, int dirY)
{
	this->dirX = double(dirX)/1.2;
	this->dirY = double(dirY)/1.2;
}

void Player::set_shoot(int shootX, int shootY)
{
	this->shootX = shootX;
	this->shootY = shootY;
}

void Player::heal_up()
{
	++health;
}

int Player::get_x() const
{
	return dest.x;
}

int Player::get_y() const
{
	return dest.y;
}

int Player::get_health() const
{
	return health;
}

void Player::update()
{
	dest.x += dirX / 10;
	dest.y += dirY / 10;

	if (dirX == 0)
	{
		if (dirY > 0)
		{
			angle = 90;
		}
		else
		{
			angle = 270;
		}
	}
	else if (dirX > 0)
	{
		angle = atan(double(dirY) / double(dirX)) * 180 / M_PI;
	}
	else
	{
		angle = 180 + atan(double(dirY) / double(dirX)) * 180 / M_PI;
	}
}

bool Player::collide(const SDL_Rect& asteroid)
{
	if (dest.x < asteroid.x + asteroid.w &&
		dest.x + dest.w > asteroid.x &&
		dest.y < asteroid.y + asteroid.h &&
		dest.y + dest.h > asteroid.y)
	{
		--health;
		return true;
	}

	return false;
}

void Player::draw()
{
	SDL_RenderCopyEx(renderer, texture, &src, &dest, angle, NULL, SDL_FLIP_NONE);
}
