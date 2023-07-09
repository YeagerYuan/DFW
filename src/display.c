#include "display.h"
#include "base.h"
#include <string.h>
#include <stdio.h>
#define LEFTMARGIN 30 // 地图左margin
#define PRINTMARGIN(x)         \
    do                         \
    {                          \
        printf("%*s", x, " "); \
    } while (0)

extern GAME game;
extern int round_nums;

char Color[8][10] = {
    "\033[30m",
    "\033[31m",
    "\033[32m",
    "\033[33m",
    "\033[34m",
    "\033[35m",
    "\033[36m",
    "\033[37m"};

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
    if (block.PlayerId != -1)
    {
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
    }
    // else if (block.ItemType == BOMb)
    // {
    //     printOfColor("@", RED);
    // }
    else if (block.ItemType == BLOCk)
    {
        printOfColor("#", RED);
    }
    else if (block.ItemType == GOd)
    {
        printOfColor("F", RED);
    }
    else
    {
        switch (block.HouseType)
        {
        case STPOINT:
            printOfColor("S", PURPLE);
            break;
        case LAND:
            switch (block.HouseOwnerId)
            {
            case -1:
                printf("0");
                break;
            case 0:
                levelBuf[0] = '0' + block.HouseLevel;
                printOfColor(levelBuf, RED);
                break;
            case 1:
                levelBuf[0] = '0' + block.HouseLevel;
                printOfColor(levelBuf, GREEN);
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
            printOfColor("T", PURPLE);
            break;
        case GIFTHOUSE:
            printOfColor("G", PURPLE);
            break;
        case MAGICHOUSE:
            printOfColor("M", PURPLE);
            break;
        case HOSPITAL:
            printOfColor("H", PURPLE);
            break;
        case JAIL:
            printOfColor("P", PURPLE);
            break;
        case MINERALFILED:
            printOfColor("$", YELLOW);
            break;
        case PARK:
            printOfColor("P", PURPLE);
            break;
        }
    }
}

void printLine(int y)
{
    _CUR_MOVE(0, y);
    _CUR_CLEAR
    int i = 0;
    _SOFT_BLUE_BG
    for (i = 0; i < 120; i++)
    {
        printf(" ");
    }
    _BLACK_BG
}

void printHelp()
{
    _YELLOW
    printf("命令:\n");
    printf("Roll -- 您可以移动1~6个步,之后不能使用其他命令\n");
    printf("Sell n -- 您可以以两倍的价格出售您拥有的土地,n是您土地的位置\n");
    printf("Block n -- 您可以在当前位置之前或之后10步的任何位置放置一个路障,任何路过的玩家都会被拦下.n是相对位置,n是正值时向前\n");
    // printf("Bomb n --您可以在当前位置之前或之后 10 步的任何地方放置一个炸弹,任何路过的玩家都会被炸伤并送往医院3轮.n是相对位置,n为正时向前\n");
    printf("Robot --您可以使用机器人从当前位置开始清除前方10步内全部的路障或炸弹\n");
    printf("Query -- 了解您当前的资产状况\n");
    printf("Help --了解如何使用其他命令\n");
    printf("Quit --退出游戏\n");
    _WHITE
}

void printStatus(PLAYER *cur_p)
{
    char names[4][10] = {"Q", "A", "S", "J"};
    LOCATION *p = cur_p->HouseId;
    _YELLOW
    switch(cur_p->PlayerId)
    {
    case 0:
        printf("玩家：钱夫人");
        break;
    case 1:
        printf("玩家：阿土伯");
        break;
    case 2:
        printf("玩家：孙小美");
        break;
    case 3:
        printf("玩家：金贝贝");
        break;
    default:
        break;
    }
    printf("  机器人---路障---金钱---点数---回合数\n", cur_p->PlayerId, names[cur_p->PlayerId]);
    _CUR_DOWN
    printf("                %d      %d      %d      %d      %d\n", cur_p->RobotNum, cur_p->BlockNum, cur_p->Money, cur_p->Point, round_nums);
     _CUR_DOWN
    if(p != NULL){
        printf("您的房产位置：\t");
        while(p != NULL){
            printf("%d\t",p->houseID);
            p = p->next;
        }
        printf("\n对应的等级：\t");
        p = cur_p->HouseId;
        while(p != NULL){
            printf("%d\t",(game.map[p->houseID]).HouseLevel);
            p = p->next;
        }
        printf("\n");
    }
    else{
        printf("您目前还没有房产.\n");
    }
    _WHITE
}
