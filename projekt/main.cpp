#include <iostream>
#include <thread>
#include <chrono>

#include "SerialTerminal.h"
#include "SerialTerminalConstants.h"
#include "Snake.h"

using std::cout;
using std::endl;

void callback1(Snake& aSnake, SerialTerminal& aSerial)
{
	while (aSnake.status() != QUIT)
	{
		auto start_time = std::chrono::high_resolution_clock::now();

		if(aSerial.GetState() == CONNECTED)
		{
			if (aSnake.speedChangedFlag == true)
			{
				aSnake.printSpeed();
				aSnake.speedChangedFlag = false;
			}

			if (aSnake.stateChangedFlag == true)
			{
				aSnake.printStatus();
				aSnake.stateChangedFlag = false;
			}

			if (aSnake.status() == RUNNING)
			{
				aSnake.move();
			}

			auto end_time = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> diff = end_time - start_time;

			double remaining = (1000 / aSnake.getSpeed()) - diff.count();
			if(remaining > 0)
				std::this_thread::sleep_for(std::chrono::milliseconds(uint8_t(remaining)));
		}
	}
}

void callback2(Snake& aSnake, SerialTerminal& aSerial)
{
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD InRec;

	while (aSnake.status() != QUIT)
	{
		DWORD numberRec;
		ReadConsoleInput(hIn, &InRec, 1, &numberRec);

		if (numberRec == 1)
		{
			switch (InRec.Event.KeyEvent.uChar.AsciiChar)
			{
			case 'w':
				aSnake.changeDir(UP);
				if(aSnake.status() == PAUSED)
					aSnake.play();
				break;
			case 's':
				aSnake.changeDir(DOWN);
				if (aSnake.status() == PAUSED)
					aSnake.play();
				break;
			case 'a':
				aSnake.changeDir(LEFT);
				if (aSnake.status() == PAUSED)
					aSnake.play();
				break;
			case 'd':
				aSnake.changeDir(RIGHT);
				if (aSnake.status() == PAUSED)
					aSnake.play();
				break;
			case ' ':
				if (aSnake.status() == RUNNING)
					aSnake.pause();
				else if (aSnake.status() == GAME_OVER)
					aSnake.init();
				break;
			case '+':
				aSnake.increaseSpeed();
				break;
			case '-':
				aSnake.decreaseSpeed();
				break;
			case 'q':
					aSnake.quit();
				break;
			}

			numberRec = 0;
		}
	}
}

void callback3(Snake& aSnake, SerialTerminal& aSerial)
{
	size_t RetriesCounter = 0;
	while(aSnake.status() != QUIT)
	{
		switch(aSerial.GetState())
		{
			case INIT:
				break;

			case RECONNECTED:
				aSerial.Clear(BLACK);
				aSnake.draw();

			case CONNECTED:
				aSerial.CheckResponse();
				break;

			case WAIT:
				++RetriesCounter;

				if(RetriesCounter >= RETRIES && RETRIES > 0)
					{
					cout << "No response from serial terminal!\nPress any key to close this window." << endl;
					aSnake.quit();
					continue;
					}

				std::this_thread::sleep_for(std::chrono::milliseconds(500));

			case DISCONNECTED:
				aSnake.pause();
				aSerial.CheckConnection();
				break;
			
			case DEINIT:
				aSnake.quit();
				break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
int main(int argc, char* argv[])
{
	SerialTerminal serial;
	Snake snake(&serial);

	char port[6];
	std::cout << "Enter serial port: ";
	while (serial.GetState() != CONNECTED)
	{
		std::cin >> port;
		try { serial.CreateConnection(port, CBR_115200, NOPARITY, 8, ONESTOPBIT); }
		catch (...) { std::cout << "Invalid port, try again: "; }
	}
	
	snake.init();

	std::cout << R"(   ____            __       )" << std::endl;
	std::cout << R"(  / __/___  ___ _ / /__ ___ )" << std::endl;
	std::cout << R"( _\ \ / _ \/ _ `//  '_// -_))" << std::endl;
	std::cout << R"(/___//_//_/\_,_//_/\_\ \__/ )" << std::endl;				
	std::cout << "===============" << std::endl;
	std::cout << "Controls:" << std::endl;
	std::cout << "Pause: SPACE" << std::endl;
	std::cout << "Speed: +,-" << std::endl;
	std::cout << "Move:  W,S,A,D" << std::endl;
	std::cout << "Quit:  Q" << std::endl;
	std::cout << "===============" << std::endl;

	std::thread thread1(callback1, std::ref(snake), std::ref(serial));
	std::thread thread2(callback2, std::ref(snake), std::ref(serial));
	std::thread thread3(callback3, std::ref(snake), std::ref(serial));

	thread1.join();
	thread2.join();
	thread3.join();

	return 0;
}

