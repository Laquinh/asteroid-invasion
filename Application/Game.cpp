#include "Game.hpp"
#include "SDL.h"
#include "SDL_ttf.h"
#include <sstream>

void Game::init()
{
	if (!SDL_Init(SDL_INIT_EVERYTHING))
	{
		window = SDL_CreateWindow("AsteroidInvasion", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 720, 0); //initialise window
		renderer = SDL_CreateRenderer(window, -1, 0); //initialise renderer
	}

	TTF_Init(); //initialise fonts' extension

	player = new Player(renderer);
	score = 0;

	SDL_Surface* surface = IMG_Load("media/background.png");
	texture = SDL_CreateTextureFromSurface(renderer, surface); //make a texture for the background
	SDL_FreeSurface(surface);
}

void Game::handle_events()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT: //window's X button (top righthand corner)
		running = false;
	}
}

void Game::update()
{
	player->update(); //update player

	for (auto& a : asteroids) //update all asteroids
	{
		a->update();
	}
	for (auto& b : bullets) //update all bullets
	{
		b->update();
	}

	for (auto& a : asteroids)
	{
		for (auto& b : bullets)
		{
			if (b->collide(a->get_dest())) //check for collision between asteroids and bullets
			{
				--a->health; //if a collision happened, asteroid's health is decreased
				if ((++score) % 20 == 0 && player->get_health() < 9)
				{
					player->heal_up(); //if the shot was successful, the score is increased by 1
				}
			}
		}
		if (player->collide(a->get_dest())) //check for collision between asteroids and the player
		{
			a->active = false; //if a collision happened, the asteroid becomes inactive
		}
	}

	//if an asteroid or a bullet is not active, delete theme and erase them from the vector

	asteroids.erase(std::remove_if(asteroids.begin(), asteroids.end(), [](const std::unique_ptr<Asteroid>& a) {
			return !a->active;
		}),
		asteroids.end());
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const std::unique_ptr<Bullet>& b) {
			return !b->active;
		}),
		bullets.end());

	if (player->get_health() < 0)
	{
		running = false; //if the player's health is bellow 0, the game finishes
	}
}

void Game::render()
{
	SDL_RenderCopy(renderer, texture, NULL, NULL); //print the background

	//draw player, asteroids and bullets
	player->draw();
	for (auto& a : asteroids)
	{
		a->draw();
	}
	for (auto& b : bullets)
	{
		b->draw();
	}

	//make texture for the text showing the player's score

	std::stringstream ss;

	ss << "Score: " << score;

	TTF_Font* arial = TTF_OpenFont("arial.ttf", 20);
	SDL_Color white = { 255, 255, 255 };

	SDL_Surface* scoreSurface = TTF_RenderText_Solid(arial, ss.str().c_str(), white);

	// now you can convert it into a texture
	SDL_Texture* scoreText = SDL_CreateTextureFromSurface(renderer, scoreSurface);

	SDL_Rect scoreDestination = { 950, 20, 80, 25 };
	SDL_RenderCopy(renderer, scoreText, NULL, &scoreDestination);

	SDL_DestroyTexture(scoreText);
	SDL_FreeSurface(scoreSurface);

	//render everything
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	delete player;
	TTF_Quit();
	SDL_DestroyTexture(Asteroid::texture);
	SDL_DestroyTexture(Bullet::texture);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Game::generate_asteroid()
{
	asteroids.emplace_back(std::make_unique<Asteroid>(renderer, player));
}

void Game::generate_bullet(int inputX, int inputY)
{
	bullets.emplace_back(std::make_unique<Bullet>(renderer, player, inputX, inputY));
}