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
extern int initMoney;

void initGame()
{
    initMap();
    chooseInitialFund();
    choosePlayer(&game);
    // 打印角色列表
    PLAYER *p = game.player;
    for (int i = 0; i < game.play_num; i++)
    {
        printf("%d %s\n", p->PlayerId, p->Name);
        setInitialFund(p->PlayerId, initMoney);
        p = p->next;
    }
}

void initMap()
{
    int cnt = 0;
    int minePoints[6] = {60, 80, 40, 100, 80, 20};
    char buffer[BUF_SIZE];
    char tmp[2];
    tmp[1] = '\0';
    strcpy(buffer, "0111111111111151111111111111211111131111111111111611111111111114777777");
    for (int i = 0; i < MAPSIZE; ++i)
    {
        tmp[0] = buffer[i];
        HOUSETYPE type = (HOUSETYPE)atoi(tmp);
        game.map[cnt].PlayerId = -1;
        game.map[cnt].HouseOwnerId = -1;
        game.map[cnt].HouseType = type;
        game.map[cnt].HouseLevel = -1;
        game.map[cnt].ItemType = NONe;
        if (type == LAND)
        {
            if (cnt < 28)
            {
                game.map[cnt].MapValue = 200;
            }
            else if (cnt < 35)
            {
                game.map[cnt].MapValue = 500;
            }
            else
            {
                game.map[cnt].MapValue = 300;
            }
        }
        else if (type == MINERALFILED)
        {
            game.map[cnt].minePoint = minePoints[cnt - 64];
        }
        cnt++;
    }
}
/*设置某一玩家的资金*/
void setInitialFund(int playerNumber, int moneyNum)
{
    PLAYER *player = game.current_player;
    while (player->PlayerId != playerNumber)
    {
        player = player->next;
    }
    player->Money = moneyNum;
}

/*设置某一玩家的点数*/
void setInitialPoint(int playerNumber, int pointNum)
{
    PLAYER *player = game.current_player;
    while (player->PlayerId != playerNumber)
    {
        player = player->next;
    }
    player->Point = pointNum;
}

/*设置某一玩家物品*/
void setInitialItem(int playerNumber, int item, int itemNum)
{
    PLAYER *player = game.current_player;
    while (player->PlayerId != playerNumber)
    {
        player = player->next;
    }
    if (item == 1)
    {
        /*设置路障*/
        player->BlockNum = itemNum;
    }
    else if (item == 2)
    {
        /*设置机器娃娃*/
        player->RobotNum = itemNum;
    }
    else if (item == 3)
    {
        /*设置炸弹*/
        player->BombNum = itemNum;
    }
}

/*设置是否有财神，buffTime表示buff的回合数*/
void setInitialBuff(int playerNumber, int buffTime)
{
    PLAYER *player = game.current_player;
    while (player->PlayerId != playerNumber)
    {
        player = player->next;
    }
    player->BuffTime = buffTime;
}
void chooseInitialFund()
{
    char number[BUF_SIZE] = {'\0'};
    char *ptr;
    int moneyInput;
    PLAYER *p = game.current_player;
    PLAYER *q = game.current_player;

    printf("Please input initial money:");
    while (1)
    {
        _getString(number);
        if (number[0] == '\0')
        {
            initMoney = INITIALMONEY;
            break;
        }
        moneyInput = (int)strtol(number, &ptr, 0);
        if ((moneyInput < 1000 || moneyInput > 50000) || *ptr != '\0')
        {
            printf("You input a bad number,please reinput :");
        }
        else
        {
            initMoney = moneyInput;
            break;
        }
    }
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
    while (1)
    {
        printf("选择玩家数量：(2-4): ");
        // 非数字字符
        if (!scanf("%d", &playerNum))
        {
            printf("请输入数字!\n");
        }
        // 人数过多
        else if (playerNum > 4)
        {
            printf("输入玩家数量过多，请输入 2 到 4 的数字!\n");
        }
        // 人数过少
        else if (playerNum < 2)
        {
            printf("输入玩家数量过少，请输入 2 到 4 的数字!\n");
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
            printf("输入位数太少!\n");
        }
        else if (len > playerNum)
        {
            printf("输入位数过多!\n");
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
                printf("不能重复选择同一个角色!\n");
            }
            else
            {
                printf("没有该角色编号!\n");
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
        player->BlockNum = 1;
        player->RobotNum = 1;
        player->BombNum = 1;
        player->SleepTime = 0;
        player->Point = 0;
        player->MovingDis = 0;
        player->dead = 0;
        player->Money = INITIALMONEY;
        player->BuffTime = 0;
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
    clearFlush(stdin);
}

int action(int sig, PLAYER *p, int action_pos)
{
    if (sig == ROLL)
    {
        int i = roll();
        _step(i, p);
        printf("掷骰子！你得到了数字:");
        _RED
            printf("%d", i);
        _WHITE
        printf("你当前的位置是: %d\n", p->CurPos);
        return ROLL;
    }
    else if (sig == BLOCK)
    {
        useBlock(p, action_pos);
        return BLOCK;
    }
    else if (sig == BOMB)
    {
        useBomb(p, action_pos);
        return BOMB;
    }
    else if (sig == ROBOT)
    {
        useRobot(p);
        return ROBOT;
    }
    else if (sig == QUERY)
    {
        printStatus(p);
        return QUERY;
    }
    else if (sig == HELP)
    {
        printHelp();
        return HELP;
    }
    else if (sig == QUIT)
    {
        printf("do quit\n");
        return QUIT;
    }
    else if (sig == STEP)
    {
        _step(action_pos, p);
        return STEP;
    }
    else if (sig == ERROR)
    {
        printf("error\n");
        return ERROR;
    }
    else if (sig == BUY)
    {
        buyEmptyBlock(p);
        return BUY;
    }
    else if (sig == UP)
    {
        upOwnBlock(p);
        return UP;
    }
    else if (sig == SELL)
    {
        sellOwnBlock(p, action_pos);
        _CUR_SPAWN
        printMap(game);
        return SELL;
    }
    else
    {
        printf("no actions\n");
        return ERROR;
    }
}

int roll()
{
    srand((unsigned int)time(0));
    return rand() % 6 + 1;
}

void _playerChangePos(PLAYER *p, int pos)
{
    p->MovingDis = pos;
    p->CurPos += pos;
    p->CurPos = p->CurPos % MAPSIZE;
}

void exitGame()
{
}

void _step(int dis, PLAYER *cur_p)
{
    if (dis < 0)
    {
        printf("移动距离不能小于0！当前移动距离：%d\n", dis);
        exit(-1);
    }
    _playerChangePos(cur_p, dis);
}

void _syncPlayerPos(PLAYER *cur_p)
{
    int i = 0;
    PLAYER *cur = cur_p->next;
    // TODO:多个玩家在一起，原来位置PLayerId不是-1
    while (cur != cur_p)
    {
        i++;
        if (cur->CurPos == cur_p->CurPos)
        {
            break;
        }
        cur = cur->next;
    }
    if (i == 0 || cur == cur_p)
    {
        game.map[cur_p->CurPos].PlayerId = -1;
    }
    else
    {
        game.map[cur_p->CurPos].PlayerId = cur->PlayerId;
    }
}

void playerPosToMap(PLAYER * cur_p) {
        int i = 0, last_pos = 0;
        PLAYER *cur = cur_p;
        // cur_p的lastPOs才是需要擦除的部分，其余的玩家都是CurPos需要擦除
        if(cur_p->CurPos - cur_p->MovingDis >= 0) {
            last_pos = cur_p->CurPos - cur_p->MovingDis;
        }
        else {
            last_pos = cur_p->CurPos - cur_p->MovingDis + MAPSIZE;
        }
        game.map[last_pos].PlayerId = -1;
        cur = cur->next;
        while(cur != cur_p) {
            game.map[cur->CurPos].PlayerId = -1;
            cur = cur->next;
        }

        // cur_p重新赋值的部分是CurPos，和其他玩家一致
        cur = cur_p->next;
        for(i=0;i<game.play_num;i++) {
            game.map[cur->CurPos].PlayerId = cur->PlayerId;
            cur = cur->next;
        }
}

/*该函数，仅用来辅助买房产和升级房产两个功能，不提供对外接口*/
char getYesOrNo()
{
    char inputString[BUF_SIZE] = {'\0'};
    while (1)
    {
        _getString(inputString);
        if ((inputString[0] == 'Y' || inputString[0] == 'y') && inputString[1] == '\0')
        {
            return 'y';
        }
        else if ((inputString[0] == 'N' || inputString[0] == 'n') && inputString[1] == '\0')
        {
            return 'n';
        }
        else
        {
            printf("您的输入有误请重新输入！\n");
        }
    }
}

/*
 *传入指向当前玩家的指针，函数结束后指针仍指向该玩家
 *玩家买入房产后更新玩家的信息和地皮的信息
 */
void buyEmptyBlock(PLAYER *cur_p)
{
    int curPos = cur_p->CurPos;
    MAPBLOCK *currentBlock = &(game.map)[curPos];
    int currentCost = currentBlock->MapValue;
    char ch;

    if (currentBlock->HouseType != LAND)
    { // 该地皮不是空地，不能购买
        // printf("The block is not land,you can not buy it!\n");
        return;
    }

    if (currentBlock->HouseOwnerId != -1)
    { // 该空地已经被别人拥有，不能购买
        // printf("The land is owned by you or others,you can not buy it!\n");
        return;
    }
    printf("你想买这块地皮吗？[y/n]\n");
    ch = getYesOrNo();

    if (ch == 'y')
    {

        if (currentCost > cur_p->Money)
        { // 现金不够，不能购买
            printf("您的资金不足\n");
            return;
        }

        // 购买完成，更新状态
        currentBlock->HouseOwnerId = cur_p->PlayerId;
        currentBlock->HouseLevel++;
        currentBlock->rentAmount = currentCost * 2;
        cur_p->Money -= currentCost;
        // 把该空地加入该玩家名下房产
        LOCATION *p = (LOCATION *)malloc(sizeof(LOCATION));
        if (p == NULL)
        {
            printf("I am wrong\n");
            exit(1);
        }
        // 采用头插法
        p->houseID = cur_p->CurPos;
        p->next = cur_p->HouseId;
        cur_p->HouseId = p;
    }
}

void upOwnBlock(PLAYER *cur_p)
{
    int curPos = cur_p->CurPos;
    MAPBLOCK *currentBlock = &(game.map)[curPos];
    int currentCost = currentBlock->MapValue;
    if (currentBlock->HouseType != LAND)
    { // 该地皮不是空地，不能升级
        printf("当前所在地块不是可升级地块!\n");
        return;
    }

    if (currentBlock->HouseOwnerId != cur_p->PlayerId)
    { // 不是自己的地皮，不能购买
        printf("当前所在地块不属于你！无法升级\n");
        return;
    }

    if (currentCost > cur_p->Money)
    { // 现金不够，不能升级
        printf("身上钱不足以升级所在地块\n");
        return;
    }
    /*按照一般大富翁的逻辑购买完成后不能立即升级,但是*************************************************待定*/
    /*
    if (cur_p->CurPos == cur_p->HouseId->houseID){
        printf("You have just bought it,you can not up it!\n");
        return;
    }
    */
    // currentBlock.house_flag;/*******************************************************************待定*/
    if (currentBlock->HouseLevel < 3)
    {
        currentBlock->HouseLevel++;
        currentBlock->rentAmount += currentCost * 2;
        cur_p->Money -= currentCost;
    }
    else
    {
        printf("Your land is already 3 level!\n");
        return;
    }
}

/*
 *cur_p为指向当前玩家的指针
 *num是玩家要出售的房产在地图上的绝对位置
 */
void sellOwnBlock(PLAYER *cur_p, int num)
{
    if (num <= 0 || num > MAPSIZE)
    { // 判断输入位置的合法性
        printf("You input a wrong number,please reinput\n");
        return;
    }
    MAPBLOCK *blockToSell = &(game.map[num]);
    if (blockToSell->HouseType != LAND)
    { // 该地皮不是空地，不能出售
        printf("The block is not land,you can not sell it!\n");
        return;
    }
    if (blockToSell->HouseOwnerId != cur_p->PlayerId)
    { // 不是自己的地皮，不能出售
        printf("The land is not owned by you,you can not sell it!\n");
        return;
    }
    printf("出售成功！\n");
    // 出售完成，更新状态
    cur_p->Money += blockToSell->MapValue * (blockToSell->HouseLevel) * 2;
    blockToSell->HouseOwnerId = -1; // 房屋ID恢复为初始ID
    blockToSell->HouseLevel = 0;    // 房屋等级恢复为初始等级
    // currentBlock.house_flag;/*******************************************************************待定*/
    // 把该空地从玩家名下房产中删除
    LOCATION *p = cur_p->HouseId, *q = NULL;
    while (p != NULL)
    {
        if (p->houseID == num)
        {
            break;
        }
        q = p;
        p = p->next;
    }
    q = p->next;
    free(p);
}

int _canUseBomb(int pos)
{
    MAPBLOCK block = game.map[pos];
    return block.PlayerId == -1 && !(block.HouseType == HOSPITAL || block.HouseType == JAIL) && block.ItemType == NONe;
}

int _canUseBlock(int pos)
{
    MAPBLOCK block = game.map[pos];
    return block.PlayerId == -1 && !(block.HouseType == HOSPITAL || block.HouseType == JAIL) && block.ItemType == NONe;
}

int useBomb(PLAYER *p, int dis)
{
    int ret = FAIL;
    if (p->BombNum <= 0)
    {
        printf("您没有炸弹！\n");
    }
    else if (dis < -10 || dis > 10)
    {
        printf("炸弹使用距离为10米以内！\n");
    }
    else if (_canUseBomb((p->CurPos + dis + MAPSIZE) % MAPSIZE))
    {
        game.map[p->CurPos + dis].ItemType = BOMb;
        p->BombNum--;

        ret = SUCCESS;
        cleanCommandWindow();
        printMap(game);
        _CUR_INPUT
        printf("使用炸弹成功！\n");
    }
    else
    {
        printf("当前位置不能放置炸弹！\n");
    }
    return ret;
}

int useBlock(PLAYER *p, int dis)
{
    int ret = FAIL;
    if (p->BlockNum <= 0)
    {
        printf("您没有路障！\n");
    }
    else if (dis < -10 || dis > 10)
    {
        printf("路障使用距离为10米以内！\n");
    }
    else if (_canUseBlock((p->CurPos + dis + MAPSIZE) % MAPSIZE))
    {
        game.map[p->CurPos + dis].ItemType = BLOCk;
        p->BlockNum--;
        ret = SUCCESS;

        _CUR_SPAWN
        printMap(game);
        cleanCommandWindow();
        _CUR_INPUT
        printf("使用路障成功！%d\n", game.map[p->CurPos + dis].ItemType);
    }
    else
    {
        printf("当前位置不能放置路障！\n");
    }
    return ret;
}

int useRobot(PLAYER *p)
{
    int ret = FAIL;
    if (p->RobotNum <= 0)
    {
        printf("您没有机器娃娃！\n");
    }
    else
    {
        for (int i = 1; i <= 10; ++i)
        {
            game.map[p->CurPos + i].ItemType = NONe;
        }
        printf("使用机器娃娃成功！\n");
        ret = SUCCESS;
    }
    return ret;
}

void timer(long sleep_time, int sig)
{
    clock_t start;
    start = clock();
    if (sig == NOCLOCK)
    {
        return;
    }
    else
    {
        while (1)
        {
            if ((clock() - start) > CLOCKS_PER_SEC * sleep_time)
            {
                break;
            }
            else
            {
            }
        }
        return;
    }
}

void payRent(PLAYER *from, PLAYER *to, int amount)
{
    from->Money -= amount;
    to->Money += amount;
}

void enterItemShop(PLAYER *cur_p)
{
    // 当调用函数就说明已经进入了道具屋，需要进行购买等操作，并且进行是否能够购买的判定
    if (cur_p->Point < BLOCK_POINTS)
    {
        printf("你的点数不足以购买道具！\n");
        return;
    }
    else
    {
        printf("欢迎光临道具屋，请选择你所需要的道具：\n");
        printf("1--路障：%d    2--机器娃娃：%d    3--炸弹：%d\n", BLOCK_POINTS, ROBOT_POINTS, BOMB_POINTS);
        printf("目前你有 %d 个点数\n", cur_p->Point);
    }
    // 取得输入
    while (1)
    {
        if (cur_p->Point < BLOCK_POINTS)
        {
            printf("你的点数不足以再购买道具，退出\n");
            return;
        }
        int item_num = getNumberInput_1();
        if (item_num == 70)
        {
            // 70表示F
            printf("退出道具屋！\n");
            return;
        }
        else
        {
            if (cur_p->BlockNum + cur_p->BombNum + cur_p->RobotNum < 10)
            {
                if (item_num == 1)
                {
                    if (cur_p->Point >= BLOCK_POINTS)
                    {
                        printf("你购买了一个路障，花费 %d 点数\n", BLOCK_POINTS);
                        cur_p->BlockNum++;
                        cur_p->Point -= BLOCK_POINTS;
                    }
                }
                else if (item_num == 2)
                {
                    if (cur_p->Point >= BLOCK_POINTS)
                    {
                        printf("你购买了一个机器娃娃，花费 %d 点数\n", ROBOT_POINTS);
                        cur_p->RobotNum++;
                        cur_p->Point -= ROBOT_POINTS;
                    }
                }
                else if (item_num == 2)
                {
                    if (cur_p->Point >= BLOCK_POINTS)
                    {
                        printf("你购买了一个炸弹，花费 %d 点数\n", BOMB_POINTS);
                        cur_p->BombNum++;
                        cur_p->Point -= BOMB_POINTS;
                    }
                }
                else
                {
                    printf("没有所选的道具！\n");
                    continue;
                }
            }
            else
            {
                printf("道具已满，不能再购买\n");
                break;
            }
        }
    }
    return;
}

void enterMagicHouse(PLAYER *cur_p)
{
    printf("魔法屋还没有开业\n");
    timer(1, ONCLOCK);
}

void enterGiftShop(PLAYER *cur_p)
{
    printf("进入礼品屋，请选择你的礼品: \n");
    printf("1---奖金（%d）    2---点数（%d）    3---财神时间（%d轮）\n", GIFTMONEY, GIFTPOINT, GIFTCSROUND);
    int gift_num = getNumberInput_1_123();
    // 只有一次选择机会，因此只判定一次即可
    if (gift_num == -1)
    {
        return;
    }
    else if (gift_num == 1)
    {
        // 奖金
        cur_p->Money += GIFTMONEY;
    }
    else if (gift_num == 2)
    {
        // 点数卡
        cur_p->Point += GIFTPOINT;
    }
    else if (gift_num == 3)
    {
        // 财神
        cur_p->BuffTime = GIFTCSROUND + 1;
    }
    else
    {
        // 异常返回
        return;
    }
    return;
}

/*_ _表示该函数为测试版本*/

/*此函数测试时可以用来设置某块地皮归某一玩家所有*/
void _buyEmptyBlock_(int playerNumber, int num, int level)
{
    PLAYER *cur_p = game.current_player;
    MAPBLOCK *blockToHave = &(game.map)[num];
    int currentCost = blockToHave->MapValue;

    while (cur_p->PlayerId != playerNumber)
    {
        cur_p = cur_p->next;
    }

    // 判断该地皮是否已经归属于其他人，如果有则假装出售之
    if (blockToHave->HouseOwnerId != -1)
    {
        PLAYER *p = cur_p;
        while (p != NULL)
        {
            if (p->PlayerId == blockToHave->HouseOwnerId)
                break;
            p = p->next;
        }
        _sellOwnBlock_(num);
    }

    blockToHave->HouseOwnerId = cur_p->PlayerId;
    blockToHave->HouseLevel++;

    // 把该空地加入该玩家名下房产
    LOCATION *p = (LOCATION *)malloc(sizeof(LOCATION));
    if (p == NULL)
    {
        printf("I am wrong\n");
        exit(1);
    }
    p->houseID = num;
    p->next = cur_p->HouseId;
    cur_p->HouseId = p;

    _upOwnBlock_(cur_p, num, level);
}

void _upOwnBlock_(PLAYER *cur_p, int num, int level)
{
    MAPBLOCK *blockToHave = &(game.map)[num];
    blockToHave->HouseLevel = level;
}

void _sellOwnBlock_(int num)
{
    MAPBLOCK *blockToSell = &game.map[num];
    PLAYER *player = game.current_player;
    for (int i = 0; i < game.play_num; i++)
    {
        if (blockToSell->HouseOwnerId == player->PlayerId)
            break;
        player = player->next;
    }
    blockToSell->HouseOwnerId = -1; // 房屋ID恢复为初始ID
    blockToSell->HouseLevel = -1;   // 房屋等级恢复为初始等级
    LOCATION *p = player->HouseId, *q = player->HouseId;
    while (p != NULL)
    {
        if (p->houseID == num)
        {
            break;
        }
        q = p;
        p = p->next;
    }
    q->next = p->next;
    free(p);
}

void _setSleepTime_(PLAYER *pPlayer, int n)
{
    pPlayer->SleepTime = n;
}

// get playerID of a map
int getPlayerID(char Name)
{
    int ID = -1;
    for (int i = 0; i < 4; i++)
    {
        if (Name == 'Q')
            ID = 0;
        else if (Name == 'A')
            ID = 1;
        else if (Name == 'S')
            ID = 2;
        else if (Name == 'J')
            ID = 3;
    }
    return ID;
}

// debug
int _useBomb_(int loc)
{
    int ret = FAIL;
    game.map[loc].ItemType = BOMB;
    ret = SUCCESS;
    return ret;
}

// debug
int _useBlock_(int loc)
{
    int ret = FAIL;
    game.map[loc].ItemType = BLOCK;
    ret = SUCCESS;
    return ret;
}

// Name: the first letter of user name
// loc: destination of map
void setUserLoc(char Name, int loc)
{
    PLAYER *pPlayer = game.player;
    for (int i = 0; i < game.play_num; i++)
    {
        if (pPlayer->Name[0] == Name)
        {
            game.map[pPlayer->CurPos].PlayerId = -1;
            pPlayer->CurPos = loc;
            game.map[pPlayer->CurPos].PlayerId = getPlayerID(Name);
        }
        pPlayer = pPlayer->next;
    }
}

// debug function
void _choosePlayer_(char *playerList)
{
    // 输入玩家个数
    int playerNum = strlen(playerList) - 1;     //-1是因为从文件中读取的一行字符串包括了换行符\n, strlen()函数会计入这一个'\n'

    // 显示选择角色的提示信息
    char *playerNameList[4] = {"Q", "A", "S", "J"};
    // printf("Choose\n");
    // printf("1:Q 2:A 3:S 4:J\n");

    // 错误输入检测
    while (1)
    {
        // 输入选择角色的序列
        int len = strlen(playerList) - 1;
        if (len < playerNum)
        {
            // printf("输入位数太少!\n");
        }
        else if (len > playerNum)
        {
            // printf("输入位数过多!\n");
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
                // printf("不能重复选择同一个角色!\n");
            }
            else
            {
                // printf("没有该角色编号!\n");
            }
        }
    }

    // 创建角色链表
    PLAYER *firstPlayer, *p;
    for (int i = 0; i < playerNum; i++)
    {
        PLAYER *player = (PLAYER *)malloc(sizeof(PLAYER));
        int playerId = playerList[i] - '1';
        player->PlayerId = playerId;
        player->dead = 0;
        player->BuffTime = 0;
        player->Name = playerNameList[playerId];
        player->CurPos = 0;
        player->HouseId = NULL;
        player->BlockNum = 1;
        player->RobotNum = 1;
        player->BombNum = 1;
        player->SleepTime = 0;
        player->Point = 0;
        player->MovingDis = 0;
        player->Money = initMoney;
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