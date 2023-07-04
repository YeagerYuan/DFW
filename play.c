#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>

#include "play.h"
#include "base.h"
#include "display.h"
#include "input.h"


extern GAME game;
void initGame()
{
    FILE *fp = NULL;
    fp = fopen("./Map.txt", "r");
    char c;
    int cnt = 0;
    char buffer[BUF_SIZE];
    fgets(buffer, BUF_SIZE, fp);
    char tmp[2];
    tmp[1] = '\0';
    strcpy(buffer, "0111111111111151111111111111211111131111111111111611111111111114777777");
    for (int i = 0; i < MAPSIZE; ++i) {
        tmp[0] = buffer[i];
        HOUSETYPE type = (HOUSETYPE)atoi(tmp);
        game.map[cnt].PlayerId = -1;
        game.map[cnt].HouseOwnerId = -1;
        game.map[cnt].HouseType = type;
        game.map[cnt].HouseLevel = -1;
        if (type == LAND) {
            if (cnt < 28) {
				game.map[cnt].MapValue = 200;
            } else if (cnt < 35) {
				game.map[cnt].MapValue = 500;
            } else {
				game.map[cnt].MapValue = 300;
            }
        }
        cnt++;
    }
    chooseInitialFund();
    choosePlayer(&game);
    // 打印角色列表
    PLAYER *p = game.player;
    for (int i = 0; i < game.play_num; i++)
    {
        printf("%d %s\n", p->PlayerId, p->Name);
        p = p->next;
    }
    fclose(fp);
}

void chooseInitialFund() {

}

void clearFlush(FILE *fp)
{
    int ch;
    while ((ch = fgetc(fp)) != EOF && ch != '\n')
        ;
}

int inputJudeg(char *string, int length)
{
    char c;
    int numberHash[4] = {0, 0, 0, 0};
    for (int i = 0; i < length; i++)
    {
        c = string[i];
        if (c > '4' || c < '1')
        {
            return 0;
        }
        if (numberHash[c - '1'])
        {
            // 重复输入
            return 2;
        }
        else
        {
            numberHash[c - '1'] = 1;
        }
    }
    return 1;
}

void choosePlayer(GAME *game1)
{
    // 输入玩家个数
    int playerNum;
    char *trashCan;
    while (1)
    {
        printf("Input player number(2-4): ");
        // 非数字字符
        if (!scanf("%d", &playerNum))
        {
            printf("Please input number!\n");
        }
        // 人数过多
        else if (playerNum > 4)
        {
            printf("Too many players, please between 2 to 4!\n");
        }
        // 人数过少
        else if (playerNum < 2)
        {
            printf("The number of player is too small, please between 2 to 4!\n");
        }
        else
        {
            break;
        }
        clearFlush(stdin);
    }

    // 显示选择角色的提示信息
    char *playerNameList[4] = {"Q", "A", "S", "J"};
    printf("Choose\n");
    printf("1:Q 2:A 3:S 4:J\n");
    char playerList[1024] = {'\0'};

    // 错误输入检测
    while (1)
    {
        // 输入选择角色的序列
        scanf("%s", playerList);
        int len = strlen(playerList);
        if (len < playerNum)
        {
            printf("Too little input!\n");
        }
        else if (len > playerNum)
        {
            printf("Too many input!\n");
        }
        else
        {
            int result;
            result = inputJudeg(playerList, len);
            if (result == 1)
            {
                break;
            }
            else if (result == 2)
            {
                printf("Do not repeat!\n");
            }
            else
            {
                printf("invalid role!\n");
            }
        }
        clearFlush(stdin);
    }

    // 创建角色链表
    PLAYER *firstPlayer, *p;
    for (int i = 0; i < playerNum; i++)
    {
        PLAYER *player = (PLAYER *)malloc(sizeof(PLAYER));
        int playerId = playerList[i] - '1';
        player->PlayerId = playerId;
        player->Name = playerNameList[playerId];
        player->CurPos = 0;
        player->HouseId = NULL;
        player->BlockNum = 0;
        player->RobotNum = 0;
        player->BombNum = 0;
        player->SleepTime = 0;
        player->Point = 0;
        player->Money = INITIALMONEY;
        player->next = NULL;
        if (!i)
        {
            firstPlayer = player;
            p = player;
        }
        else
        {
            p->next = player;
            p = p->next;
        }
    }
    p->next = firstPlayer;

    game.player = firstPlayer;
    game.current_player = firstPlayer;
    game.play_num = playerNum;
}

int action(int sig, PLAYER * p, int action_pos) {
    if(sig == ROLL) {
        int i = roll();
        _step(i, p);
        printf("do roll ! You get num: %d,  now you are on pos: %d\n", i, p->CurPos);
        return ROLL;
    }
    else if(sig == BLOCK) {
        useBlock(p, action_pos);
        return BLOCK;
    }
    else if(sig == BOMB) {
        useBomb(p, action_pos);
        return BOMB;
    }
    else if(sig == ROBOT) {
        useRobot(p);
        return ROBOT;
    }
    else if(sig == QUERY) {
        printStatus(p);
        return QUERY;
    }
    else if(sig == HELP) {
        printHelp();
        return HELP;
    }
    else if(sig == QUIT) {
        printf("do quit\n");
        return QUIT;
    }
    else if(sig == STEP) {
        printf("do step\n");
        return STEP;
    }
    else if(sig == ERROR) {
        printf("error\n");
        return ERROR;
    }
    else if(sig == BUY){
        buyEmptyBlock(p);
        return BUY;
    }
    else if(sig == UP){
        upOwnBlock(p);
        return UP;
    }
    else if(sig == SELL) {
        sellOwnBlock(p, action_pos);
        return SELL;
    }
    else {
        printf("no actions\n");
        return ERROR;
    }
}

int roll() {
    return rand() % 6 + 1;
}

void playerChangePos(PLAYER * p, int pos) {
    p->LastPos = p->CurPos;
    p->CurPos+=pos;
    p->CurPos = p->CurPos % MAPSIZE;
}

void exitGame() {

}

void _step(int dis, PLAYER * cur_p) {
    int i=0;
    PLAYER * cur = cur_p->next;
        // TODO:多个玩家在一起，原来位置PLayerId不是-1
        while(cur != cur_p) {
            i++;
            if(cur->CurPos == cur_p->CurPos) {
                break;
            }
            cur = cur->next;
        }
        if(i == 0 || cur == cur_p) {
            game.map[cur_p->CurPos].PlayerId = -1;
        }
        else {
            game.map[cur_p->CurPos].PlayerId = cur->PlayerId;
        }
        _playerChangePos(cur_p, dis);

    game.map[cur_p->CurPos].PlayerId = cur_p->PlayerId;

}
void _playerChangePos(PLAYER * p, int pos) {
    p->CurPos+=pos;
    p->CurPos = p->CurPos % MAPSIZE;
}

/*
*传入指向当前玩家的指针，函数结束后指针仍指向该玩家
*玩家买入房产后更新玩家的信息和地皮的信息
*/
void  buyEmptyBlock(PLAYER *cur_p){
    //PLAYER *currentPlayer = game.current_player;
    int curPos = cur_p->CurPos;
    MAPBLOCK *currentBlock = &(game.map)[curPos];
    int currentCost = currentBlock->MapValue;
    char ch;

    if (currentBlock->HouseType != LAND){//该地皮不是空地，不能购买
        printf("The block is not land,you can not buy it!\n");
        return;
    }

    if (currentBlock->HouseOwnerId != -1){//该空地已经被别人拥有，不能购买
        printf("The land is owned by you or others,you can not buy it!\n");
        return;
    }

    if (currentCost > cur_p->Money){//现金不够，不能购买
        printf("You don\'t have enough money,you can not buy it!\n");
        return;
    }

    //购买完成，更新状态
    currentBlock->HouseOwnerId = cur_p->PlayerId;
    currentBlock->HouseLevel++;
    //currentBlock.house_flag;/*******************************************************************待定*/
    cur_p->Money -= currentCost;
    //把该空地加入该玩家名下房产
    LOCATION *p = (LOCATION*)malloc(sizeof(LOCATION));
    if (p == NULL){
        printf("I am wrong\n");
        exit(1);
    }
    //采用头插法
    p->houseID = cur_p->CurPos;
    p->next = cur_p->HouseId;
    cur_p->HouseId = p;
}

void upOwnBlock(PLAYER *cur_p){
    int curPos = cur_p->CurPos;
    MAPBLOCK *currentBlock = &(game.map)[curPos];
    int currentCost = currentBlock->MapValue;
    char ch;

    if (currentBlock->HouseType != LAND){//该地皮不是空地，不能升级
        printf("The block is not land,you can not up it!\n");
        return;
    }

    if (currentBlock->HouseOwnerId != cur_p->PlayerId){//不是自己的地皮，不能购买
        printf("The land is not owned by you,you can not up it!\n");
        return;
    }

    if (currentCost > cur_p->Money){//现金不够，不能升级
        printf("You don\'t have enough money,you can not up it!\n");
        return;
    }
    /*按照一般大富翁的逻辑购买完成后不能立即升级,但是*************************************************待定*/
    /*
    if (cur_p->CurPos == cur_p->HouseId->houseID){
        printf("You have just bought it,you can not up it!\n");
        return;
    }
    */
    //currentBlock.house_flag;/*******************************************************************待定*/
    if(currentBlock->HouseLevel < 3) {
        currentBlock->HouseLevel++;
        cur_p->Money -= currentCost;
    }
    else {
        printf("Your land is already 3 level!\n");
        return;
    }
}

/*
*cur_p为指向当前玩家的指针
*num是玩家要出售的房产在地图上的绝对位置
*/
void sellOwnBlock(PLAYER *cur_p,int num){
    if (num<=0 || num >MAPSIZE){                //判断输入位置的合法性
        printf("You input a wrong number,please reinput\n");
        return;
    }
    MAPBLOCK *blockToSell = &game.map[num];
    if (blockToSell->HouseType != LAND){//该地皮不是空地，不能出售
        printf("The block is not land,you can not sell it!\n");
        return;
    }
    if (blockToSell->HouseOwnerId != cur_p->PlayerId){//不是自己的地皮，不能出售
        printf("The land is not owned by you,you can not sell it!\n");
        return;
    }

    //出售完成，更新状态
    cur_p->Money += blockToSell->MapValue * (blockToSell->HouseLevel) * 2;
    blockToSell->HouseOwnerId = -1;     //房屋ID恢复为初始ID
    blockToSell->HouseLevel = 0;       //房屋等级恢复为初始等级
    //currentBlock.house_flag;/*******************************************************************待定*/
    //把该空地从玩家名下房产中删除
    LOCATION *p = cur_p->HouseId,*q = NULL;
    while(p != NULL){
        if(p->houseID == num){
            break;
        }
        q = p;
        p = p->next;
    }
    q = p->next;
    free(p);
}


int useBomb(PLAYER * p, int dis) {
    int ret = FAIL;
    if (p->BombNum <= 0) {
        printf("您没有炸弹！\n");
    } else if (dis < 10 || dis > 10) {
        printf("炸弹使用距离为10米以内！\n");
    } else {
        game.map[p->CurPos + dis].ItemType = BOMB;
        p->BombNum--;
        printf("使用炸弹成功！\n");
        ret = SUCCESS;
    }
    return ret;
}

int useBlock(PLAYER * p, int dis) {
    int ret = FAIL;
    if (p->BlockNum <= 0) {
        printf("您没有路障！\n");
    } else if (dis < 10 || dis > 10) {
        printf("路障使用距离为10米以内！\n");
    } else {
        game.map[p->CurPos + dis].ItemType = BLOCK;
        p->BlockNum--;
        printf("使用路障成功！\n");
        ret = SUCCESS;
    }
    return ret;
}

int useRobot(PLAYER * p) {
    int ret = FAIL;
    if (p->RobotNum <= 0) {
        printf("您没有机器娃娃！\n");
    } else {
        for (int i = 1; i <= 10; ++i) {
            game.map[p->CurPos + i].ItemType = NONE;
        }
        printf("使用机器娃娃成功！\n");
        ret = SUCCESS;
    }
    return ret;
}


void timer(long sleep_time, int sig) {
    clock_t start;
    start = clock();
    if(sig == NOCLOCK) {
        return;
    }
    else {
     while(1) {
        if((clock() - start) > CLOCKS_PER_SEC * sleep_time) {
            break;
        }
        else {}
    }
    return;
    }
}



