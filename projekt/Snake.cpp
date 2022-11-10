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
	snake[0].iX = 40;
	snake[0].iY = 20;
	snake[0].dir = DOWNWARDS;

	snake[1].iX = 40;
	snake[1].iY = 19;
	snake[1].dir = DOWNWARDS;

	snake[2].iX = 40;
	snake[2].iY = 18;
	snake[2].dir = DOWNWARDS;

	snake[3].iX = 40;
	snake[3].iY = 17;
	snake[3].dir = DOWNWARDS;

	end = 3;

	srand((unsigned)time(NULL));

	XiX = ((uint8_t)rand() % DISPLAY_WIDTH);
	YiY = ((uint8_t)rand() % DISPLAY_HEIGHT);

	serial->SetPos(XiX, YiY);
	serial->Print('#', BLACK, RED);

	type = 0;
}

void Snake::addElement()
{
}

void Snake::move()
{
	uint8_t tempEnd = end;
	if ((snake[0].iX == XiX) && (snake[0].iY == YiY))
	{
		++end;
		snake[end].iX = snake[end - 1].iX;
		snake[end].iY = snake[end - 1].iY;
		snake[end].dir = snake[end - 1].dir;

		srand((unsigned)time(NULL));

		XiX = ((uint8_t)rand() % DISPLAY_WIDTH);
		YiY = ((uint8_t)rand() % DISPLAY_HEIGHT);
		type = ((uint8_t)rand() % 3);

		serial->SetPos(XiX, YiY);
		serial->Print('#', BLACK, RED);
	}


	for (int8_t i = tempEnd; i >= 0; --i)
	{
		switch (snake[i].dir)
		{
			case DOWNWARDS:
				++snake[i].iY;

				if (snake[0].iY > 38)
					snake[0].dir = LEFTWARDS;
				break;
			case UPWARDS:
				--snake[i].iY;

				if (snake[0].iY < 1)
					snake[0].dir = RIGHTWARDS;
				break;
			case LEFTWARDS:
				--snake[i].iX;

				if (snake[0].iX < 1)
					snake[0].dir = UPWARDS;
				break;
			case RIGHTWARDS:
				++snake[i].iX;

				if (snake[0].iX > 78)
					snake[0].dir = DOWNWARDS;
				break;

		}
		if(i > 0)
			snake[i].dir = snake[i - 1].dir;
	}
}

void Snake::draw()
{
	for (size_t i = 0; i < end; ++i)
	{
		if (i == 0)
		{	// Draw head of snake
			serial->SetPos(snake[i].iX, snake[i].iY);
			serial->Print(headChar, BLACK, WHITE);
		}
		else
		{	// Draw body of snake
			serial->SetPos(snake[i].iX, snake[i].iY);
			serial->Print(bodyChar, BLACK, WHITE);
		}
	}
}

void Snake::redraw()
{
	// Draw new head of snake
	serial->SetPos(snake[0].iX, snake[0].iY);
	serial->Print(headChar, BLACK, WHITE);

	// Make the old head a body
	if (end > 1)
	{
		serial->SetPos(snake[1].iX, snake[1].iY);
		serial->Print(bodyChar, BLACK, WHITE);
	}

	// Erase the last node of snake
	serial->SetPos(snake[end].iX, snake[end].iY);
	serial->Print(' ', BLACK, WHITE);
}

void Snake::changeDir(direction dir)
{
	snake[0].dir = dir;
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