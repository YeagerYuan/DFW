#include <stdio.h>
#include "display.h"
#include "base.h"
#include "map.h"
#include "play.h"
#include "input.h"

GAME game;
int main()
{
    _COLOR_INIT
    initGame(game);
    PLAYER * cur_p;
    int player_sig = 0;
    int action_sig = 0;
    int judge_sig = 0;
    _COLOR_INIT
    printMap(game);
    cur_p = game.player;
    fflush(stdout);
    while(1) {
        // 整个游戏过程循环
        cleanCommandWindow();
        printMap(game);
        printLine(COMMAND_WINDOW);
        _CUR_INPUT
        _GREEN printf("Now it is your turn! Now the action player is %d\n", cur_p->PlayerId);_WHITE
        while(1) {

            judge(cur_p);

            // 单个玩家的输入循环
            player_sig = getInput();
            if(player_sig == ERROR) {
                printf("input wrong command, please reinput.\n");
                continue;
            }
            else if(player_sig == QUIT) {
                printf("skip the turn.\n");
                cleanCommandWindow();
                break;
            }
            else {
                action_sig = action(player_sig, cur_p);
                if(action_sig == ROLL) {
                    break;
                }
            }
            //本次循环退出也就意味着要重新刷新屏幕并给到下一个人进行输入循环
        }
        cur_p = cur_p->next;
    }
}



