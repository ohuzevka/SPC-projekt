#include <iostream>

#include "emulator.h"
#include "errorList.h"

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	emulator emulator;
	try
	{
		emulator.Connect();
	}
	catch (err num)
	{
		cout << "Error number: " << num << endl;
		return 1;
	}
	catch (...)
	{
		cout << "Unknown error" << endl;
		return 1;
	}

	while (1)
	{
		emulator.Draw();
	}

	return 0;
}