#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

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

int action(int sig, PLAYER * p) {
    if(sig == ROLL) {
        int i = roll();
        _step(i, p);
        printf("do roll ! You get num: %d\n", i);
        return ROLL;
    }
    else if(sig == SELL) {
        printf("do sell\n");
    }
    else if(sig == BLOCK) {
        printf("do block\n");
    }
    else if(sig == BOMB) {
        printf("do bomb\n");
    }
    else if(sig == ROBOT) {
        printf("do robot\n");
    }
    else if(sig == QUERY) {
        printStatus(p);
    }
    else if(sig == HELP) {
        printHelp();
    }
    else if(sig == QUIT) {
        printf("do quit\n");
    }
    else if(sig == STEP) {
        printf("do step\n");
    }
    else if(sig == ERROR) {
        printf("error\n");
    }
    else {
        printf("no actions\n");
    }
}

int roll() {
    return rand() % 6 + 1;
}

void playerChangePos(PLAYER * p, int pos) {
    p->CurPos+=pos;
    p->CurPos = p->CurPos % MAPSIZE;
}

void exitGame() {

}

int blocked(int dis) {
    // TODO:
    // 如果blocked, 位置设置为被block的位置,返回1，否则0
    return 0;
}

void _step(int dis, PLAYER * cur_p) {
    if (!blocked(dis)) {
        // TODO:多个玩家在一起，原来位置PLayerId不是-1
         game.map[cur_p->CurPos].PlayerId = -1;
        _playerChangePos(cur_p, dis);
    }

    game.map[cur_p->CurPos].PlayerId = cur_p->PlayerId;

}
void _playerChangePos(PLAYER * p, int pos) {
    p->CurPos+=pos;
    p->CurPos = p->CurPos % MAPSIZE;
}

