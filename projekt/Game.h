#pragma once
#include <stdint.h>
#include "SerialTerminal.h"


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


class Game
{
	SerialTerminal* serial = nullptr;
	bool paused;
	unsigned int score;
	unsigned int speed;
	BorderPos border = { 1, DISPLAY_WIDTH - 1, 1, DISPLAY_HEIGHT - 1 };

public:	
	Game(SerialTerminal* aSerial);
	void DrawBorder(const char character);
};