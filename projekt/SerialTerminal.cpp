#include <iostream>
#include <queue>
#include <mutex>
#include "SerialTerminal.h"

using std::cout;
using std::endl;

SerialTerminal::SerialTerminal()
{
	;
}

void SerialTerminal::CreateConnection(const char commPort[], DWORD baudRate, BYTE parity, BYTE byteSize, BYTE stopBits)
{
	cout << "Creating connection to serial port on " << commPort << endl;

	hComm = CreateFileA(commPort,
				        GENERIC_READ | GENERIC_WRITE,
					    0,      //  must be opened with exclusive-access
					    NULL,   //  default security attributes
					    OPEN_EXISTING, //  must use OPEN_EXISTING
					    0,      //  not overlapped I/O
					    NULL); //  hTemplate must be NULL for comm devices

	if (hComm == INVALID_HANDLE_VALUE)
		throw CREATE_FILE_ERR;

	SecureZeroMemory(&commPar, sizeof(DCB));
	commPar.DCBlength = sizeof(DCB);
	if (!GetCommState(hComm, &commPar))
		throw GET_COMM_STATE_ERR;

	commPar.BaudRate = baudRate;
	commPar.Parity = parity;
	commPar.ByteSize = byteSize;
	commPar.StopBits = stopBits;

	if (!SetCommState(hComm, &commPar))
		throw SET_COMM_STATE_ERR;

	// Specify time-out between charactor for receiving.
	comTimeOut.ReadIntervalTimeout = 50;
	// Specify value that is multiplied 
	// by the requested number of bytes to be read. 
	comTimeOut.ReadTotalTimeoutMultiplier = 50;
	// Specify value is added to the product of the 
	// ReadTotalTimeoutMultiplier member
	comTimeOut.ReadTotalTimeoutConstant = 10;
	// Specify value that is multiplied 
	// by the requested number of bytes to be sent. 
	comTimeOut.WriteTotalTimeoutMultiplier = 50;
	// Specify value is added to the product of the 
	// WriteTotalTimeoutMultiplier member
	comTimeOut.WriteTotalTimeoutConstant = 10;
	// set the time-out parameter into device control.
	if (!SetCommTimeouts(hComm, &comTimeOut))
		throw SET_COMM_TIMEOUTS_ERR;

	cout << "Successfully connected to serial port on " << commPort << endl;

	state = CONNECTED;
}

bool SerialTerminal::Write(uint8_t val[])
{
	DWORD numberOfBytesWritten;

	if(state != CONNECTED && state != RECONNECTED && val[0] != NOP)
		return false;

	if (!WriteFile(hComm, val, 4, &numberOfBytesWritten, NULL))
	{
		state = DEINIT;

		return false;
	}

	if (numberOfBytesWritten != 4)
	{
		state = DEINIT;

		return false;
	}

	buffer_mutex.lock();
	buffer.push(val[0]);
	buffer_mutex.unlock();
	
	return true;
}

void SerialTerminal::Write(uint8_t val[], DWORD number)
{
	DWORD numberOfBytesWritten;

	if (WriteFile(hComm, val, number, &numberOfBytesWritten, NULL))
		throw DEINIT;

	if (numberOfBytesWritten != number)
		throw DEINIT;
}

bool SerialTerminal::Read(uint8_t* val)
{
	DWORD numberOfBytesReaded;

	if(!ReadFile(hComm, val, 1, &numberOfBytesReaded, NULL))
		return false;

	if(numberOfBytesReaded != 1)
		return false;

	return true;
}

void SerialTerminal::Read(char val)
{
	DWORD numberOfBytesReaded;

	char buf[2];

	if (!ReadFile(hComm, buf, 2, &numberOfBytesReaded, NULL))
		throw READ_ERR;

	if (numberOfBytesReaded != 2)
		throw READ_ERR;

	if (buf[0] != SUCCESS || buf[1] != val)
		throw WRONG_COMMAND;

	if (buf[0] != SUCCESS && buf[0] != CMD_ERROR)
		throw NO_RESPONSE;
}

bool SerialTerminal::Clear()
{
	uint8_t buf[4] = { ERASE, EMPTY, EMPTY, CRC};

	return Write(buf);
}

bool SerialTerminal::Clear(uint8_t colour)
{
	Print('A', colour, colour);
	uint8_t buf[4] = { ERASE, EMPTY, EMPTY, CRC };

	return Write(buf);
}

bool SerialTerminal::SetPos(uint8_t x, uint8_t y)
{
	actX = x;
	actY = y;

	uint8_t buf[4] = { SET_POS, x, y, CRC };

	return Write(buf);
}

bool SerialTerminal::Cursor(uint8_t colour, bool mode, bool blink, bool visible)
{
	uint8_t tempColor = colour;
	uint8_t tempR = colour & 0b001;
	uint8_t tempB = colour & 0b100;
	colour = tempR << 2 | tempB >> 2;
	tempColor &= 0b11111010;
	colour = colour | tempColor;
	

	uint8_t buf[4] = { CURSOR, colour | mode << CURSOR_MODE | blink << CURSOR_BLINK | visible << CURSOR_SHOW, EMPTY, CRC };

	return Write(buf);
}

bool SerialTerminal::Print(char character, uint8_t background, uint8_t text)
{
	++actX;
	if (actX > DISPLAY_WIDTH)
	{
		actX = 0;
		++actY;
	}

	uint8_t buf[4] = { WRITE_CHAR, character, background | text << TEXT_COLOR, CRC };

	return Write(buf);
}

bool SerialTerminal::Print(const char* str, uint8_t background, uint8_t text)
{
	for (int i = 0; i < strlen(str); i++)
	{
		uint8_t buf[4] = { WRITE_CHAR, str[i], background | text << TEXT_COLOR, CRC};
		
		if (!Write(buf))
			return false;

		if (str[i] == '\n')
		{
			actX = 0;
			++actY;
		}
		else
		{
			++actX;
			if (actX > DISPLAY_WIDTH)
			{
				actX = 0;
				++actY;
			}
		}
	}
}

bool SerialTerminal::GetStatus()
{
	return isConnected;
}

void SerialTerminal::CheckConnection()
{
	// Init receive buffer
	buffer_mutex.lock();
	while(!buffer.empty())
		buffer.pop();
	buffer_mutex.unlock();

	uint8_t buf[4] = { NOP, EMPTY, EMPTY, CRC };

	if(Write(buf))
	{
		state = RECONNECTED;
		try
		{
			buffer_mutex.lock();
			char val = buffer.front();
			buffer.pop();
			buffer_mutex.unlock();

			Read(val);
		}
		catch(SerialTerminalErr err)
		{
			if(err == READ_ERR)
				state = WAIT;

			if(err == NO_RESPONSE)
				state = WAIT;

			cout << "No respose" << endl;
		}
	}
}

void SerialTerminal::CheckResponse()
{
	if(!buffer.empty())
	{
		buffer_mutex.lock();
		char val = buffer.front();
		buffer.pop();
		buffer_mutex.unlock();

		cout << val << std::flush;

		auto start_time = std::chrono::high_resolution_clock::now();

		state = CONNECTED;

		try
		{
			Read(val);
			auto end_time = std::chrono::high_resolution_clock::now();

			std::chrono::duration<double> diff = end_time - start_time;
			cout << " - OK " << diff.count() << endl;
		}
		catch(SerialTerminalErr err)
		{
			cout << " - ERROR" << endl;

			if(err == READ_ERR)
				state = DISCONNECTED;

			if(err == NO_RESPONSE)
				state = DISCONNECTED;
		}
	}
}

SerialTerminalState SerialTerminal::GetState()
{
	return state;
}