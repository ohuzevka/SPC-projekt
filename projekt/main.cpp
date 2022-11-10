#include <iostream>
#include <thread>
#include <chrono>

#include "SerialTerminal.h"
#include "SerialTerminalConstants.h"
#include "Snake.h"

using std::cout;
using std::endl;

//bool stopRequest = false;

void callback1(Snake& aSnake, SerialTerminal& aSerial)
{
	while (1)
	{
		if (aSnake.state() == false)
		{
			aSnake.move();
			aSnake.redraw();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void callback2(Snake& aSnake, SerialTerminal& aSerial)
{
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD InRec;

	while (1)
	{
		DWORD numberRec;
		ReadConsoleInput(hIn, &InRec, 1, &numberRec);

		if (numberRec == 1)
		{
			switch (InRec.Event.KeyEvent.uChar.AsciiChar)
			{
			case 'w':
				aSnake.changeDir(UP);
				break;
			case 's':
				aSnake.changeDir(DOWN);
				break;
			case 'a':
				aSnake.changeDir(LEFT);
				break;
			case 'd':
				aSnake.changeDir(RIGHT);
				break;
			case 'p':
				aSnake.pause();
				break;
			case 'l':
				aSnake.play();
				break;
			}

			numberRec = 0;
		}
	}
}

void callback3(Snake& aSnake, SerialTerminal& aSerial)
{
	size_t retriesCounter = 0;

	while (1)
	{
		aSerial.KeepAlive();
		
		if (retriesCounter >= RETRIES)
		{
			cout << "Connection timeout!" << endl;
		}
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

int main(int argc, char* argv[])
{
	SerialTerminal serial;
	Snake snake(&serial);


	serial.CreateConnection("COM2", CBR_115200, NOPARITY, 8, ONESTOPBIT);

	serial.Clear();
	snake.init();
	snake.draw();

	std::thread thread1(callback1, std::ref(snake), std::ref(serial));
	std::thread thread2(callback2, std::ref(snake), std::ref(serial));
	std::thread thread3(callback3, std::ref(snake), std::ref(serial));

	thread1.join();
	thread2.join();
	thread3.join();

	return 0;
}

