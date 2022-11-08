#pragma once

#define RETRIES 5

#define DISPLAY_WIDTH 79
#define DISPLAY_HEIGHT 39

#define WRITE_CHAR 'c'
#define SET_POS	'p'
#define CURSOR 's'
#define NOP 'n'
#define GET_STATUS 'g'
#define ERASE 'e'

#define CRC 'l'
#define EMPTY 'a'

#define CURSOR_COLOR_R 0
#define CURSOR_COLOR_G 1
#define CURSOR_COLOR_B 2
#define CURSOR_MODE 4
#define CURSOR_BLINK 5
#define CURSOR_SHOW 6

#define TEXT_COLOR 4
#define TEXT_BLINK 7

#define SUCCESS 'A'
#define CMD_ERROR 'N'

#define BLACK 0b000
#define WHITE 0b111
#define RED 0b001
#define GREEN 0b010
#define BLUE 0b100
#define YELLOW 0b011
#define MAGENTA 0b101
#define CYAN 0b110