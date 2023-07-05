#include "base.h"

// 玩家在前进道路上所处的状态
#define BOMBED 1
#define BLOCKED 2
#define OK 3

// 前进路上的道具状态
#define ENCOUNTERED 1  // 表示有某个道具
#define NONE_ITEM 2

int afterActionJudge(PLAYER *cur_p);
void onSiteActionJudge(cur_p, judge_sig);


MOVING_ENCOUNTERED * passedLandStatusSync(PLAYER *p);
