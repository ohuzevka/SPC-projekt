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
	element snake[20];
	uint8_t type;
	uint8_t XiX;
	uint8_t YiY;
public:
	bool apause;
	uint8_t end = 0;
	Snake(SerialTerminal* aSerial);
	void init();
	void addElement();
	void move();
	void draw();

	void changeDir(direction dir);
	void pause();
	void play();
	bool state();
};

