#include <iostream>
#include "Game.h"

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	Game game;

	// Select output terminal in Game.h
#ifdef SERIAL_TERMINAL	// Serial terminal
	try
	{
		game.serial.CreateConnection("COM3", CBR_115200, NOPARITY, 8, ONESTOPBIT);
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

	game.serial.Clear(WHITE);
	
	game.serial.SetPos(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	game.serial.Cursor(BLUE, false, true, true);
	game.serial.Print("Hello", BLACK, CYAN);
#endif	// Serial terminal

#ifdef WINDOWS_TERMINAL	// windows terminal	WindowsTerminal windows;
		
	game.DrawBorder('#');
	
#endif	// windows terminal

	return 0;
}