#pragma once
// 指令类型
typedef enum COMMAND_
{
    // preset
    pRESETMONEY,  // 初始资金
    pRESETPLAYER, // 选择初始玩家 (124)
                  // 玩家、地图
    sETSTOP,
    sETPOS,
    sETMONEY, // 设置某个玩家的金钱 (set money A 10000)
    sETPOINT,
    sETITEM,
    sETBUFF,
    sETMAP, // 设置地图归属 (set map 5 A 2)
    sETUNMAP,
    // 道具
    sETBOMB,
    sETBARRIER,
    // 正常指令
    sTEP,
    sELL,
    rOLL,
    uSEROBOT,
    uSEBLOCK,
    uSEBOMB,
    // dump
    dUMP,
    // other
    yESORNO, // yes or no
    nUM,     // 一个单独的num
    qUIT,
} COMMAND;

// 指令的所有内容(类型、参数)
typedef struct COMMANDS_
{
    COMMAND command;
    char player; // 'Q' 'J' 'S' 'A'
    int num[5];  // 1. yse or no : 1 yes 0 no
}OP;


void preAction();
void Dump(int j);
void commandExplain(OP* command_state);
void preSet();
void afterAction();
int openFile(int num);
