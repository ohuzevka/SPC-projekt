#pragma once

#include <Windows.h>
#include "SerialTerminalConstants.h"

enum SerialTerminalErr
{
	CREATE_FILE_ERR,
	GET_COMM_STATE_ERR,
	SET_COMM_STATE_ERR,
	SET_COMM_TIMEOUTS_ERR,
	WRITE_ERR,
	READ_ERR,
	WRONG_COMMAND,
	NO_RESPONSE
};

class SerialTerminal
{
	// Handle to serial communication
	HANDLE hComm;

	// Serial communication parameters
	DCB commPar;

	COMMTIMEOUTS comTimeOut;

	size_t actX, actY;


public:
	SerialTerminal();

	void CreateConnection(const char aCommPort[], DWORD aBaudRate, BYTE aParity, BYTE aByteSize, BYTE aStopBits);

	void Write(uint8_t val[]);

	void Write(uint8_t val[], DWORD number);

	void Read(uint8_t* val);

	void Read(char val);

	void Clear();

	void Clear(uint8_t colour);

	void SetPos(uint8_t x, uint8_t y);

	void Cursor(uint8_t colour, bool mode, bool blink, bool visible);

	void Print(char character, uint8_t background, uint8_t text);

	void Print(const char* str, uint8_t background, uint8_t text);

	void KeepAlive();
};

