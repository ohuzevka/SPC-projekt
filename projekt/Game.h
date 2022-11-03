#pragma once
#include <stdint.h>
#include "SerialTerminal.h"
#include "WindowsTerminal.h"

// select output terminal:
#define WINDOWS_TERMINAL
#define SERIAL_TERMINAL

enum Dir	// Direction of snake movement
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

struct Chain	// One chain of snake
{
	uint8_t posX;
	uint8_t posY;
};

class Snake
{
	static const unsigned int maxLenght = 50;
	uint8_t lenght;
	Dir dir;
	Chain chain[maxLenght];

public:
	void Draw();

	void Move();
};

class Game
{
	bool running;
	unsigned int score;
	unsigned int speed;
	Snake snake;

public:
	SerialTerminal serial;
	WindowsTerminal windows;
};