#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "base.h"
#include "new_test.h"
#include "play.h"

#define MAX_LENGTH 1024

GAME game;
FILE *pInputFile;
int initMoney = INITIALMONEY;
char buffer[MAX_LENGTH + 1];

enum GameState
{
    INPUT,
    AFTER,
    END
} gameState;

int main()
{
    freopen("tmp.txt", "w", stdout);
    int inputFileNum = 0;
    while (openFile(inputFileNum))
    {
        inputFileNum++;
        preSet();
        gameState = INPUT;
        while (gameState != END)
        {
            switch (gameState)
            {
            case INPUT:
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

int getLine(char *buffer)
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
    for (; buffer[i] != '\n'; i++)
    {
        op2.num[i] = (int)(buffer[i] - '0');
    }
    op2.num[i] = -1;
    initGame(&op1, &op2);
}

PLAYER *playerSel(char *name) {
    PLAYER *player_now = game.player;
    while (strcmp(player_now->Name, name))
    {
        player_now = player_now->next;
    }
    return player_now;
}
int name2ID(char name) {
    int ret = -1;
    if (name == 'A') {
        ret = 2;
    } else if (name == 'Q') {
         ret = 1;
    } else if (name == 'J') {
        ret = 4;
    } else if (name == 'S') {
        ret = 3;
    }
    assert(ret != -1);
    return ret;
}
void preAction() {
    getLine(buffer);
    OP *cur_op = (OP*)malloc(sizeof(OP));
    if(cur_op == NULL) {
        exit(1);
    }
    commandExplain(buffer, cur_op);
    PLAYER *sel_p;
    // 解析出指令，然后进行指令选择响应
    switch(cur_op->command) {
        case sETPOS: {
            sel_p = playerSel(cur_op->player);
            _playerChangePos(sel_p, cur_op->num[0]);
            // TODO 是否要直接指定位置
            break;
        }
        case sETMONEY: {
            sel_p = playerSel(cur_op->player);
            sel_p->Money = cur_op->num[0];
            break;
        }
        case sETITEM: {
            sel_p = playerSel(cur_op->player);
            if(cur_op->num[0] == 1) {
                sel_p->BlockNum = cur_op->num[1];
            }
            else if(cur_op->num[0] == 2) {
                sel_p->RobotNum = cur_op->num[1];
            }
            else if(cur_op->num[0] == 3) {
                sel_p->BombNum = cur_op->num[1];
            }
            else {

            }
            break;
        }
        case sETBUFF: {
            sel_p = playerSel(cur_op->player);
            sel_p->BuffTime = cur_op->num[0];
            break;
        }
        case sETMAP: {
            sel_p = playerSel(cur_op->player);
            game.map[cur_op->num[0]].HouseOwnerId = sel_p->PlayerId;
            _upOwnBlock_(sel_p, cur_op->num[0], cur_op->num[1]);
            break;
        }
        case sETUNMAP: {
            game.map[cur_op->num[0]].HouseOwnerId = -1;
            game.map[cur_op->num[0]].HouseLevel = -1;
            game.map[cur_op->num[0]].rentAmount = 0;
            break;
        }
        case sETBOMB: {
            game.map[cur_op->num[0] % MAPSIZE].ItemType = BOMb;
            break;
        }
        case sETBARRIER:{
            game.map[cur_op->num[0] % MAPSIZE].ItemType = BLOCk;
            break;
        }
        case sTEP: {
            _step(cur_op->num[0], game.current_player);
            afterActionJudge(game.current_player);
            playerPosToMap(game.current_player);
            gameState = AFTER;
            break;
        }
        case sELL: {
            
            break;
        }
        case rOLL: {
            break;
        }
        case uSEROBOT: {
            sel_p = playerSel(cur_op->player);
            game.map[cur_op->num[0]].ItemType = BOMb;
            sel_p->BombNum--;
            break;
        }
        case uSEBLOCK: {
            sel_p = playerSel(cur_op->player);
            game.map[cur_op->num[0]].ItemType = BOMb;
            sel_p->BombNum--;
            break;
        }
        case uSEBOMB: {
            sel_p = playerSel(cur_op->player);
            game.map[cur_op->num[0]].ItemType = BOMb;
            sel_p->BombNum--;
            break;
        }
        case dUMP: {
            break;
        }
        case yESORNO: {
            break;
        }
        case nUM: {
            break;
        }
    }
}




void dump()
{
}

/* test input */
void commandExplain(char* command, OP* command_state)
{
    char* buf = strtok(command, " ");   // String splitting, taking out the string before the first space

    char* ptr = NULL;
    int num = (int)strtol(command,&ptr,0);

    if(num != 0)
    {
        initMoney = num;
        command_state->num[0] = initMoney;//initmoney or initplayer

        // command->num[0] = num;

        /* function */
        // initMap();
        // _choosePlayer_(command);

        return;
    }

    // Preset mode
    else if(!strcmp(buf, "set"))
    {
        buf = strtok(NULL, " ");        // continue splitting

        if(!strcmp(buf, "money"))
        {
            buf = strtok(NULL," ");
            if(!strcmp(buf,"Q")){
                buf = strtok(NULL," ");
                int moneySet = (int)strtol(buf,'\0',0);
                // setInitialFund(0,moneySet);
                command_state->command = sETMONEY;
                command_state->player = 'Q';
                command_state->num[0] = moneySet;

            }
            else if(!strcmp(buf,"A")){
                buf = strtok(NULL," ");
                int moneySet = (int)strtol(buf,'\0',0);
                // setInitialFund(1,moneySet);
                command_state->command = sETMONEY;
                command_state->player = 'A';
                command_state->num[0] = moneySet;
            }
            else if(!strcmp(buf,"S")){
                buf = strtok(NULL," ");
                int moneySet = (int)strtol(buf,'\0',0);
                // setInitialFund(2,moneySet);
                command_state->command = sETMONEY;
                command_state->player = 'S';
                command_state->num[0] = moneySet;
            }
            else if(!strcmp(buf,"J")){
                buf = strtok(NULL," ");
                int moneySet = (int)strtol(buf,'\0',0);
                // setInitialFund(3,moneySet);
                command_state->command = sETMONEY;
                command_state->player = 'J';
                command_state->num[0] = moneySet;
            }
            return;
        }

        else if(!strcmp(buf, "point"))
        {
            buf = strtok(NULL," ");
            if(!strcmp(buf,"Q")){
                buf = strtok(NULL," ");
                int pointSet = (int)strtol(buf,'\0',0);
                // setInitialPoint(0,pointSet);
                command_state->command = sETPOINT;
                command_state->player = 'Q';
                command_state->num[0] = pointSet;
            }
            else if(!strcmp(buf,"A")){
                buf = strtok(NULL," ");
                int pointSet = (int)strtol(buf,'\0',0);
                // setInitialPoint(1,pointSet);
                command_state->command = sETPOINT;
                command_state->player = 'A';
                command_state->num[0] = pointSet;
            }
            else if(!strcmp(buf,"S")){
                buf = strtok(NULL," ");
                int pointSet = (int)strtol(buf,'\0',0);
                // setInitialPoint(2,pointSet);
                command_state->command = sETPOINT;
                command_state->player = 'S';
                command_state->num[0] = pointSet;
            }
            else if(!strcmp(buf,"J")){
                buf = strtok(NULL," ");
                int pointSet = (int)strtol(buf,'\0',0);
                // setInitialPoint(3,pointSet);
                command_state->command = sETPOINT;
                command_state->player = 'J';
                command_state->num[0] = pointSet;
            }
            return;
        }

        else if(!strcmp(buf, "item"))
        {
            buf = strtok(NULL," ");
            if(!strcmp(buf,"Q")){
                /*获取道具*/
                buf = strtok(NULL," ");
                int item = (int)strtol(buf,'\0',0);
                /*获取数量*/
                buf = strtok(NULL," ");
                int itemNum=(int)strtol(buf,'\0',0);
                // setInitialItem(0,item,itemNum);
                command_state->command = sETITEM;
                command_state->player = 'Q';
                command_state->num[0] = item;
                command_state->num[1] = itemNum;        //format: num[5] = {item, itemNum}
            }
            else if(!strcmp(buf,"A")){
                /*获取道具*/
                buf = strtok(NULL," ");
                int item = (int)strtol(buf,'\0',0);
                /*获取数量*/
                buf = strtok(NULL," ");
                int itemNum=(int)strtol(buf,'\0',0);
                // setInitialItem(1,item,itemNum);
                command_state->command = sETITEM;
                command_state->player = 'A';
                command_state->num[0] = item;
                command_state->num[1] = itemNum;        //format: num[5] = {item, itemNum}
            }
            else if(!strcmp(buf,"S")){
                /*获取道具*/
                buf = strtok(NULL," ");
                int item = (int)strtol(buf,'\0',0);
                /*获取数量*/
                buf = strtok(NULL," ");
                int itemNum=(int)strtol(buf,'\0',0);
                // setInitialItem(2,item,itemNum);
                command_state->command = sETITEM;
                command_state->player = 'S';
                command_state->num[0] = item;
                command_state->num[1] = itemNum;        //format: num[5] = {item, itemNum}
            }
            else if(!strcmp(buf,"J")){
                /*获取道具*/
                buf = strtok(NULL," ");
                int item = (int)strtol(buf,'\0',0);
                /*获取数量*/
                buf = strtok(NULL," ");
                int itemNum=(int)strtol(buf,'\0',0);
                // setInitialItem(3,item,itemNum);
                command_state->command = sETITEM;
                command_state->player = 'J';
                command_state->num[0] = item;
                command_state->num[1] = itemNum;        //format: num[5] = {item, itemNum}
            }
            return;
        }

        else if(!strcmp(buf, "buff"))
        {
            buf = strtok(NULL," ");
            if(!strcmp(buf,"Q")){
                buf = strtok(NULL," ");
                int buffTimeSet = (int)strtol(buf,'\0',0);
                // setInitialBuff(0,buffTimeSet);
                command_state->command = sETBUFF;
                command_state->player = 'Q';
                command_state->num[0] = buffTimeSet;   //format: num[5] = {buffTimeSet}
            }
            else if(!strcmp(buf,"A")){
                buf = strtok(NULL," ");
                int buffTimeSet = (int)strtol(buf,'\0',0);
                // setInitialBuff(1,buffTimeSet);
                command_state->command = sETBUFF;
                command_state->player = 'A';
                command_state->num[0] = buffTimeSet;   //format: num[5] = {buffTimeSet}
            }
            else if(!strcmp(buf,"S")){
                buf = strtok(NULL," ");
                int buffTimeSet = (int)strtol(buf,'\0',0);
                // setInitialBuff(2,buffTimeSet);
                command_state->command = sETBUFF;
                command_state->player = 'S';
                command_state->num[0] = buffTimeSet;   //format: num[5] = {buffTimeSet}
            }
            else if(!strcmp(buf,"J")){
                buf = strtok(NULL," ");
                int buffTimeSet = (int)strtol(buf,'\0',0);
                // setInitialBuff(3,buffTimeSet);
                command_state->command = sETBUFF;
                command_state->player = 'J';
                command_state->num[0] = buffTimeSet;   //format: num[5] = {buffTimeSet}
            }
            return;
        }

        //map
        else if(!strcmp(buf, "map"))
        {
            char* whichBlock = strtok(NULL," ");
            char* whichPlayer = strtok(NULL," ");
            char* whichLevel = strtok(NULL," ");

            int blockNum = (int)strtol(whichBlock,'\0',0);
            int level = (int)strtol(whichLevel,'\0',0);

            if(!strcmp(whichPlayer,"Q")){
                // _buyEmptyBlock_(0,blockNum,level);
                command_state->command = sETMAP;
                command_state->player = 'Q';
                command_state->num[0] = blockNum;
                command_state->num[1] = level;      //format: num[5] = {blockNum, level}
            }
            else if(!strcmp(whichPlayer,"A")){
                // _buyEmptyBlock_(1,blockNum,level);
                command_state->command = sETMAP;
                command_state->player = 'A';
                command_state->num[0] = blockNum;
                command_state->num[1] = level;      //format: num[5] = {blockNum, level}
            }
            else if(!strcmp(whichPlayer,"S")){
                // _buyEmptyBlock_(2,blockNum,level);
                command_state->command = sETMAP;
                command_state->player = 'S';
                command_state->num[0] = blockNum;
                command_state->num[1] = level;      //format: num[5] = {blockNum, level}
            }
            else if(!strcmp(whichPlayer,"J")){
                // _buyEmptyBlock_(3,blockNum,level);
                command_state->command = sETMAP;
                command_state->player = 'J';
                command_state->num[0] = blockNum;
                command_state->num[1] = level;      //format: num[5] = {blockNum, level}
            }
            return;
        }

        // set unmap [n] // 将可购买的地皮n设置为未购买状态
        else if(!strcmp(buf,"unmap"))
        {
            buf = strtok(NULL," ");
            int blockNum = (int)strtol(buf,'\0',0);
            // _sellOwnBlock_(blockNum);
            command_state->command = sETUNMAP;
            command_state->num[0] = blockNum;   //format: num[5] = {blockNum}
            return;
        }

        else if(!strcmp(buf, "bomb"))
        {
            buf = strtok(NULL, " ");
            int k = atoi(buf);      //loc of set bomb
            // if(game.map[k].PlayerId == -1)
            // {
            //     _useBomb_(k);
            // }
            command_state->command = sETBOMB;
            command_state->num[0] = k;   //format: num[5] = {k}
            return;
        }

        else if(!strcmp(buf, "barrier"))
        {
            buf = strtok(NULL, " ");
            int k = atoi(buf);      //loc of set barrier
            // if(game.map[k].PlayerId == -1)
            // {
            //     _useBlock_(k);
            // }
            command_state->command = sETBARRIER;
            command_state->num[0] = k;   //format: num[5] = {k}
            return;
        }

        // add or decline
        else if(!strcmp(buf, "pos"))
        {
            buf = strtok(NULL, " ");
            // add or decline user`s location
            PLAYER* pPlayer = game.player;
            for (int i = 0; i < game.play_num; i++)
            {
                if(pPlayer->Name[0] == buf[0])
                {
                    buf = strtok(NULL, " ");
                    {
                        int n = atoi(buf);
                        // setUserLoc(pPlayer->Name[0], n);

                        command_state->command = sETPOS;
                        command_state->player = pPlayer->Name[0];
                        command_state->num[0] = n;
                    }
                }
                pPlayer = pPlayer->next;
            }
            return;
        }
        
        // stop
        else if(!strcmp(buf, "stop"))
        {
            buf = strtok(NULL, " ");
            PLAYER* pPlayer = game.player;
            for (int i = 0; i < game.play_num; i++)
            {
                if(pPlayer->Name[0] == buf[0])
                {
                    buf = strtok(NULL, " ");
                    // _setSleepTime_(pPlayer, atoi(buf));
                    command_state->command = sETSTOP;
                    command_state->player = pPlayer->Name[0];
                    command_state->num[0] = atoi(buf);
                }
                pPlayer = pPlayer->next;
            }
            return;
            
        }
    }

    //roll
    else if (!strcmp(buf, "roll"))
    {
        command_state->command = rOLL;
        return;
    }

    //sell
    else if (!strcmp(buf, "sell"))
    {
        buf = strtok(NULL, " ");
        int i = atoi(buf);          //char -> int
        command_state->command = sELL;
        command_state->player = game.current_player->Name[0];
        command_state->num[0] = i;
        return;
    }
    
    // use bomb
    else if (!strcmp(buf, "bomb"))
    {
        buf = strtok(NULL, " ");
        int i = atoi(buf);          //char -> int
        command_state->command = uSEBOMB;
        command_state->player = game.current_player->Name[0];
        command_state->num[0] = i;
        return;
    }

    // use robot
    else if (!strcmp(buf, "robot"))
    {
        buf = strtok(NULL, " ");
        int i = atoi(buf);          //char -> int
        command_state->command = uSEROBOT;
        command_state->player = game.current_player->Name[0];
        // command_state->num[0] = i;
        return;
    }

    // use block
    else if (!strcmp(buf, "block"))
    {
        buf = strtok(NULL, " ");
        int i = atoi(buf);          //char -> int
        command_state->command = uSEBLOCK;
        command_state->player = game.current_player->Name[0];
        // command_state->num[0] = i;
        return;
    }

    //step
    else if(!strcmp(buf, "step"))
    {
        buf = strtok(NULL, " ");
        int i = atoi(buf);          //char -> int
        /* unit test */
        // _playerChangePos(game.current_player, i);
        // refresh state
        // game.current_player = game.current_player->next;    // End the current round 
        command_state->command = sTEP;
        command_state->player = game.current_player->Name[0];
        command_state->num[0] = i;
        return;
    }

    //yes or no
    else if(!strcmp(buf, "y"))
    {
        buf = strtok(NULL, " ");
        command_state->command = yESORNO;
        command_state->player = game.current_player->Name[0];
        command_state->num[0] = 1;
        return;
    }
    else if(!strcmp(buf, "n"))
    {
        buf = strtok(NULL, " ");
        command_state->command = yESORNO;
        command_state->player = game.current_player->Name[0];
        command_state->num[0] = 0;
        return;
    }

    //dump
    else if(!strcmp(buf, "dump"))
    {
        // Dump();
        command_state->command = dUMEP;
        return;
    }

    //quit
    else if(!strcmp(buf, "quit"))
    {
        command_state->command = qUIT;
        return;
    }
    return ;
}

/* get game state & print into file */
void Dump(int j){
    PLAYER * pPlayer = game.player;
    FILE * fp = NULL;
    // static int j = 0;       // number of dump_.txt
    char n_j;
    char addr[50] = "dump/dump_0.txt";
    itoa(j, &n_j, 10);
    addr[10] = n_j;
    fp = fopen(addr, "w");
    if(!fp){
        printf("open failed");
        return;
    }

    // user AQS
    char name[5] = "";
    for (int i = 0; i < game.play_num; i++){
        name[i] = pPlayer->Name[0];
        pPlayer = pPlayer->next;
    }
    fprintf(fp, "user %s\n", name);

    //pretuser A
    fprintf(fp, "preuser %c\n", game.current_player->Name[0]);

    // user info
    pPlayer = game.player;
    for (int i = 0; i < game.play_num; i++){
        fprintf(fp, "%c\n", pPlayer->Name[0]);
        fprintf(fp, "alive %d\n", 1-pPlayer->dead);
        fprintf(fp, "money %d\n", pPlayer->Money);
        fprintf(fp, "point %d\n", pPlayer->Point);
        fprintf(fp, "item1 %d\n", pPlayer->BlockNum);
        fprintf(fp, "item2 %d\n", pPlayer->RobotNum);
        fprintf(fp, "item3 %d\n", pPlayer->BombNum);
        fprintf(fp, "buff %d\n", pPlayer->BuffTime);
        fprintf(fp, "stop %d\n", pPlayer->SleepTime);
        fprintf(fp, "userloc %d\n", pPlayer->CurPos);
        pPlayer = pPlayer->next;
    }

    // MAP
    char owner_flag = 0;
    fprintf(fp, "MAP\n");

    // mapuser [n] [AQSJ]//地块n上的玩家（由先到达的排在前）
    for(int i = 0; i < 70; i++){
        if(game.map[i].PlayerId != -1){
            if(game.map[i].PlayerId == 0)   owner_flag = 'Q';
            else if(game.map[i].PlayerId == 1)  owner_flag = 'A';
            else if(game.map[i].PlayerId == 2)  owner_flag = 'S';
            else if(game.map[i].PlayerId == 3)  owner_flag = 'J';
            fprintf(fp, "mapuser %d %c\n", i, owner_flag, owner_flag);
        }
    }

    // map [n] [A|Q|S|J] [level]//地块n上某玩家拥有等级为level的房屋
    for (int i = 0; i < 70; i++){
        if (game.map[i].HouseOwnerId != -1){
            //get owner char
            if(game.map[i].HouseOwnerId == 0)   owner_flag = 'Q';
            else if(game.map[i].HouseOwnerId == 1)  owner_flag = 'A';
            else if(game.map[i].HouseOwnerId == 2)  owner_flag = 'S';
            else if(game.map[i].HouseOwnerId == 3)  owner_flag = 'J';
            fprintf(fp, "map %d %c %d\n", i, owner_flag, game.map[i].HouseLevel);
        }
    }

    // item [n] [1|3]//地块n上存在某个道具
    for (int i = 0; i < 70; i++){
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
    fclose(fp);
    return ;
}