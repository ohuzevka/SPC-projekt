#include "Snake.h"
#include<cstdlib>
#include "SerialTerminal.h"

Snake::Snake(SerialTerminal* aSerial)
{
	serial = aSerial;
	state = PAUSED;
	direction = DOWN;

	border.topPos = 4;
	border.bottomPos = DISPLAY_HEIGHT;
	border.leftPos = 0;
	border.rightPos = DISPLAY_WIDTH;
}

void Snake::init()
{
	snakeElement[0].iX = 40;
	snakeElement[0].iY = 20;

	snakeElement[1].iX = 40;
	snakeElement[1].iY = 19;

	snakeElement[2].iX = 40;
	snakeElement[2].iY = 18;

	snakeElement[3].iX = 40;
	snakeElement[3].iY = 17;

	lenght = 3;

	serial->Clear(BLACK);

	serial->SetPos(1, 1);
	serial->Print("Move:  W,S,A,D", BLACK, WHITE);
	serial->SetPos(1, 2);
	serial->Print("Pause: SPACE", BLACK, WHITE);

	drawBorder('#');
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
		snakeElement[lenght].iX = snakeElement[lenght - 1].iX;
		snakeElement[lenght].iY = snakeElement[lenght - 1].iY;

		generateFood();
	}

	// Shift elements
	for (int8_t i = lenght + 1; i > 0; --i)
	{
		snakeElement[i] = snakeElement[i - 1];
	}
	
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
	for (uint8_t i = 0; i < lenght; ++i)
	{
		if (i == 0)
		{	// Draw head of snake
			serial->SetPos(snakeElement[i].iX, snakeElement[i].iY);
			serial->Print(headChar, BLACK, WHITE);
		}
		else
		{	// Draw body of snake
			serial->SetPos(snakeElement[i].iX, snakeElement[i].iY);
			serial->Print(bodyChar, BLACK, WHITE);
		}
	}
}

void Snake::redraw()
{
	// Draw new head of snake
	serial->SetPos(snakeElement[0].iX, snakeElement[0].iY);
	serial->Print(headChar, BLACK, WHITE);

	// Make the old head a body
	if (lenght > 1)
	{
		serial->SetPos(snakeElement[1].iX, snakeElement[1].iY);
		serial->Print(bodyChar, BLACK, WHITE);
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
	serial->Print('#', BLACK, RED);
}

void Snake::drawBorder(const char character)
{
	if (border.leftPos >= border.rightPos || border.topPos >= border.bottomPos)
		serial->Print("Invalid border dimensions", BLACK, WHITE);
	else
	{
		serial->SetPos(border.leftPos, border.topPos);
		for (int i = 0; i <= border.rightPos; i++)	// top
			serial->Print(character, BLACK, WHITE);
		for (int i = border.topPos + 1; i < border.bottomPos; i++)		// left
		{
			serial->SetPos(border.leftPos, i);
			serial->Print(character, BLACK, WHITE);
		}
		for (int i = border.topPos + 1; i < border.bottomPos; i++)		// right
		{
			serial->SetPos(border.rightPos, i);
			serial->Print(character, BLACK, WHITE);
		}
		serial->SetPos(border.leftPos, border.bottomPos);
		for (int i = 0; i <= border.rightPos; i++)	// bottom
			serial->Print(character, BLACK, WHITE);
	}
}

void Snake::changeDir(Direction dir)
{
	if ((dir == LEFT && direction == RIGHT) || (dir == RIGHT && direction == LEFT) ||
		(dir == UP && direction == DOWN) || (dir == DOWN && direction == UP))
		return;
	direction = dir;
}

void Snake::pause()
{
	state = PAUSED;
}

void Snake::play()
{
	state = RUNNING;
}

Status Snake::status()
{
	return state;
}

void Snake::gameOver()
{
	state = GAME_OVER;
	serial->SetPos(35, 1);
	serial->Print("Game Over", RED, BLACK);
	serial->SetPos(29, 2);
	serial->Print("Press SPACE to restart", RED, BLACK);
}