#include "Snake.h"
#include<cstdlib>
#include "SerialTerminal.h"

Snake::Snake(SerialTerminal* aSerial)
{
	serial = aSerial;
	apause = true;
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

	generateFood();

	type = 0;
}

void Snake::addElement()
{
}

void Snake::move()
{
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
	srand((unsigned)time(NULL));

	food.iX = ((uint8_t)rand() % DISPLAY_WIDTH);
	food.iY = ((uint8_t)rand() % DISPLAY_HEIGHT);

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
		for (int i = 0; i < border.rightPos; i++)	// top
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
		for (int i = 0; i < border.rightPos; i++)	// bottom
			serial->Print(character, BLACK, WHITE);
	}
}

void Snake::changeDir(Direction dir)
{
	direction = dir;
}

void Snake::pause()
{
	apause = true;

}

void Snake::play()
{
	apause = false;
}

bool Snake::state()
{
	return apause;
}