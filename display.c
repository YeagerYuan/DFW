#include "display.h"
#include "base.h"
#include <stdio.h>
#define LEFTMARGIN 30 // 地图左margin
#define PRINTMARGIN(x)         \
    do                         \
    {                          \
        printf("%*s", x, " "); \
    } while (0)

extern GAME game;

char Color[8][10] = {
    "\033[30m",
    "\033[31m",
    "\033[32m",
    "\033[33m",
    "\033[34m",
    "\033[35m",
    "\033[36m",
    "\033[37m" };

void printOfColor(char *str, COLOR color)
{
    printf(Color[color]);
    printf(str);
    printf("\033[0m");
}

void printMap(GAME Game)
{
    // 打印左margin
    PRINTMARGIN(LEFTMARGIN);
    // 上面一条边
    for (int i = 0; i <= 28; ++i)
    {
        _printBlock(Game.map[i]);
    }
    printf("\033[1B\033[29D");
    // 左右两条边
    for (int i = 69; i >= 64; --i)
    {
        _printBlock(Game.map[i]);
        PRINTMARGIN(27);
        _printBlock(Game.map[98 - i]);
        printf("\033[1B\033[29D");
    }

    // 底边
    for (int i = 63; i >= 35; --i)
    {
        _printBlock(Game.map[i]);
    }
    printf("\033[1B\033[29D");
}


void _printBlock(MAPBLOCK block)
{
    char levelBuf[2];
    levelBuf[1] = '\0';
    if (block.PlayerId != -1) {
        switch (block.PlayerId)
        {
        case 0:
             printOfColor("Q", RED);
             break;
        case 1:
             printOfColor("A", GREEN);
             break;
        case 2:
             printOfColor("S", BLUE);
             break;
        case 3:
             printOfColor("J", YELLOW);
             break;
        }
    } else {
        switch (block.HouseType)
        {
        case STPOINT:
            printf("S");
            break;
        case LAND:
            switch (block.HouseOwnerId)
            {
            case -1:
                printf("0");
                break;
            case 0:
                levelBuf[0] = '0' + block.HouseLevel;
                printOfColor(levelBuf, GREEN);
                break;
            case 1:
                levelBuf[0] = '0' + block.HouseLevel;
                printOfColor(levelBuf, RED);
                break;
            case 2:
                levelBuf[0] = '0' + block.HouseLevel;
                printOfColor(levelBuf, BLUE);
                break;
            case 3:
                levelBuf[0] = '0' + block.HouseLevel;
                printOfColor(levelBuf, YELLOW);
                break;
            }
            break;
        case PROPHOUSE:
            printOfColor("T", SKYBLUE);
            break;
        case GIFTHOUSE:
            printOfColor("G", PURPLE);
            break;
        case MAGICHOUSE:
            printOfColor("M", PURPLE);
            break;
        case HOSPITAL:
            printf("H");
            break;
        case JAIL:
            printf("P");
            break;
        case MINERALFILED:
            printf("$");
            break;
        }
    }
}

void printLine(int y) {
    _CUR_MOVE(0, y);
    _CUR_CLEAR
    int i=0;
    _SOFT_BLUE_BG
    for(i=0;i<120;i++) {
        printf(" ");
    }
    _BLACK_BG
}

void printHelp() {
    _YELLOW
    printf("Command:\n");
    printf("Roll -- You can move 1~6 steps and after that you cannot use other commands\n");
    printf("Sell n -- You can sell the land you own for twice the price, n is the location of your land\n");
    printf("Block n -- You can place a block anywhere 10 steps before or after your current position,any passing player will be blocked.n is the relative position, and n is forward when it is positive\n");
    printf("Bomb n --You can place a block anywhere 10 steps before or after your current position,any passing player will be bombed and sent to the hospital for 3 turns.n is the relative position, and n is forward when it is positive\n");
    printf("Robot --You can use robot to clear the block or bomb anywhere 10 steps before or after your current position\n");
    printf("Query -- Get your current asset situation\n");
    printf("Help --See how to use other commands\n");
    printf("Quit --Quit the game\n");
    _WHITE
}

void printStatus(PLAYER * cur_p) {
    char names[4][10] = {"Q", "A", "S", "J"};
    _YELLOW
    printf("Player %d--%s     Robot---BLOCK---BOMB---MONEY---POINT\n", cur_p->PlayerId, names[cur_p->PlayerId]);
    _CUR_DOWN
    printf("                  %d      %d     %d      %d      %d\n", cur_p->RobotNum, cur_p->BlockNum, cur_p->BombNum, cur_p->Money, cur_p->Point);
    _WHITE
}
