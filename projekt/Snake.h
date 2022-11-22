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
	Position snakeElement[100];
	Direction direction;
	Direction oldDiderction;
	const char headChar = 'O';
	const char bodyChar = 'o';
	const uint8_t snakeBgColor = BLACK;
	const uint8_t snakeTextColor = WHITE;
	uint8_t speed;
	BorderPos border;
	Position food;
	uint8_t type;
	uint8_t lenght;
	Status state;
public:	
	bool speedChangedFlag;
	bool stateChangedFlag;

	Snake(SerialTerminal* aSerial);
	void init();
	void generateFood();
	void addElement();
	void move();
	void drawSnake();
	void redrawSnake();
	void drawBorder(const char character, uint8_t bgColor, uint8_t textColor);
	void changeDir(Direction dir);
	Status status();
	void pause();
	void play();
	void gameOver();
	void increaseSpeed();
	void decreaseSpeed();
	uint8_t getSpeed();
	void printSpeed();
	void printLenght();
	void printStatus();
	void printFood();
	void draw();
};

