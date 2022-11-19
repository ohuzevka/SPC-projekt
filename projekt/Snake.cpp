#include <cstdlib>
#include <stdio.h>
#include "Snake.h"
#include "SerialTerminal.h"

Snake::Snake(SerialTerminal* aSerial)
{
	serial = aSerial;
	state = PAUSED;
	oldDiderction = direction = DOWN;
	speed = 10;
	speedChangedFlag = false;

	border.topPos = 5;
	border.bottomPos = DISPLAY_HEIGHT;
	border.leftPos = 0;
	border.rightPos = DISPLAY_WIDTH;
}

void Snake::init()
{
	state = PAUSED;

	snakeElement[0].iX = 40;
	snakeElement[0].iY = 20;

	snakeElement[1].iX = 40;
	snakeElement[1].iY = 19;

	oldDiderction = direction = DOWN;
	lenght = 2;

	serial->Clear(BLACK);

	serial->SetPos(1, 1); serial->Print("Pause: SPACE", BLACK, WHITE);
	serial->SetPos(1, 2); serial->Print("Speed: +,-", BLACK, WHITE);
	serial->SetPos(1, 3); serial->Print("Move:  W,S,A,D", BLACK, WHITE);

	printStatus();
	serial->SetPos(68, 2); serial->Print("Speed:  ", BLACK, WHITE); printSpeed();
	serial->SetPos(68, 3); serial->Print("Lenght: ", BLACK, WHITE); printLenght();

	serial->SetPos(28, 0); serial->Print(R"(   ____          __      )", BLACK, WHITE);
	serial->SetPos(28, 1); serial->Print(R"(  / __/__  ___ _/ /_____ )", BLACK, WHITE);
	serial->SetPos(28, 2); serial->Print(R"( _\ \/ _ \/ _ `/  '_/ -_))", BLACK, WHITE);
	serial->SetPos(28, 3); serial->Print(R"(/___/_//_/\_,_/_/\_\\__/ )", BLACK, WHITE);
	serial->SetPos(28, 4); serial->Print(R"(                         )", BLACK, WHITE);
	
	drawBorder('#', WHITE, BLACK);
	draw();

	generateFood();
}

void Snake::addElement()
{
}

void Snake::move()
{
	// Test if it crashed to border
	if (snakeElement[0].iX == border.leftPos || snakeElement[0].iX == border.rightPos ||
		snakeElement[0].iY == border.bottomPos || snakeElement[0].iY == border.topPos)
	{
		gameOver();
		return;
	}

	// Test if snake crashed into itself
	for (uint8_t i = 1; i < lenght; i++)
	{
		if (snakeElement[0].iX == snakeElement[i].iX && snakeElement[0].iY == snakeElement[i].iY)
		{
			gameOver();
			return;
		}
	}

	if ((snakeElement[0].iX == food.iX) && (snakeElement[0].iY == food.iY))
	{
		++lenght;
		printLenght();

		snakeElement[lenght].iX = snakeElement[lenght - 1].iX;
		snakeElement[lenght].iY = snakeElement[lenght - 1].iY;

		generateFood();
	}

	// Shift elements
	for (int8_t i = lenght + 1; i > 0; --i)
	{
		snakeElement[i] = snakeElement[i - 1];
	}
	
	oldDiderction = direction;

	// New head position
	switch (direction)
	{
	case DOWN:
		++snakeElement[0].iY;
		break;
	case UP:
		--snakeElement[0].iY;
		break;
	case LEFT:
		--snakeElement[0].iX;
		break;
	case RIGHT:
		++snakeElement[0].iX;
		break;
	}

	redraw();
}

void Snake::draw()
{
	// Draw head of snake
	serial->SetPos(snakeElement[0].iX, snakeElement[0].iY);
	serial->Print(headChar, snakeBgColor, snakeTextColor);

	// Draw body of snake
	for (uint8_t i = 1; i < lenght; ++i)
	{
		serial->SetPos(snakeElement[i].iX, snakeElement[i].iY);
		serial->Print(bodyChar, snakeBgColor, snakeTextColor);
	}
}

void Snake::redraw()
{
	// Draw new head of snake
	serial->SetPos(snakeElement[0].iX, snakeElement[0].iY);
	serial->Print(headChar, snakeBgColor, snakeTextColor);

	// Make the old head a body
	if (lenght > 1)
	{
		serial->SetPos(snakeElement[1].iX, snakeElement[1].iY);
		serial->Print(bodyChar, snakeBgColor, snakeTextColor);
	}

	// Erase the last node of snake
	serial->SetPos(snakeElement[lenght].iX, snakeElement[lenght].iY);
	serial->Print(' ', BLACK, WHITE);
}

void Snake::generateFood()
{
	bool flag = true;
	while (flag == true)
	{
		srand((unsigned)time(NULL));

		food.iX = border.leftPos + 1 + ((uint8_t)rand() % (border.rightPos - border.leftPos - 2));
		food.iY = border.topPos + 1 + ((uint8_t)rand() % (border.bottomPos - border.topPos - 2));

		flag = false;
		for (uint8_t i = 1; i < lenght; i++)
		{
			if (food.iX == snakeElement[i].iX && food.iY == snakeElement[i].iY)
				flag = true;
		}
	}
	serial->SetPos(food.iX, food.iY);
	serial->Print('@', BLACK, RED);
}

void Snake::drawBorder(const char character, uint8_t bgColor, uint8_t textColor)
{
	if (border.leftPos >= border.rightPos || border.topPos >= border.bottomPos)
		serial->Print("Invalid border dimensions", BLACK, WHITE);
	else
	{
		serial->SetPos(border.leftPos, border.topPos);
		for (int i = 0; i <= border.rightPos; i++)	// top
			serial->Print(character, bgColor, textColor);
		for (int i = border.topPos + 1; i < border.bottomPos; i++)		// left
		{
			serial->SetPos(border.leftPos, i);
			serial->Print(character, bgColor, textColor);
		}
		for (int i = border.topPos + 1; i < border.bottomPos; i++)		// right
		{
			serial->SetPos(border.rightPos, i);
			serial->Print(character, bgColor, textColor);
		}
		serial->SetPos(border.leftPos, border.bottomPos);
		for (int i = 0; i <= border.rightPos; i++)	// bottom
			serial->Print(character, bgColor, textColor);
	}
}

void Snake::changeDir(Direction dir)
{
	if ((dir == LEFT && oldDiderction == RIGHT) || (dir == RIGHT && oldDiderction == LEFT) ||
		(dir == UP && oldDiderction == DOWN) || (dir == DOWN && oldDiderction == UP))
		return;
	direction = dir;
}

void Snake::pause()
{
	state = PAUSED;
	stateChangedFlag = true;
}

void Snake::play()
{
	state = RUNNING;
	stateChangedFlag = true;
}

Status Snake::status()
{
	return state;
}

void Snake::gameOver()
{
	state = GAME_OVER;
	serial->SetPos(68, 1); serial->Print("         ", BLACK, BLACK);

	serial->SetPos(snakeElement[0].iX, snakeElement[0].iY); serial->Print(headChar, BLACK, RED);
	
	/*
	serial->SetPos(35, 1); serial->Print("Game Over", RED, BLACK);
	serial->SetPos(29, 2); serial->Print("Press SPACE to restart", RED, BLACK);
	*/
	
	serial->SetPos(17, 0); serial->Print(R"(  _____                   ____                 )", BLACK, RED);
	serial->SetPos(17, 1); serial->Print(R"( / ___/___ _ __ _  ___   / __ \ _  __ ___  ____)", BLACK, RED);
	serial->SetPos(17, 2); serial->Print(R"(/ (_ // _ `//  ' \/ -_) / /_/ /| |/ // -_)/ __/)", BLACK, RED);
	serial->SetPos(17, 3); serial->Print(R"(\___/ \_,_//_/_/_/\__/  \____/ |___/ \__//_/   )", BLACK, RED);

	serial->SetPos(1, 1); serial->Print("            ", BLACK, BLACK);
	serial->SetPos(1, 2); serial->Print("Press ", BLACK, WHITE); serial->Print(" SPACE ", WHITE, BLACK);
	serial->SetPos(1, 3); serial->Print("to try again  ", BLACK, WHITE);
}

void Snake::increaseSpeed()
{
	if (++speed > 99)
		speed = 99;

	speedChangedFlag = true;
}

void Snake::decreaseSpeed()
{
	if (--speed < 1)
		speed = 1;

	speedChangedFlag = true;
}

uint8_t Snake::getSpeed()
{
	return speed;
}

void Snake::printSpeed()
{
	char speedString[3];
	sprintf_s(speedString, 3, "%2d", speed);
	serial->SetPos(76, 2); serial->Print(speedString, BLACK, WHITE);
}

void Snake::printLenght()
{
	char lenghtString[3];
	sprintf_s(lenghtString, 3, "%2d", lenght);
	serial->SetPos(76, 3); serial->Print(lenghtString, BLACK, WHITE);
}

void Snake::printStatus()
{
	serial->SetPos(68, 1);
	if (state == RUNNING)
		serial->Print(" RUNNING ", GREEN, BLACK);
	else if (state == PAUSED)
		serial->Print(" PAUSED ", YELLOW, BLACK); serial->Print(' ', BLACK, WHITE);
}