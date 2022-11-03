#include "Game.h"
#include "iostream"

using std::cout;
using std::endl;

void Game::DrawBorder(const char character)
{
#ifdef WINDOWS_TERMINAL
	windows.SetPos(0, 0);
	for (int x = 0; x < playingField.X + 2; x++)	// top
		cout << character;
	for (int y = 0; y < playingField.Y; y++)		// left
		cout << "\n" << character;
	for (int y = 0; y < playingField.Y; y++)		// right
	{
		windows.SetPos(playingField.X + 1, y + 1);
		cout << character;
	}
	windows.SetPos(0, playingField.Y + 1);
	for (int x = 0; x < playingField.X + 2; x++)	// bottom
		cout << character;
#endif // WINDOWS_TERMINAL

#ifdef SERIAL_TERMINAL
	serial.SetPos(0, 0);
	for (int x = 0; x < playingField.X + 2; x++)	// top
		serial.Print(character, BLACK, WHITE);		// !! starta funkce, potreba opravit
	for (int y = 0; y < playingField.Y; y++)		// left
	{
		serial.Print('\n', BLACK, WHITE);
		serial.Print(character, BLACK, WHITE);
	}
	for (int y = 0; y < playingField.Y; y++)		// right
	{
		serial.SetPos(playingField.X + 1, y + 1);
		serial.Print(character, BLACK, WHITE);
	}
	serial.SetPos(0, playingField.Y + 1);
	for (int x = 0; x < playingField.X + 2; x++)	// bottom
		serial.Print(character, BLACK, WHITE);

#endif // SERIAL_TERMINAL
}