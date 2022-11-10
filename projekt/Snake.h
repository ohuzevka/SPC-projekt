#pragma once

#include <iostream>
#include "SerialTerminal.h"

enum direction
{
	UPWARDS,
	DOWNWARDS,
	LEFTWARDS,
	RIGHTWARDS
};

struct element
{
	uint8_t iX;
	uint8_t iY;

	direction dir;
};

class Snake
{
	SerialTerminal* serial = nullptr;
	bool apause;
	element snake[20];
	const char headChar = '@';
	const char bodyChar = 'O';
	uint8_t type;
	uint8_t XiX;
	uint8_t YiY;
public:
	uint8_t end = 0;
	Snake(SerialTerminal* aSerial);
	void init();
	void addElement();
	void move();
	void draw();
	void redraw();

	void changeDir(direction dir);
	void pause();
	void play();
	bool state();
};

