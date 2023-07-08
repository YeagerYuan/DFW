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
        int iter_cur = (pos_now - i) % MAPSIZE;
        if(game.map[iter_cur].ItemType == BOMb) {
            printf("在%d号地块被炸\n", iter_cur);
            timer(2, ONCLOCK);
            info->been_bombed = ENCOUNTERED;
            info->been_blocked = NONE_ITEM;
            info->bombed_pos = (pos_now - i) % MAPSIZE;
            // 当已经检测到炸弹时，退出，不继续检测
            break;
        }
        else if(game.map[iter_cur].ItemType == BLOCk) {
            printf("在%d号地块被路障挡住\n", iter_cur);
            timer(2, ONCLOCK);
            info->been_blocked = ENCOUNTERED;
            info->been_bombed = NONE_ITEM;
            info->blocked_pos = iter_cur;
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
    if(cur_p->BuffTime > 0) {
        cur_p->BuffTime--;
    }
    if (judgeSig == BOMBED) {
        return;
    }
    MAPBLOCK block = game.map[cur_p->CurPos];
    switch (game.map[cur_p->CurPos].HouseType) {
    case LAND:
        if (block.HouseOwnerId == -1) {
            buyEmptyBlock(cur_p);
        } else if (block.HouseOwnerId != cur_p->PlayerId)
        {
            if (cur_p->BuffTime > 0) {
                printf("财神附体，免交租金！\n");
            } else {
                PLAYER* owner = cur_p;
                while (owner->PlayerId != block.HouseOwnerId)
                {
                    owner = owner->next;
                }
                if (cur_p->Money < block.rentAmount) {
                    printf("您没有足够现金，您已出局！\n");
                } else {
                    payRent(cur_p, owner, block.rentAmount);
                    printf("当前地块主人为%s，您交付租金%d元.\n", owner->Name, block.rentAmount);
                }
            }
        }
        break;
    case HOSPITAL:
        break;
    case STPOINT:
        break;
    case MAGICHOUSE:
        enterMagicHouse(cur_p);
        break;
    case PROPHOUSE:
        enterItemShop(cur_p);
        break;
    case GIFTHOUSE:
        enterGiftShop(cur_p);
        break;
    case JAIL:
        printf("您进入监狱，将被跳过两轮！\n");
        cur_p->SleepTime = 2;
        break;
    case MINERALFILED:
        cur_p->Point += block.minePoint;
        printf("获取点数：%d\n", block.minePoint);
        break;
    }
    timer(1, ONCLOCK);
}

void bombed(PLAYER *cur_p, int bombed_pos) {
    cur_p->SleepTime = BOMBED_SLEEP_ROUND;
    game.map[cur_p->CurPos].PlayerId = -1;
    game.map[cur_p->CurPos - cur_p->MovingDis].PlayerId = -1;
    cur_p->CurPos = HOSPITAL_POS;
    game.map[bombed_pos].ItemType = NONe;
    // TODO
    // 当玩家被送到医院时，是否显示该玩家的图标？
    return;
}

int blocked(PLAYER *cur_p, int blocked_pos) {
    game.map[cur_p->CurPos].PlayerId = -1;
    cur_p->MovingDis = (blocked_pos - (cur_p->CurPos - cur_p->MovingDis + MAPSIZE) % MAPSIZE + MAPSIZE) % MAPSIZE;
    cur_p->CurPos = blocked_pos;
    game.map[blocked_pos].ItemType = NONe;
}

