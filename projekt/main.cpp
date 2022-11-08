#include <iostream>
#include <thread>
#include <chrono>

#include "SerialTerminal.h"
#include "SerialTerminalConstants.h"
#include "Game.h"

using std::cout;
using std::endl;

bool stopRequest = false;

void callback1(Game& aGame, SerialTerminal& aSerial)
{
	while (!stopRequest)
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
}

void callback2(Game& aGame, SerialTerminal& aSerial)
{
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD InRec;

	while (!stopRequest)
	{
		DWORD numberRec;
		ReadConsoleInput(hIn, &InRec, 1, &numberRec);

		if (numberRec == 1)
		{
			//zavolatMetodu(InRec.Event.KeyEvent.uChar.AsciiChar);

			numberRec = 0;
		}
	}
}

void callback3(Game& aGame, SerialTerminal& aSerial)
{
	size_t retriesCounter = 0;

	while (!stopRequest)
	{
		try
		{
			aSerial.KeepAlive();
		}
		catch (SerialTerminalErr err)
		{
			++retriesCounter;

			switch (err)
			{
			case READ_ERR:
				cout << "Serial port disconnected!" << endl;

				stopRequest = true;

				break;

			case WRITE_ERR:
				cout << "Serial port disconnected!" << endl;

				stopRequest = true;

				break;

			case NO_RESPONSE:
				cout << "No response from serial terminal!" << endl;

				break;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		}
		catch (...)
		{
			;
		}

		if (retriesCounter >= RETRIES)
		{
			stopRequest = true;

			cout << "Connection timeout!" << endl;
		}
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

int main(int argc, char* argv[])
{
	Game game;
	SerialTerminal serial;

	std::thread thread1(callback1, std::ref(game), std::ref(serial));
	std::thread thread2(callback2, std::ref(game), std::ref(serial));
	std::thread thread3(callback3, std::ref(game), std::ref(serial));

	thread1.join();
	thread2.join();
	thread3.join();

	return 0;
}

