#include "Game.h"
#include "iostream"

using std::cout;
using std::endl;

Game::Game()
{

}

void Game::DrawBorder(const char character)
{
#ifdef WINDOWS_TERMINAL
	if (border.leftPos >= border.rightPos || border.topPos >= border.bottomPos)
		cout << "Invalid border dimensions" << endl;
	else
	{
		windows.SetPos(border.leftPos, border.topPos);
		for (int i = 0; i < border.rightPos; i++)	// top
			cout << character;
		for (int i = border.topPos + 1; i < border.bottomPos; i++)		// left
		{
			windows.SetPos(border.leftPos, i);
			cout << character;
		}
		for (int i = border.topPos + 1; i < border.bottomPos; i++)		// right
		{
			windows.SetPos(border.rightPos, i);
			cout << character;
		}
		windows.SetPos(border.leftPos, border.bottomPos);
		for (int i = 0; i < border.rightPos; i++)	// bottom
			cout << character;
	}
#endif // WINDOWS_TERMINAL

#ifdef SERIAL_TERMINAL
	if (border.leftPos >= border.rightPos || border.topPos >= border.bottomPos)
		serial.Print("Invalid border dimensions", BLACK, WHITE);
	else
	{
		serial.SetPos(border.leftPos, border.topPos);
		for (int i = 0; i < border.rightPos; i++)	// top
			serial.Print(character, BLACK, WHITE);
		for (int i = border.topPos + 1; i < border.bottomPos; i++)		// left
		{
			serial.SetPos(border.leftPos, i);
			serial.Print(character, BLACK, WHITE);
		}
		for (int i = border.topPos + 1; i < border.bottomPos; i++)		// right
		{
			serial.SetPos(border.rightPos, i);
			serial.Print(character, BLACK, WHITE);
		}
		serial.SetPos(border.leftPos, border.bottomPos);
		for (int i = 0; i < border.rightPos; i++)	// bottom
			serial.Print(character, BLACK, WHITE);
	}
#endif // SERIAL_TERMINAL
}

void Snake::Spawn(uint8_t SpawnPosX, uint8_t SpawnPosY)
{
	lenght = 0;
	dir = STILL;
	chain[0].X = SpawnPosX;
	chain[0].Y = SpawnPosY;
}

