#pragma once

#include <iostream>
#include "SerialTerminal.h"

enum Direction	// Direction of snake movement
{
	STILL,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

struct Position	// Position on terminal
{
	uint8_t iX;
	uint8_t iY;
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
	SerialTerminal* serial = nullptr;
	bool apause;
	Position snakeElement[50];
	Direction direction;
	const char headChar = '@';
	const char bodyChar = 'O';
	unsigned int score;
	unsigned int speed;
	BorderPos border;
	Position food;
	uint8_t type;
	uint8_t lenght;
public:
		
	Snake(SerialTerminal* aSerial);
	void init();
	void spawn(uint8_t SpawnPosX = 0, uint8_t SpawnPosY = 0);
	void generateFood();
	void addElement();
	void move();
	void draw();
	void redraw();
	void drawBorder(const char character);
	void changeDir(Direction dir);
	void pause();
	void play();
	bool state();
};

