#pragma once

#include <iostream>
#include <queue>
#include <mutex>

#include <Windows.h>
#include "SerialTerminalConstants.h"

enum SerialTerminalState
{
	INIT,
	CONNECTED,
	DISCONNECTED,
	RECONNECTED,
	WAIT,
	DEINIT
};

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

	std::queue<char> buffer;

	std::mutex buffer_mutex;

	SerialTerminalState state;

	bool isConnected;

	bool isAlive;

	bool Write(uint8_t val[]);

	void Write(uint8_t val[], DWORD number);

	bool Read(uint8_t* val);

	void Read(char val);

public:
	SerialTerminal();

	void CreateConnection(const char aCommPort[], DWORD aBaudRate, BYTE aParity, BYTE aByteSize, BYTE aStopBits);

	bool Clear();

	bool Clear(uint8_t colour);

	bool SetPos(uint8_t x, uint8_t y);

	bool Cursor(uint8_t colour, bool mode, bool blink, bool visible);

	bool Print(char character, uint8_t background, uint8_t text);

	bool Print(const char* str, uint8_t background, uint8_t text);

	bool GetStatus();

	void CheckConnection();

	void CheckResponse();

	SerialTerminalState GetState();
};

