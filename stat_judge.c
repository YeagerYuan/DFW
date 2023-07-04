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
    memset(info, NONE, sizeof(info));
    if(cur_p->CurPos - cur_p->CurPos <= 0) {
        return FAIL;
    }
    int i = 0;
    for(i = cur_p->LastPos;i<=cur_p->CurPos;i++) {
        // 遍历将会走过的所有位置
        if(game.map[i].ItemType == BOMB) {
            info->been_bombed = ENCOUNTERED;
            info->bombed_pos = i;
            // 当已经检测到炸弹时，退出，不继续检测
            break;
        }
        else if(game.map[i].ItemType == BLOCK) {
            info->been_blocked = ENCOUNTERED;
            info->blocked_pos = i;
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
        bombed(cur_p);
        free(moving);
        return BLOCKED;
    }
    else {
        return OK;
    }
}

int onSiteActionJudge(PLAYER *cur_p) {

}

void bombed(PLAYER *cur_p) {
    cur_p->SleepTime = BOMBED_SLEEP_ROUND;
    game.map[cur_p->CurPos].PlayerId = -1;
    cur_p->CurPos = HOSPITAL_POS;
    // TODO
    // 当玩家被送到医院时，是否显示该玩家的图标？
    return;
}

int blocked(PLAYER *cur_p, int blocked_pos) {
    return -1;
}

