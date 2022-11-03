#pragma once
#include <stdint.h>
#include "SerialTerminal.h"
#include "WindowsTerminal.h"

// select output terminal:
#define WINDOWS_TERMINAL
//#define SERIAL_TERMINAL

enum Dir	// Direction of snake movement
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

struct Pos	// Position on terminal
{
	uint8_t X;
	uint8_t Y;
};

struct BorderPos
{
	uint8_t leftPos;
	uint8_t rightPos;
	uint8_t topPos;
	uint8_t bottomPos;
};

class Snake
{
	static const unsigned int maxLenght = 50;
	uint8_t lenght;
	Dir dir;
	Pos chain[maxLenght];

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
	BorderPos border = { 1, DISPLAY_WIDTH - 1, 1, DISPLAY_HEIGHT - 1 };

public:
	SerialTerminal serial;
	WindowsTerminal windows;

	void DrawBorder(const char character);
};