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

enum Status
{
	PAUSED,
	RUNNING,
	GAME_OVER
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
	Position snakeElement[50];
	Direction direction;
	Direction oldDiderction;
	const char headChar = '@';
	const char bodyChar = 'O';
	uint8_t speed;
	BorderPos border;
	Position food;
	uint8_t type;
	uint8_t lenght;
	Status state;
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
	Status status();
	void pause();
	void play();
	void gameOver();
	void increaseSpeed();
	void decreaseSpeed();
	uint8_t getSpeed();
	void printSpeed();
	bool speedChangedFlag;
};

