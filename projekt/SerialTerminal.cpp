#include <iostream>
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
}

void SerialTerminal::Write(uint8_t val[])
{
	DWORD numberOfBytesWritten;

	WriteFile(hComm, val, 4, &numberOfBytesWritten, NULL);

	if (numberOfBytesWritten != 4)
		throw WRITE_ERR;
}

void SerialTerminal::Write(uint8_t val[], DWORD number)
{
	DWORD numberOfBytesWritten;

	WriteFile(hComm, val, number, &numberOfBytesWritten, NULL);

	if (numberOfBytesWritten != number)
		throw WRITE_ERR;
}

void SerialTerminal::Read(uint8_t* val)
{
	DWORD numberOfBytesReaded;

	ReadFile(hComm, val, 1, &numberOfBytesReaded, NULL);

	cout << *val << endl;

	if (numberOfBytesReaded != 1);
		//throw READ_ERR;
}

void SerialTerminal::Clear()
{
	uint8_t buf[4] = { ERASE, EMPTY, EMPTY, CRC};

	Write(buf);

	uint8_t resp[2];
	Read(&resp[0]);
	Read(&resp[1]);

	if (resp[0] == SUCCESS && resp[1] == ERASE)
		return;

	// TODO: handle error when terminal returns error

}

void SerialTerminal::Clear(uint8_t colour)
{
	Print('A', colour, colour);
	uint8_t buf[4] = { ERASE, EMPTY, EMPTY, CRC };

	Write(buf);

	uint8_t resp[2];
	Read(&resp[0]);
	Read(&resp[1]);

	if (resp[0] == SUCCESS && resp[1] == ERASE)
		return;

	// TODO: handle error when terminal returns error

}

void SerialTerminal::SetPos(uint8_t x, uint8_t y)
{
	uint8_t buf[4] = { SET_POS, x, y, CRC };

	Write(buf);

	uint8_t resp[2];
	Read(&resp[0]);
	Read(&resp[1]);

	if (resp[0] == SUCCESS && resp[1] == SET_POS)
		return;

	// TODO: handle error when terminal returns error

}

void SerialTerminal::Cursor(uint8_t colour, bool mode, bool blink, bool visible)
{
	uint8_t tempColor = colour;
	uint8_t tempR = colour & 0b001;
	uint8_t tempB = colour & 0b100;
	colour = tempR << 2 | tempB >> 2;
	tempColor &= 0b11111010;
	colour = colour | tempColor;
	

	uint8_t buf[4] = { CURSOR, colour | mode << CURSOR_MODE | blink << CURSOR_BLINK | visible << CURSOR_SHOW, EMPTY, CRC };

	Write(buf);

	uint8_t resp[2];
	Read(&resp[0]);
	Read(&resp[1]);

	if (resp[0] == SUCCESS && resp[1] == SET_POS)
		return;
}

void SerialTerminal::Print(char character, uint8_t background, uint8_t text)
{
	uint8_t buf[4] = { WRITE_CHAR, character, background | text << TEXT_COLOR, CRC };

	Write(buf);

	uint8_t resp[2];
	Read(&resp[0]);
	Read(&resp[1]);

	if (resp[0] == SUCCESS && resp[1] == WRITE_CHAR)
		return;

	// TODO: handle error when terminal returns error

}