#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "judge.h"
#include "base.h"
#include "new_test.h"
#include "play.h"

#define MAX_LENGTH 1024

GAME game;
FILE* pInputFile;
int initMoney = INITIALMONEY;
char buffer[MAX_LENGTH + 1];
int inputFileNum = 0;

enum GameState
{
    BEFORE,
    AFTER,
    END
} gameState;

void getCommand(OP* p)
{
    commandExplain(p);
}

int main()
{
    freopen("tmp.txt", "w", stdout);
    while (openFile(inputFileNum))
    {
        inputFileNum++;
        preSet(); // 假设文件前两行一定是对的
        gameState = BEFORE;
        while (gameState != END)
        {
            switch (gameState)
            {
            case BEFORE:
                preAction();
                break;
            case AFTER:
                afterAction();
                break;
            case END:
                break;
            }
        }
    }
}

int getLine(char* buffer)
{
    return fgets(buffer, MAX_LENGTH, pInputFile) != NULL;
}

int openFile(int num)
{
    if (pInputFile != NULL)
    {
        fclose(pInputFile);
    }

    char n_j[10];
    char addr[50] = "input/input_";
    itoa(num, &n_j, 10); // int -> char
    strcat(addr, n_j);
    strcat(addr, ".txt");

    pInputFile = fopen(addr, "r");
    return pInputFile != NULL;
}

void preSet()
{
    OP op1, op2;
    op1.command = pRESETMONEY;
    op2.command = pRESETPLAYER;
    getLine(buffer);
    op1.num[0] = atoi(buffer);
    getLine(buffer);
    int i = 0;
    char tmpBuf[2];
    tmpBuf[1] = '\0';
    for (; buffer[i] != '\n'; i++)
    {
        tmpBuf[0] = buffer[i];
        op2.num[i] = atoi(tmpBuf);
    }
    op2.num[i] = -1;
    initGame(&op1, &op2);
}

PLAYER* playerSel(char name)
{
    PLAYER* player_now = game.player;
    char nameBuf[2];
    nameBuf[0] = name;
    nameBuf[1] = '\0';
    while (strcmp(player_now->Name, nameBuf))
    {
        player_now = player_now->next;
    }
    return player_now;
}

void preAction()
{
    if (game.current_player->SleepTime > 0) {
        game.current_player->SleepTime--;
    }
    if (game.current_player->SleepTime > 0) {
        PLAYER* tmp = game.current_player;
        game.current_player = game.current_player->next;
        while (game.current_player->dead == 1)
        {
            game.current_player = game.current_player->next;
        }
        // 只剩一个玩家
        if (game.current_player == tmp)
        {
            gameState = END;
        }
        else {
            gameState = BEFORE;
        }
        return;
    }
    OP* cur_op = (OP*)malloc(sizeof(OP));
    if (cur_op == NULL)
    {
        exit(1);
    }
    commandExplain(cur_op);
    PLAYER* sel_p;
    // 解析出指令，然后进行指令选择响应
    switch (cur_op->command)
    {
    case sETPOS:
    {
        sel_p = playerSel(cur_op->player);
        sel_p->CurPos = cur_op->num[0] % MAPSIZE;
        // TODO 是否要直接指定位置
        break;
    }
    case sETMONEY:
    {
        sel_p = playerSel(cur_op->player);
        sel_p->Money = cur_op->num[0];
        break;
    }
    case sETPOINT:
    {
        sel_p = playerSel(cur_op->player);
        sel_p->Point = cur_op->num[0];
    }
    case sETITEM:
    {
        sel_p = playerSel(cur_op->player);
        if (cur_op->num[0] == 1)
        {
            sel_p->BlockNum = cur_op->num[1];
        }
        else if (cur_op->num[0] == 2)
        {
            sel_p->RobotNum = cur_op->num[1];
        }
        else if (cur_op->num[0] == 3)
        {
            sel_p->BombNum = cur_op->num[1];
        }
        else
        {
        }
        break;
    }
    case sETBUFF:
    {
        sel_p = playerSel(cur_op->player);
        sel_p->BuffTime = cur_op->num[0] + 1;
        break;
    }
    case sETMAP:
    {
        sel_p = playerSel(cur_op->player);
        game.map[cur_op->num[0]].HouseOwnerId = sel_p->PlayerId;
        _upOwnBlock_(sel_p, cur_op->num[0], cur_op->num[1]);
        LOCATION* newLocation = (LOCATION*)malloc(sizeof(LOCATION));
        newLocation->houseID = cur_op->num[0];
        newLocation->next = sel_p->HouseId;
        sel_p->HouseId = newLocation;
        free(newLocation);
        break;
    }
    case sETUNMAP:
    {
        sel_p = playerSel(cur_op->player);
        game.map[cur_op->num[0]].HouseOwnerId = -1;
        game.map[cur_op->num[0]].HouseLevel = -1;
        game.map[cur_op->num[0]].rentAmount = 0;
        LOCATION* p = sel_p->HouseId, * q = sel_p->HouseId;
        while (p != NULL)
        {
            if (p->houseID == cur_op->num[0])
            {
                break;
            }
            q = p;
            p = p->next;
        }
        if (p == sel_p->HouseId)
        {
            sel_p->HouseId = p->next;
        }
        else
        {
            q->next = p->next;
        }
        break;
    }
    case sETBOMB:
    {
        game.map[cur_op->num[0] % MAPSIZE].ItemType = BOMb;
        break;
    }
    case sETBARRIER:
    {
        game.map[cur_op->num[0] % MAPSIZE].ItemType = BLOCk;
        break;
    }
    case rOLL:
    {
        cur_op->num[0] = roll();
    }
    case sTEP:
    {
        int dis = cur_op->num[0];
        PLAYER* p = game.current_player;
        int bombedORblocked = 0;
        game.map[p->CurPos].PlayerId = -1;
        for (int i = 1; i <= dis; ++i)
        {
            if (game.map[(i + p->CurPos) % MAPSIZE].ItemType != NONe)
            {
                bombedORblocked = 1;
                if (game.map[(i + p->CurPos) % MAPSIZE].ItemType == BOMb)

                {
                    game.map[(i + p->CurPos) % MAPSIZE].ItemType = NONe;
                    p->CurPos = HOSPITAL_POS;
                }
                else if (game.map[(i + p->CurPos) % MAPSIZE].ItemType == BLOCk)
                {
                    game.map[(i + p->CurPos) % MAPSIZE].ItemType = NONe;
                    _playerChangePos(p, i);
                }

                break;
            }
        }
        if (!bombedORblocked)
        {
            _playerChangePos(p, dis);
        }
        game.map[p->CurPos].PlayerId = p->PlayerId;
        playerPosToMap(p);
        gameState = AFTER;
        break;
    }
    case sELL:
    {
        game.current_player->Money += ((game.map[cur_op->num[0]]).MapValue) * ((game.map[cur_op->num[0]]).HouseLevel + 1) * 2;
        sellOwnBlock(game.current_player, 0, cur_op);
        break;
    }
    case uSEROBOT:
    {
        useRobot(game.current_player);
        break;
    }
    case uSEBLOCK:
    {
        useBlock(game.current_player, cur_op->num[0]);
        break;
    }
    case uSEBOMB:
    {
        useBomb(game.current_player, cur_op->num[0]);
        break;
    }
    case dUMP:
    {
        Dump(inputFileNum - 1);
        gameState = END;
        break;
    }
    default:
        break;
    }
    free(cur_op);
}

void afterAction()
{
    OP* cur_op = (OP*)malloc(sizeof(OP));
    if (cur_op == NULL)
    {
        exit(1);
    }
    PLAYER* cur_p = game.current_player;
    if (cur_p->BuffTime > 0)
    {
        cur_p->BuffTime--;
    }
    printf("-------num: %d\n", inputFileNum);
    assert(cur_p->dead != 1);
    MAPBLOCK block = game.map[cur_p->CurPos];
    switch (game.map[cur_p->CurPos].HouseType)
    {
    case LAND:
        if (block.HouseOwnerId == -1)
        {
            getCommand(cur_op);
            while (cur_op->command != yESORNO) {
                getCommand(cur_op);
            }
            int curPos = cur_p->CurPos;
            MAPBLOCK* currentBlock = &(game.map)[curPos];
            int currentCost = currentBlock->MapValue;
            if (cur_op->num[0] == 1)
            {
                if (currentCost > cur_p->Money)
                { // 现金不够，不能购买
                    // printf("您的资金不足\n");
                    break;
                }

                // 购买完成，更新状态
                currentBlock->HouseOwnerId = cur_p->PlayerId;
                currentBlock->HouseLevel++;
                currentBlock->rentAmount = currentCost * 2;
                cur_p->Money -= currentCost;
                // 把该空地加入该玩家名下房产
                LOCATION* p = (LOCATION*)malloc(sizeof(LOCATION));
                if (p == NULL)
                {
                    // printf("I am wrong\n");
                    exit(1);
                }
                // 采用头插法
                p->houseID = cur_p->CurPos;
                p->next = cur_p->HouseId;
                cur_p->HouseId = p;
            }
        }
        else if (block.HouseOwnerId != cur_p->PlayerId)
        {
            if (cur_p->BuffTime > 0)
            {
                // printf("财神附体，免交租金！\n");
            }
            else
            {
                PLAYER* owner = cur_p;
                while (owner->PlayerId != block.HouseOwnerId)
                {
                    owner = owner->next;
                }
                if (cur_p->Money < block.rentAmount)
                {
                    // printf("您没有足够现金，您已出局！\n");
                    cur_p->dead = 1;
                }
                else
                {
                    payRent(cur_p, owner, block.rentAmount);
                    // printf("当前地块主人为%s，您交付租金%d元.\n", owner->Name, block.rentAmount);
                }
            }
        }
        else
        {
            // update my house
            // printf("你想升级这块地皮吗？[y/n]\n");
            getCommand(cur_op);
            while (cur_op->command != yESORNO)
            {
                getCommand(cur_op);
            }
            if (cur_op->num[0] == 1)
            {
                upOwnBlock(cur_p);
            }
        }
        break;
    case HOSPITAL:
        break;
    case STPOINT:
        break;
    case MAGICHOUSE:
        enterMagicHouse(cur_p, NULL);
        break;
    case PROPHOUSE:
        getCommand(cur_op);
        while (cur_op->num[0] != -1)
        {
            // TODO 添加异常二级命令处理：如 step 2 会被视为正常输入
            enterItemShop(cur_p, cur_op);
            getCommand(cur_op);
        }
        break;
    case GIFTHOUSE:
        getCommand(cur_op);
        enterGiftShop(cur_p, cur_op);
        break;
    case JAIL:
        // printf("您进入监狱，将被跳过两轮！\n");
        cur_p->SleepTime = 2 + 1;
        break;
    case MINERALFILED:
        cur_p->Point += block.minePoint;
        // printf("获取点数：%d\n", block.minePoint);
        break;
    }

    free(cur_op);
    gameState = BEFORE;
    PLAYER* tmp = game.current_player;
    game.current_player = game.current_player->next;
    while (game.current_player->dead == 1)
    {
        game.current_player = game.current_player->next;
    }
    // 只剩一个玩家
    if (game.current_player == tmp)
    {
        gameState = END;
    }
}

/* test input */
void commandExplain(OP* command_state)
{
    char* buf;

    char* ptr = NULL;
    // int num = (int)strtol(command, &ptr, 0);
    //  if (num != 0)
    //  {
    //      initMoney = num;
    //      command_state->num[0] = initMoney; // initmoney or initplayer
    //      return;
    //  }
    int ctn = 1; // 解析到错误指令，continue
    while (ctn)
    {
        ctn = 0;
        getLine(buffer);
        buffer[strlen(buffer) - 1] = '\0';
        buf = strtok(buffer, " "); // String splitting, taking out the string before the first space
        printf("buffer: %s, buf: %s\n", buffer, buf);
        // Preset mode
        if (!strcmp(buf, "set"))
        {
            buf = strtok(NULL, " "); // continue splitting
            printf("buffer: %s, buf: %s\n", buffer, buf);

            if (!strcmp(buf, "money"))
            {
                buf = strtok(NULL, " ");
                if (!strcmp(buf, "Q"))
                {
                    buf = strtok(NULL, " ");
                    int moneySet = (int)strtol(buf, '\0', 0);
                    // setInitialFund(0,moneySet);
                    command_state->command = sETMONEY;
                    command_state->player = 'Q';
                    command_state->num[0] = moneySet;
                }
                else if (!strcmp(buf, "A"))
                {
                    buf = strtok(NULL, " ");
                    int moneySet = (int)strtol(buf, '\0', 0);
                    // setInitialFund(1,moneySet);
                    command_state->command = sETMONEY;
                    command_state->player = 'A';
                    command_state->num[0] = moneySet;
                }
                else if (!strcmp(buf, "S"))
                {
                    buf = strtok(NULL, " ");
                    int moneySet = (int)strtol(buf, '\0', 0);
                    // setInitialFund(2,moneySet);
                    command_state->command = sETMONEY;
                    command_state->player = 'S';
                    command_state->num[0] = moneySet;
                }
                else if (!strcmp(buf, "J"))
                {
                    buf = strtok(NULL, " ");
                    int moneySet = (int)strtol(buf, '\0', 0);
                    // setInitialFund(3,moneySet);
                    command_state->command = sETMONEY;
                    command_state->player = 'J';
                    command_state->num[0] = moneySet;
                }
            }

            else if (!strcmp(buf, "point"))
            {
                buf = strtok(NULL, " ");
                if (!strcmp(buf, "Q"))
                {
                    buf = strtok(NULL, " ");
                    int pointSet = (int)strtol(buf, '\0', 0);
                    // setInitialPoint(0,pointSet);
                    command_state->command = sETPOINT;
                    command_state->player = 'Q';
                    command_state->num[0] = pointSet;
                }
                else if (!strcmp(buf, "A"))
                {
                    buf = strtok(NULL, " ");
                    int pointSet = (int)strtol(buf, '\0', 0);
                    // setInitialPoint(1,pointSet);
                    command_state->command = sETPOINT;
                    command_state->player = 'A';
                    command_state->num[0] = pointSet;
                }
                else if (!strcmp(buf, "S"))
                {
                    buf = strtok(NULL, " ");
                    int pointSet = (int)strtol(buf, '\0', 0);
                    // setInitialPoint(2,pointSet);
                    command_state->command = sETPOINT;
                    command_state->player = 'S';
                    command_state->num[0] = pointSet;
                }
                else if (!strcmp(buf, "J"))
                {
                    buf = strtok(NULL, " ");
                    int pointSet = (int)strtol(buf, '\0', 0);
                    // setInitialPoint(3,pointSet);
                    command_state->command = sETPOINT;
                    command_state->player = 'J';
                    command_state->num[0] = pointSet;
                }

            }

            else if (!strcmp(buf, "item"))
            {
                buf = strtok(NULL, " ");
                if (!strcmp(buf, "Q"))
                {
                    /*获取道具*/
                    buf = strtok(NULL, " ");
                    int item = (int)strtol(buf, '\0', 0);
                    /*获取数量*/
                    buf = strtok(NULL, " ");
                    int itemNum = (int)strtol(buf, '\0', 0);
                    // setInitialItem(0,item,itemNum);
                    command_state->command = sETITEM;
                    command_state->player = 'Q';
                    command_state->num[0] = item;
                    command_state->num[1] = itemNum; // format: num[5] = {item, itemNum}
                }
                else if (!strcmp(buf, "A"))
                {
                    /*获取道具*/
                    buf = strtok(NULL, " ");
                    int item = (int)strtol(buf, '\0', 0);
                    /*获取数量*/
                    buf = strtok(NULL, " ");
                    int itemNum = (int)strtol(buf, '\0', 0);
                    // setInitialItem(1,item,itemNum);
                    command_state->command = sETITEM;
                    command_state->player = 'A';
                    command_state->num[0] = item;
                    command_state->num[1] = itemNum; // format: num[5] = {item, itemNum}
                }
                else if (!strcmp(buf, "S"))
                {
                    /*获取道具*/
                    buf = strtok(NULL, " ");
                    int item = (int)strtol(buf, '\0', 0);
                    /*获取数量*/
                    buf = strtok(NULL, " ");
                    int itemNum = (int)strtol(buf, '\0', 0);
                    // setInitialItem(2,item,itemNum);
                    command_state->command = sETITEM;
                    command_state->player = 'S';
                    command_state->num[0] = item;
                    command_state->num[1] = itemNum; // format: num[5] = {item, itemNum}
                }
                else if (!strcmp(buf, "J"))
                {
                    /*获取道具*/
                    buf = strtok(NULL, " ");
                    int item = (int)strtol(buf, '\0', 0);
                    /*获取数量*/
                    buf = strtok(NULL, " ");
                    int itemNum = (int)strtol(buf, '\0', 0);
                    // setInitialItem(3,item,itemNum);
                    command_state->command = sETITEM;
                    command_state->player = 'J';
                    command_state->num[0] = item;
                    command_state->num[1] = itemNum; // format: num[5] = {item, itemNum}
                }

            }

            else if (!strcmp(buf, "buff"))
            {
                buf = strtok(NULL, " ");
                if (!strcmp(buf, "Q"))
                {
                    buf = strtok(NULL, " ");
                    int buffTimeSet = (int)strtol(buf, '\0', 0);
                    // setInitialBuff(0,buffTimeSet);
                    command_state->command = sETBUFF;
                    command_state->player = 'Q';
                    command_state->num[0] = buffTimeSet; // format: num[5] = {buffTimeSet}
                }
                else if (!strcmp(buf, "A"))
                {
                    buf = strtok(NULL, " ");
                    int buffTimeSet = (int)strtol(buf, '\0', 0);
                    // setInitialBuff(1,buffTimeSet);
                    command_state->command = sETBUFF;
                    command_state->player = 'A';
                    command_state->num[0] = buffTimeSet; // format: num[5] = {buffTimeSet}
                }
                else if (!strcmp(buf, "S"))
                {
                    buf = strtok(NULL, " ");
                    int buffTimeSet = (int)strtol(buf, '\0', 0);
                    // setInitialBuff(2,buffTimeSet);
                    command_state->command = sETBUFF;
                    command_state->player = 'S';
                    command_state->num[0] = buffTimeSet; // format: num[5] = {buffTimeSet}
                }
                else if (!strcmp(buf, "J"))
                {
                    buf = strtok(NULL, " ");
                    int buffTimeSet = (int)strtol(buf, '\0', 0);
                    // setInitialBuff(3,buffTimeSet);
                    command_state->command = sETBUFF;
                    command_state->player = 'J';
                    command_state->num[0] = buffTimeSet; // format: num[5] = {buffTimeSet}
                }

            }

            // map
            else if (!strcmp(buf, "map"))
            {
                char* whichBlock = strtok(NULL, " ");
                char* whichPlayer = strtok(NULL, " ");
                char* whichLevel = strtok(NULL, " ");

                int blockNum = (int)strtol(whichBlock, '\0', 0);
                int level = (int)strtol(whichLevel, '\0', 0);

                if (!strcmp(whichPlayer, "Q"))
                {
                    // _buyEmptyBlock_(0,blockNum,level);
                    command_state->command = sETMAP;
                    command_state->player = 'Q';
                    command_state->num[0] = blockNum;
                    command_state->num[1] = level; // format: num[5] = {blockNum, level}
                }
                else if (!strcmp(whichPlayer, "A"))
                {
                    // _buyEmptyBlock_(1,blockNum,level);
                    command_state->command = sETMAP;
                    command_state->player = 'A';
                    command_state->num[0] = blockNum;
                    command_state->num[1] = level; // format: num[5] = {blockNum, level}
                }
                else if (!strcmp(whichPlayer, "S"))
                {
                    // _buyEmptyBlock_(2,blockNum,level);
                    command_state->command = sETMAP;
                    command_state->player = 'S';
                    command_state->num[0] = blockNum;
                    command_state->num[1] = level; // format: num[5] = {blockNum, level}
                }
                else if (!strcmp(whichPlayer, "J"))
                {
                    // _buyEmptyBlock_(3,blockNum,level);
                    command_state->command = sETMAP;
                    command_state->player = 'J';
                    command_state->num[0] = blockNum;
                    command_state->num[1] = level; // format: num[5] = {blockNum, level}
                }

            }

            // set unmap [n] // 将可购买的地皮n设置为未购买状态
            else if (!strcmp(buf, "unmap"))
            {
                buf = strtok(NULL, " ");
                int blockNum = (int)strtol(buf, '\0', 0);
                // _sellOwnBlock_(blockNum);
                command_state->command = sETUNMAP;
                command_state->num[0] = blockNum; // format: num[5] = {blockNum}

            }

            else if (!strcmp(buf, "bomb"))
            {
                buf = strtok(NULL, " ");
                int k = atoi(buf); // loc of set bomb
                // if(game.map[k].PlayerId == -1)
                // {
                //     _useBomb_(k);
                // }
                command_state->command = sETBOMB;
                command_state->num[0] = k; // format: num[5] = {k}

            }

            else if (!strcmp(buf, "barrier"))
            {
                buf = strtok(NULL, " ");
                int k = atoi(buf); // loc of set barrier
                // if(game.map[k].PlayerId == -1)
                // {
                //     _useBlock_(k);
                // }
                command_state->command = sETBARRIER;
                command_state->num[0] = k; // format: num[5] = {k}

            }

            // add or decline
            else if (!strcmp(buf, "pos"))
            {
                buf = strtok(NULL, " ");
                printf("buffer: %s, buf: %s\n", buffer, buf);
                // add or decline user`s location
                PLAYER* pPlayer = game.player;
                for (int i = 0; i < game.play_num; i++)
                {
                    if (pPlayer->Name[0] == buf[0])
                    {
                        buf = strtok(NULL, " ");
                        printf("buffer: %s, buf: %s\n", buffer, buf);
                        int n = atoi(buf);
                        // setUserLoc(pPlayer->Name[0], n);

                        command_state->command = sETPOS;
                        command_state->player = pPlayer->Name[0];
                        command_state->num[0] = n;
                        break;
                    }
                    pPlayer = pPlayer->next;
                }

            }

            // stop
            else if (!strcmp(buf, "stop"))
            {
                buf = strtok(NULL, " ");
                PLAYER* pPlayer = game.player;
                for (int i = 0; i < game.play_num; i++)
                {
                    if (pPlayer->Name[0] == buf[0])
                    {
                        buf = strtok(NULL, " ");
                        // _setSleepTime_(pPlayer, atoi(buf));
                        command_state->command = sETSTOP;
                        command_state->player = pPlayer->Name[0];
                        command_state->num[0] = atoi(buf);
                    }
                    pPlayer = pPlayer->next;
                }

            }
        }

        // roll
        else if (!strcmp(buf, "roll"))
        {
            command_state->command = rOLL;

        }

        // sell
        else if (!strcmp(buf, "sell"))
        {
            buf = strtok(NULL, " ");
            int i = atoi(buf); // char -> int
            command_state->command = sELL;
            command_state->player = game.current_player->Name[0];
            command_state->num[0] = i;

        }

        // use bomb
        else if (!strcmp(buf, "bomb"))
        {
            buf = strtok(NULL, " ");
            int i = atoi(buf); // char -> int
            command_state->command = uSEBOMB;
            command_state->player = game.current_player->Name[0];
            command_state->num[0] = i;

        }

        // use robot
        else if (!strcmp(buf, "robot"))
        {
            buf = strtok(NULL, " ");
            int i = atoi(buf); // char -> int
            command_state->command = uSEROBOT;
            command_state->player = game.current_player->Name[0];

            // command_state->num[0] = i;

        }

        // use block
        else if (!strcmp(buf, "block"))
        {
            buf = strtok(NULL, " ");
            int i = atoi(buf); // char -> int
            command_state->command = uSEBLOCK;
            command_state->player = game.current_player->Name[0];
            command_state->num[0] = i;

        }

        // step
        else if (!strcmp(buf, "step"))
        {
            buf = strtok(NULL, " ");

            printf("buffer: %s, buf: %s\n", buffer, buf);
            int i = atoi(buf); // char -> int
            /* unit test */
            // _playerChangePos(game.current_player, i);
            // refresh state
            // game.current_player = game.current_player->next;    // End the current round
            command_state->command = sTEP;
            command_state->player = game.current_player->Name[0];
            command_state->num[0] = i;

        }

        // yes or no
        else if (!strcmp(buf, "y") || !strcmp(buf, "Y"))
        {
            buf = strtok(NULL, " ");
            printf("buffer: %s, buf: %s\n", buffer, buf);
            command_state->command = yESORNO;
            command_state->player = game.current_player->Name[0];
            command_state->num[0] = 1;

        }
        else if (!strcmp(buf, "n") || !strcmp(buf, "N"))
        {
            buf = strtok(NULL, " ");
            printf("buffer: %s, buf: %s\n", buffer, buf);
            command_state->command = yESORNO;
            command_state->player = game.current_player->Name[0];
            command_state->num[0] = 0;

        }

        // dump
        else if (!strcmp(buf, "dump"))
        {
            // Dump();
            command_state->command = dUMP;

        }

        // quit
        else if (!strcmp(buf, "quit"))
        {
            assert(!strcmp(buf, "quit"));
            command_state->command = qUIT;
        }
        else
        {
            ctn = 1;
        }
    }
    return;
}

/* get game state & print into file */
void Dump(int j) {
    PLAYER* pPlayer = game.player;
    FILE* fp = NULL;
    // static int j = 0;       // number of dump_.txt
    char n_j[10];
    char addr[50] = "dump/dump_";
    itoa(j, &n_j, 10);
    strcat(addr, n_j);
    strcat(addr, ".txt");
    fp = fopen(addr, "w");
    if (!fp) {
        printf("open failed");
        return;
    }

    // user AQS
    char name[5] = "";
    for (int i = 0; i < game.play_num; i++) {
        name[i] = pPlayer->Name[0];
        pPlayer = pPlayer->next;
    }
    fprintf(fp, "user %s\n", name);

    //pretuser A
    fprintf(fp, "preuser %c\n", game.current_player->Name[0]);

    // user info
    // pPlayer = game.player;
    // for (int i = 0; i < game.play_num; i++){
    //     fprintf(fp, "%c\n", pPlayer->Name[0]);
    //     fprintf(fp, "alive %d\n", 1-pPlayer->dead);
    //     fprintf(fp, "money %d\n", pPlayer->Money);
    //     fprintf(fp, "point %d\n", pPlayer->Point);
    //     fprintf(fp, "item1 %d\n", pPlayer->BlockNum);
    //     fprintf(fp, "item2 %d\n", pPlayer->RobotNum);
    //     fprintf(fp, "item3 %d\n", pPlayer->BombNum);
    //     fprintf(fp, "buff %d\n", pPlayer->BuffTime);
    //     fprintf(fp, "stop %d\n", pPlayer->SleepTime);
    //     fprintf(fp, "userloc %d\n", pPlayer->CurPos);
    //     pPlayer = pPlayer->next;
    // }
    pPlayer = game.player;
    char players_name[4] = { 'Q', 'A', 'S', 'J' };
    int flag = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < game.play_num; j++)
        {
            if (pPlayer->Name[0] == players_name[i])
            {
                flag = 1;
                break;
            }
            pPlayer = pPlayer->next;
        }
        if (flag == 1)
        {
            fprintf(fp, "%c\n", pPlayer->Name[0]);
            fprintf(fp, "alive %d\n", 1 - pPlayer->dead);
            fprintf(fp, "money %d\n", pPlayer->Money);
            fprintf(fp, "point %d\n", pPlayer->Point);
            fprintf(fp, "item1 %d\n", pPlayer->BlockNum);
            fprintf(fp, "item2 %d\n", pPlayer->RobotNum);
            fprintf(fp, "item3 %d\n", pPlayer->BombNum);
            fprintf(fp, "buff %d\n", pPlayer->BuffTime);
            fprintf(fp, "stop %d\n", pPlayer->SleepTime);
            fprintf(fp, "userloc %d\n", pPlayer->CurPos);
            flag = 0;
        }
        else {
            fprintf(fp, "%c\n", players_name[i]);
            fprintf(fp, "alive %d\n", 0);
            fprintf(fp, "money %d\n", 0);
            fprintf(fp, "point %d\n", 0);
            fprintf(fp, "item1 %d\n", 0);
            fprintf(fp, "item2 %d\n", 0);
            fprintf(fp, "item3 %d\n", 0);
            fprintf(fp, "buff %d\n", 0);
            fprintf(fp, "stop %d\n", 0);
            fprintf(fp, "userloc %d\n", 0);
        }
    }

    // MAP
    char owner_flag = 0;
    fprintf(fp, "MAP\n");

    // mapuser [n] [AQSJ]//地块n上的玩家（由先到达的排在前）
    for (int i = 0; i < 70; i++) {
        if (game.map[i].PlayerId != -1) {
            if (game.map[i].PlayerId == 0)   owner_flag = 'Q';
            else if (game.map[i].PlayerId == 1)  owner_flag = 'A';
            else if (game.map[i].PlayerId == 2)  owner_flag = 'S';
            else if (game.map[i].PlayerId == 3)  owner_flag = 'J';
            fprintf(fp, "mapuser %d %c\n", i, owner_flag, owner_flag);
        }
    }

    // map [n] [A|Q|S|J] [level]//地块n上某玩家拥有等级为level的房屋
    for (int i = 0; i < 70; i++) {
        if (game.map[i].HouseOwnerId != -1) {
            //get owner char
            if (game.map[i].HouseOwnerId == 0)   owner_flag = 'Q';
            else if (game.map[i].HouseOwnerId == 1)  owner_flag = 'A';
            else if (game.map[i].HouseOwnerId == 2)  owner_flag = 'S';
            else if (game.map[i].HouseOwnerId == 3)  owner_flag = 'J';
            fprintf(fp, "map %d %c %d\n", i, owner_flag, game.map[i].HouseLevel);
        }
    }

    // item [n] [1|3]//地块n上存在某个道具
    for (int i = 0; i < 70; i++) {
        if (game.map[i].ItemType == BLOCK)
        {
            fprintf(fp, "item %d 1\n", i);
        }
        else if (game.map[i].ItemType == ROBOT)
        {
            fprintf(fp, "item %d 2\n", i);
        }
        else if (game.map[i].ItemType == BOMB)
        {
            fprintf(fp, "item %d 3\n", i);
        }
    }

    // j++;    // next dump
    // printf("output success");
    fclose(fp);
    return;
}
