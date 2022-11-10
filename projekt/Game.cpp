#include "Game.h"
#include "iostream"

using std::cout;
using std::endl;

Game::Game(SerialTerminal* aSerial)
{
	serial = aSerial;
}

void Game::DrawBorder(const char character)
{
	if (border.leftPos >= border.rightPos || border.topPos >= border.bottomPos)
		serial->Print("Invalid border dimensions", BLACK, WHITE);
	else
	{
		serial->SetPos(border.leftPos, border.topPos);
		for (int i = 0; i < border.rightPos; i++)	// top
			serial->Print(character, BLACK, WHITE);
		for (int i = border.topPos + 1; i < border.bottomPos; i++)		// left
		{
			serial->SetPos(border.leftPos, i);
			serial->Print(character, BLACK, WHITE);
		}
		for (int i = border.topPos + 1; i < border.bottomPos; i++)		// right
		{
			serial->SetPos(border.rightPos, i);
			serial->Print(character, BLACK, WHITE);
		}
		serial->SetPos(border.leftPos, border.bottomPos);
		for (int i = 0; i < border.rightPos; i++)	// bottom
			serial->Print(character, BLACK, WHITE);
	}
}

