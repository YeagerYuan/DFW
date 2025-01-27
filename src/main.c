#include <stdio.h>
#include "display.h"
#include "base.h"
#include "play.h"
#include "input.h"
#include "judge.h"

GAME game;
int initMoney = INITIALMONEY;
// 游戏回合数
int round_nums = 0;
int ContinueGame();
int main()
{
    // 游戏回合数
    _COLOR_INIT
    initGame(NULL, NULL);
    PLAYER *cur_p;
    int player_sig = 0;
    int action_sig = 0;
    int judge_sig = 0;
    int action_pos = -1;
    _COLOR_INIT
    printMap(game);
    cur_p = game.player;
    fflush(stdout);
    while (ContinueGame())
    {
        // 整个游戏过程循环
        cleanCommandWindow();
        printMap(game);
        printLine(COMMAND_WINDOW);
        _CUR_INPUT

        if (cur_p->dead == 1)
        {
            cur_p = cur_p->next;
            continue;
        }
        else if (cur_p->SleepTime > 0)
        {
            cur_p->SleepTime--;
            printf("该轮跳过！您还需跳过%d轮！\n", cur_p->SleepTime);
            timer(2, ONCLOCK);
            cur_p = cur_p->next;
            continue;
        }

        _GREEN printf("现在是你的回合! 当前动作的玩家是：%s\n", cur_p->Name);
        _WHITE

        while (1)
        {
            // 单个玩家的输入循环
            player_sig = getInput(&action_pos);
            if (player_sig == ERROR)
            {
                printf("命令不存在！请重新输入\n");
                continue;
            }
            else {
                action_sig = action(player_sig, cur_p, action_pos);
                if(action_sig == ROLL || action_sig == STEP) {
                    timer(1, NOCLOCK);
                    break;
                }
            }
            // 本次循环退出也就意味着要重新刷新屏幕并给到下一个人进行输入循环
        }
        judge_sig = afterActionJudge(cur_p);

        playerPosToMap(cur_p);
        _CUR_SAVE_POS
        _CUR_SPAWN
        printMap(game);
        _CUR_REFORM_POS

        onSiteActionJudge(cur_p, judge_sig);

        playerPosToMap(cur_p);

        cur_p = cur_p->next;
        refreshGodTime(round_nums);    //生成财神
        round_nums++;
    }
}

int ContinueGame()
{
    int aliveNum = game.play_num;
    PLAYER *p = game.player->next;
    while (p != game.player)
    {
        if (p->dead)
        {
            aliveNum--;
        }
        p = p->next;
    }
    return aliveNum != 1;
}
