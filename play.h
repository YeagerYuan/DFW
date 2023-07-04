#pragma once

#include "base.h"

#define ROLL 1
#define SELL 2
#define BLOCK 3
#define BOMB 4
#define ROBOT 5
#define QUERY 6
#define HELP 7
#define QUIT 8
#define STEP 9
#define ERROR -1

#define INITIALMONEY 10000

void initGame();
void chooseInitialFund();
void choosePlayer();
void exitGame();
int inputJudeg(char *string, int length);

int action(int sig, PLAYER * p);

int roll();
void _step(int dis, PLAYER * p);
void _playerChangePos(PLAYER* p, int pos);
void printStatus(PLAYER * cur_p);
