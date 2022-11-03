#include <iostream>

#include "SerialTerminal.h"


using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	SerialTerminal serial;

	try
	{
		serial.CreateConnection("COM3", CBR_115200, NOPARITY, 8, ONESTOPBIT);
	}
	catch (SerialTerminalErr err)
	{
		if (err == CREATE_FILE_ERR)
			cout << "Create connection to serial port failed" << endl;
		if (err == GET_COMM_STATE_ERR)
			cout << "Get serial port state failed" << endl;
		if (err == SET_COMM_STATE_ERR)
			cout << "Set serial port parameters failed" << endl;

		return 1;
	}
	catch (...)
	{
		cout << "Unknown error" << endl;

		return 1;
	}

	serial.Clear(WHITE);
	
	serial.SetPos(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	serial.Cursor(BLUE, false, true, true);
	serial.Print("Hello", BLACK, CYAN);
	
	return 0;
}