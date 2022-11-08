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
	bool apause;
	element snake[20];

	uint8_t type;
	uint8_t XiX;
	uint8_t YiY;
public:
	uint8_t end = 0;
	void init(SerialTerminal* serial);
	void addElement();
	void move(SerialTerminal* serial);
	void draw(SerialTerminal* serial);

	void changeDir(direction dir);
	void pause();
	void play();
	bool state();
};

