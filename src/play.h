#pragma once

#include "base.h"
#include "new_test.h"

// 动作状态和单元格道具状态（有混用，后续讨论是否改进）
#define ROLL 1
#define SELL 2
#define BLOCK 3
#define BOMB 4
#define ROBOT 5
#define QUERY 6
#define HELP 7
#define QUIT 8
#define STEP 9
#define BUY 10
#define UP 11
#define POINT 12
#define MONEY 13
#define POS_SET 14
#define ERROR -1

// 时钟状态
#define NOCLOCK 0
#define ONCLOCK 1

// 玩家特殊状态
#define ON_HOSPITAL 11
#define ON_PRISON 12
#define ON_MINE 13
#define ON_RENT 14

// 被炸休息轮数
#define BOMBED_SLEEP_ROUND 3

// 特殊地点位置
#define HOSPITAL_POS 14

// 初始化资金定义（待修改）
#define INITIALMONEY 10000

// 道具点数价格
#define BOMB_POINTS 50
#define BLOCK_POINTS 50
#define ROBOT_POINTS 30

// 礼品屋选择
#define GIFTMONEY 2000
#define GIFTPOINT 200
#define GIFTCSROUND 5

void initGame(OP *op1, OP *op2);
void initMap();
void chooseInitialFund();
void setInitialPoint(int playerNumber, int pointNum);
void setInitialFund(int playerNumner, int moneyNum);
void setInitialItem(int playerNumber, int item, int itemNum);
void setInitialBuff(int playerNumber, int buffTime);
void choosePlayer(OP *op);
void exitGame();
int inputJudeg(char *string, int length);

int action(int sig, PLAYER *p, int n);

int roll();
void _step(int dis, PLAYER *p);
void _syncPlayerPos(PLAYER *cur_p);
void _playerChangePos(PLAYER *p, int pos);
void playerPosToMap(PLAYER *cur_p);
void printStatus(PLAYER *cur_p);
void buyEmptyBlock(PLAYER *cur_p, OP *cur_t);
void upOwnBlock(PLAYER *cur_p);
void sellOwnBlock(PLAYER *cur_p, int num,OP *cur_t);

// time related
void timer(long sleep_time, int sig);

// item usage
int _canUseBomb(int pos);
int useBomb(PLAYER *p, int dis);
int _canUseBlock(int pos);
int useBlock(PLAYER *p, int dis);
int useRobot(PLAYER *p);

void payRent(PLAYER *from, PLAYER *to, int amount);

void enterItemShop(PLAYER *cur_p, OP *op);
int buyItem(PLAYER *cur_p, int item_num);
void enterMagicHouse(PLAYER *p, OP *op);
void enterGiftShop(PLAYER *cur_p, OP *p);

int blocked(PLAYER *cur_p, int blocked_pos);
void bombed(PLAYER *cur_p, int bombed_pos);

// 测试函数
void _buyEmptyBlock_(int playerNumber, int num, int level); // 该函数用来实现测试中的set map功能
void _upOwnBlock_(PLAYER *cur_p, int num, int level);
void _sellOwnBlock_(int num); // 该函数用来实现测试中的set unmap功能
void _setSleepTime_(PLAYER *pPlayer, int n);
int _useBomb_(int loc);
int _useBlock_(int loc);
void setUserLoc(char Name, int loc);
void _choosePlayer_(char *playerList);
