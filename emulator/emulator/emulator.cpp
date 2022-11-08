#include "emulator.h"
#include "errorList.h"
#include "SerialTerminalConstants.h"

using std::cout;
using std::endl;
using std::flush;

void emulator::Connect()
{
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	cout << "BPC - SPC Serial Terminal Emulator\n" << endl;
	cout << "Enter serial port: ";

	std::cin >> commPort;

	cout << "Creating connection to serial port on " << commPort << endl;

	hComm = CreateFileA(commPort,
		GENERIC_READ | GENERIC_WRITE,
		0,      //  must be opened with exclusive-access
		NULL,   //  default security attributes
		OPEN_EXISTING, //  must use OPEN_EXISTING
		0,      //  not overlapped I/O
		NULL); //  hTemplate must be NULL for comm devices

	if (hComm == INVALID_HANDLE_VALUE)
		throw CONN_ERR;

	SecureZeroMemory(&commPar, sizeof(DCB));
	commPar.DCBlength = sizeof(DCB);
	if (!GetCommState(hComm, &commPar))
		throw GET_ERR;

	commPar.BaudRate = CBR_115200;
	commPar.Parity = NOPARITY;
	commPar.ByteSize = 8;
	commPar.StopBits = ONESTOPBIT;

	if (!SetCommState(hComm, &commPar))
		throw SET_ERR;

	comTimeOut.ReadIntervalTimeout = 50;
	comTimeOut.ReadTotalTimeoutMultiplier = 50;
	comTimeOut.ReadTotalTimeoutConstant = 10; 
	comTimeOut.WriteTotalTimeoutMultiplier = 50;
	comTimeOut.WriteTotalTimeoutConstant = 10;
	if (!SetCommTimeouts(hComm, &comTimeOut))
		throw SET_ERR;

	system("CLS");

	lastColor = 0;

	DisplayArea.Right = WIDTH;
	DisplayArea.Bottom = HEIGHT;
	SetConsoleWindowInfo(hOut, TRUE, &DisplayArea);

	cursor.dwSize = 100;
	cursor.bVisible = FALSE;
	SetConsoleCursorInfo(hOut, &cursor);

	SetPos(0, 0);
	cout << "X (0,0)" << flush;
	SetPos(72, 0);
	cout << "(79,0) X" << flush;
	SetPos(0, 39);
	cout << "X (0,39)" << flush;
	SetPos(71, 39);
	cout << "(79,39) X" << flush;
	SetPos(21, 4);
	cout << "BSPC 2022 SERIAL TERMINAL EMULATOR" << flush;
	SetPos(29, 5);
	cout << "Software version 1" << flush;
	SetPos(33, 7);
	cout << "03/11/2022" << flush;
}

void emulator::SetPos(uint8_t x, uint8_t y)
{
	COORD pos = { x, y };

	SetConsoleCursorPosition(hOut, pos);

	posX = x;
	posY = y;
}

void emulator::Print(char val, uint8_t colors)
{
	uint8_t background = colors & 0x07;
	uint8_t text = ( colors >> TEXT_COLOR ) & 0x07;

	lastColor = background;

	cout << "\033[" << text + 30 << ";" << background + 40 << "m" << val << "\033[0m" << std::flush;

	++posX;
	if (posX > WIDTH)
	{
		posX = 0;
		++posY;
		cout << "\n" << flush;
	}
	if (posY > HEIGHT)
	{
		posY = 0;

		SetPos(0, 0);
	}
}

void emulator::Clear()
{
	system("CLS");

	SetPos(0, 0);
	/*
	char temp[WIDTH] = { ' ' };

	for (size_t i = 0; i <= (HEIGHT + 1) * (WIDTH + 1); ++i)
	{
		Print(' ', lastColor);
	}*/
}

void emulator::Cursor(uint8_t par)
{
	cursor.bVisible = TRUE;
	if (par & 0x10)
		cursor.dwSize = 100;
	else
		cursor.dwSize = 20;

	SetConsoleCursorInfo(hOut, &cursor);
}

void emulator::Draw()
{
	DWORD numberOfBytesReaded;
	uint8_t buf[4];

	size_t i = 0;
	do
	{
		ReadFile(hComm, &buf[i], 1, &numberOfBytesReaded, NULL);
		if (numberOfBytesReaded == 1)
			++i;
	} while (i < 4);

	uint8_t val[2] = { 'N', buf[0] };

	if (buf[3] != CRC)
	{
		WriteFile(hComm, val, 2, NULL, NULL);
	}

	switch (buf[0])
	{
		case WRITE_CHAR:
			val[0] = SUCCESS;
			WriteFile(hComm, val, 2, NULL, NULL);

			Print(buf[1], buf[2]);

			break;
		case SET_POS:
			val[0] = SUCCESS;
			WriteFile(hComm, val, 2, NULL, NULL);

			SetPos(buf[1], buf[2]);

			break;
		case CURSOR:
			val[0] = SUCCESS;
			WriteFile(hComm, val, 2, NULL, NULL);

			Cursor(buf[1]);

			break;
		case NOP:
			val[0] = SUCCESS;
			WriteFile(hComm, val, 2, NULL, NULL);

			break;
		case GET_STATUS:
			val[0] = SUCCESS;
			WriteFile(hComm, val, 2, NULL, NULL);

			break;
		case ERASE:
			val[0] = SUCCESS;
			WriteFile(hComm, val, 2, NULL, NULL);

			Clear();

			break;
		default:
			WriteFile(hComm, val, 2, NULL, NULL);

			break;
	}
}

