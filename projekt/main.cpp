#include <iostream>
#include <thread>
#include <chrono>

#include "SerialTerminal.h"
#include "SerialTerminalConstants.h"
#include "Snake.h"

using std::cout;
using std::endl;

//bool stopRequest = false;

void callback1(Snake& aGame, SerialTerminal& aSerial)
{
	while (1)
	{
		aGame.draw(aSerial);
		aGame.move(aSerial);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void callback2(Snake& aGame, SerialTerminal& aSerial)
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
				aGame.changeDir(UPWARDS);
				break;
			case 's':
				aGame.changeDir(DOWNWARDS);
				break;
			case 'a':
				aGame.changeDir(LEFTWARDS);
				break;
			case 'd':
				aGame.changeDir(RIGHTWARDS);
				break;
			case 'p':
				aGame.pause();
				break;
			case 'l':
				aGame.play();
				break;
			}

			numberRec = 0;
		}
	}
}

void callback3(Snake& aGame, SerialTerminal& aSerial)
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
	Snake game;
	SerialTerminal serial;

	serial.CreateConnection("COM9", CBR_115200, NOPARITY, 8, ONESTOPBIT);

	serial.Clear();
	game.init(serial);

	std::thread thread1(callback1, std::ref(game), std::ref(serial));
	std::thread thread2(callback2, std::ref(game), std::ref(serial));
	std::thread thread3(callback3, std::ref(game), std::ref(serial));

	thread1.join();
	thread2.join();
	thread3.join();

	return 0;
}

