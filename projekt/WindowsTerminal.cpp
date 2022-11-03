#include "WindowsTerminal.h"

WindowsTerminal::WindowsTerminal()
{
    // set size of the window
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect);
    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 607, 678, TRUE); //607, 678
}

void WindowsTerminal:: SetPos(int x, int y) {
    COORD pos = { x, y };
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}