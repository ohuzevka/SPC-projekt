#pragma once

#include <iostream>
#include <Windows.h>

#define WIDTH 79
#define HEIGHT 39

class emulator
{
	uint8_t posX;
	uint8_t posY;

	uint8_t lastColor;

	HANDLE hOut;
	HANDLE hComm;

	CONSOLE_CURSOR_INFO cursor;

	SMALL_RECT DisplayArea = { 0, 0, 0, 0 };

	DCB commPar;

	COMMTIMEOUTS comTimeOut;

	char commPort[6];


public:
	void Connect();

	void SetPos(uint8_t x, uint8_t y);
	void Print(char val, uint8_t colors);
	void Clear();
	void Cursor(uint8_t par);

	void Draw();
};

