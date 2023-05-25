#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

#include "RNG.hpp"

#include "SDL.h"
#include "SerialPort.hpp"
#include "Game.hpp"

#undef main

int convert(char input) //convert from unsigned char to signed int
{
	int output;
	if (input < 0)
	{
		output = input + 256;
	}
	else
	{
		output = input;
	}
	output -= 127;
	if (output >= -3 && output <= 3) //as joysticks are not flawless, it's common for them to have an error of 1-2 in their values.
									//As a result, the player would be moving even when not touching the joystick. To avoid this, values near 0
									//are rounded to 0.
	{
		output = 0;
	}
	return output;
}

//This function will communicate with Arduino. It'll be responsible for both sending to and receiving data from it.
void serial_connection(Game& game)
{
	std::string port("\\\\.\\COM3");

	SerialPort* arduino = new SerialPort(port.c_str());

	while (!arduino->isConnected()) { //keeps trying until manages to connect with Arduino.
		delete arduino;
		Sleep(200);
		arduino = new SerialPort(port.c_str());
	}

	while (arduino->isConnected() && game.running)
	{
		char incomingData[5];
		int readResult = arduino->readSerialPort(incomingData, 5); //reads 5 bytes from the Serial buffer

		if (incomingData[0] == 'A') //the first byte of data being 'A' means this information was sent from Arduino
		{
			game.player->set_direction(convert(incomingData[1]), convert(incomingData[2])); //updates player's movement direction

			int shootX = convert(incomingData[3]);
			int shootY = convert(incomingData[4]);
			game.player->set_shoot(shootX, shootY); //updates player's shooting direction
			if (shootX || shootY)
			{
				game.generate_bullet(shootX, shootY); //if the shooting direction's joystic is actually moved, a bullet will be generated
			}
		}

		char numberData[2];
		numberData[0] = 'C';
		numberData[1] = char(game.player->get_health()); 
		arduino->writeSerialPort(numberData, 2); //sends player's health to Arduino

		Sleep(50);
	}
	game.running = false;
	delete arduino;
}

void play(Game& game)
{
	game.init();
	int asteroidClock = 0;
	while (game.running)
	{
		if (++asteroidClock == 100)
		{
			game.generate_asteroid(); //every 20 frames, a new asteroid spawns
			asteroidClock = 0;
		}
		game.update();
		game.handle_events();
		game.render();
		Sleep(50);
	}
	std::cout << "Score: " << game.score; //when the game ends, the final score will be shown in the console
	game.clean();
}

int main()
{
	Game game;

	//multi-threading, so as for the Serial connection and the actual videogame to run independently.
	std::thread connectionThread(serial_connection, std::ref(game));
	std::thread gameThread(play, std::ref(game));

	if (gameThread.joinable())
	{
		gameThread.join();
	}
	if (connectionThread.joinable())
	{
		connectionThread.join();
	}
	return 0;
}