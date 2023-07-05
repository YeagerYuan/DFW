#ifndef DISPLAY_H
#define DISPLAY_H
#include "base.h"
#include <stdlib.h>


// 光标相关
#define _COLOR_INIT system("cls");
#define _CUR_CLEAR printf("\033[K");fflush(stdout);
#define _CUR_MOVE(x, y) printf("\033[%d;%dH", (y), (x));fflush(stdout);
#define _CUR_INPUT _CUR_MOVE(0, COMMAND_WINDOW+1);
#define _CUR_DOWN printf("\033[%dB", 1);fflush(stdout);
#define _CUR_UP printf("\033[nA");fflush(stdout);
#define _CUR_SPAWN _CUR_MOVE(0, 0);

// 文字颜色相关
#define _GREEN printf("\033[32m");fflush(stdout);
#define _YELLOW printf("\033[33m");fflush(stdout);
#define _RED printf("\033[31m");fflush(stdout);
#define _LIGHT_BLUE printf("\033[36m");fflush(stdout);
#define _BLUE printf("\033[34m");fflush(stdout);
#define _WHITE printf("\033[37m");fflush(stdout);

// 背景颜色相关
#define _WHITE_BG printf("\033[47m");fflush(stdout);
#define _BLACK_BG printf("\033[40m");fflush(stdout);
#define _RED_BG printf("\033[41m");fflush(stdout);
#define _GREEN_BG printf("\033[42m");fflush(stdout);
#define _YELLOW_BG printf("\033[43m");fflush(stdout);
#define _BLUE_BG printf("\033[44m");fflush(stdout);
#define _PURPLE_BG printf("\033[45m");fflush(stdout);
#define _SOFT_BLUE_BG printf("\033[46m");fflush(stdout);

// 输入命令的位置相关
#define COMMAND_WINDOW 11

typedef enum COLOR_
{
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    PURPLE,
    SKYBLUE,
    WHITE
} COLOR;


void printOfColor(char *str, COLOR color);
void _printBlock(MAPBLOCK block);
void printMap(GAME Game);

void printLine(int y);
void printHelp();
#endif
