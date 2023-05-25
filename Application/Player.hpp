#pragma once

#include "SDL.h"
#include "SDL_image.h"

class Player
{
public:
	Player(SDL_Renderer* renderer);
	~Player();
	void set_direction(int dirX, int dirY);
	void set_shoot(int shootX, int shootY);

	void heal_up();

	bool collide(const SDL_Rect& asteroid);

	int get_x() const;
	int get_y() const;
	int get_health() const;

	void update();
	void draw();
private:
	SDL_Texture* texture;
	SDL_Renderer* renderer;

	const SDL_Rect src{ 0, 0, 16, 16 };
	SDL_Rect dest;
	int dirX;
	int dirY;
	int shootX;
	int shootY;
	double angle;
	int health;
};

