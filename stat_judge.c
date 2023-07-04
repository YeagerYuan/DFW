#include <stdio.h>
#include <malloc.h>

#include "base.h"
#include "play.h"
#include "judge.h"


extern GAME game;

MOVING_ENCOUNTERED * passedLandStatusSync(PLAYER *cur_p) {
    MOVING_ENCOUNTERED * info = (MOVING_ENCOUNTERED *)malloc(sizeof(MOVING_ENCOUNTERED));
    if(info == NULL) {
        printf("run out of mem.\n");
        exit(1);
    }
    if(cur_p->MovingDis <= 0) {
        exit(1);
    }
    memset(info, 0, sizeof(MOVING_ENCOUNTERED));
    int i = 0, pos_now = cur_p->CurPos;
    if(pos_now < cur_p->MovingDis) {
        pos_now+=MAPSIZE;
    }
    for(i = cur_p->MovingDis;i>=0;i--) {
        // 遍历将会走过的所有位置
        if(game.map[pos_now - i].ItemType == BOMB) {
            printf("在%d号地块被炸\n", pos_now - i);
            timer(2, ONCLOCK);
            info->been_bombed = ENCOUNTERED;
            info->been_blocked = NONE_ITEM;
            info->bombed_pos = pos_now - i;
            // 当已经检测到炸弹时，退出，不继续检测
            break;
        }
        else if(game.map[pos_now - i].ItemType == BLOCK) {
            printf("在%d号地块被路障挡住\n", pos_now - i);
            timer(2, ONCLOCK);
            info->been_blocked = ENCOUNTERED;
            info->been_bombed = NONE_ITEM;
            info->blocked_pos = pos_now - i;
            break;
        }
        else {
            continue;
        }
    }
    return info;
}


int afterActionJudge(PLAYER *cur_p) {
    MOVING_ENCOUNTERED * moving = passedLandStatusSync(cur_p);

    // 需要处理所有状况
    if(moving->been_blocked == ENCOUNTERED) {
        blocked(cur_p, moving->blocked_pos);
        free(moving);
        return BOMBED;
    }
    else if(moving->been_bombed == ENCOUNTERED) {
        bombed(cur_p, moving->bombed_pos);
        free(moving);
        return BLOCKED;
    }
    else {
        return OK;
    }
}

void onSiteActionJudge(PLAYER *cur_p, int judgeSig) {
    if (judgeSig != OK) {
        return;
    }

    MAPBLOCK block = game.map[cur_p->CurPos];
    switch (game.map[cur_p->CurPos].HouseType) {
    case LAND:
        if (block.HouseOwnerId == -1) {
            buyEmptyBlock(cur_p);
        } else if (block.HouseOwnerId != cur_p->PlayerId)
        {
            PLAYER* owner = cur_p;
            while (owner->PlayerId != block.HouseOwnerId)
            {
                owner = owner->next;
            }
            payRent(cur_p, owner, block.rentAmount);
            printf("当前地块主人为%s，您交付租金%d元.\n", block.rentAmount);
            // TODO: 判定破产
        }
    case HOSPITAL:
    case STPOINT:
        break;
    case MAGICHOUSE:
        enterMagicHouse(cur_p);
        break;
    case PROPHOUSE:
        break;
        enterMagicHouse(cur_p);
    case JAIL:
        cur_p->SleepTime = 2;
        break;
    case MINERALFILED:
        cur_p->Point += block.minePoint;
        printf("获取点数：%d\n", block.minePoint);
        break;
    }
}

void bombed(PLAYER *cur_p, int bombed_pos) {
    cur_p->SleepTime = BOMBED_SLEEP_ROUND;
    game.map[cur_p->CurPos].PlayerId = -1;
    cur_p->CurPos = HOSPITAL_POS;
    game.map[bombed_pos].ItemType = NONE;
    // TODO
    // 当玩家被送到医院时，是否显示该玩家的图标？
    return;
}

int blocked(PLAYER *cur_p, int blocked_pos) {
    cur_p->CurPos = blocked_pos;
    game.map[cur_p->CurPos].PlayerId = -1;
    game.map[blocked_pos].ItemType = NONE;
}

