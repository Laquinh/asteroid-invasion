#pragma once

#include <iostream>
#include "SDL.h"

#include <vector>
#include "Asteroid.hpp"
#include "Bullet.hpp"
#include "Player.hpp"

class Game
{
public:
	void init();
	void update();
	void handle_events();
	void render();
	void clean();
	void generate_asteroid();
	void generate_bullet(int inputX, int inputY);

	static inline SDL_Window* window;
	static inline SDL_Renderer* renderer;

	SDL_Texture* texture;
	
	std::vector<std::unique_ptr<Asteroid>> asteroids;
	std::vector<std::unique_ptr<Bullet>> bullets;

	bool running = true;
	Player* player;
	int score;
};

