#include "test.h"
#include "play.h"
#include "judge.h"

GAME game;
int initMoney = INITIALMONEY;
int DEBUGMODE = 0;

/* test input */
void inputGame(int j)
{
    int judge_sig = 0;
    FILE *file = NULL;
    char command[MAX_LENGTH];
    // static int j = 0;       // number of input_.txt
    char n_j[10];
    char addr[50] = "input/input_";
    itoa(j, &n_j, 10);          //int -> char
    strcat(addr, n_j);
    strcat(addr, ".txt");
    file = fopen(addr, "r");

    if (file == NULL) {
        //printf("open file failed.\n");
        return;
    }
    // read input based on line
    while (fgets(command, MAX_LENGTH, file) != NULL) {
        char* buf = strtok(command, " ");   // String splitting, taking out the string before the first space

        char* ptr = NULL;
        int num = (int)strtol(command,&ptr,0);

        if(num != 0)
        {
            initMoney = num;
            fgets(command, MAX_LENGTH, file);

            initMap();
            _choosePlayer_(command);

            // char *pbuf = command;
            // int _player_nums = strlen(command);
            // _player_nums--;
            // FILE *buf_txt = freopen("buf.txt", "w", stdout);
            // printf("%d ", _player_nums);
            // while(*pbuf != '\0')      // get nums of players
            // {
            //     if(*pbuf == '1')    printf("1");
            //     else if(*pbuf == '2')   printf("2");
            //     else if(*pbuf == '3')   printf("3");
            //     else if(*pbuf == '4')   printf("4");
            //     // else    printf("%c", *pbuf);
            //     pbuf++;
            // }
            // fclose(buf_txt);

            // // // output into dump after origin info
            // // FILE* buf_txt_dump = freopen("dump/dump_0.txt", "a", stdout);   
            // // read from buftxt
            // buf_txt = freopen("buf.txt", "r",stdin);
            // char test[10];

            // // unit test
            // initMap();
            // choosePlayer();

            // fclose(buf_txt);
            // // fclose(buf_txt_dump);
            continue;
        }

        // Preset mode
        else if(!strcmp(buf, "set"))
        {
            buf = strtok(NULL, " ");        // continue splitting

            if(!strcmp(buf, "player"))
            {
                buf = strtok(NULL, " ");
                char *pbuf = buf;
                int _player_nums = strlen(buf);
                _player_nums--;
                FILE *buf_txt = freopen("buf.txt", "w", stdout);
                printf("%d ", _player_nums);
                while(*pbuf != '\0')      // get nums of players
                {
                    if(*pbuf == 'Q')    printf("1");
                    else if(*pbuf == 'A')   printf("2");
                    else if(*pbuf == 'S')   printf("3");
                    else if(*pbuf == 'J')   printf("4");
                    // else    printf("%c", *pbuf);
                    pbuf++;
                }
                fclose(buf_txt);

                // // output into dump after origin info
                // FILE* buf_txt_dump = freopen("dump/dump_0.txt", "a", stdout);   
                // read from buftxt
                buf_txt = freopen("buf.txt", "r",stdin);
                char test[10];

                // unit test
                initMap();
                choosePlayer();

                fclose(buf_txt);
                // fclose(buf_txt_dump);
                continue;
            }

            else if(!strcmp(buf, "money"))
            {
                buf = strtok(NULL," ");
                if(!strcmp(buf,"Q")){
                    buf = strtok(NULL," ");
                    int moneySet = (int)strtol(buf,'\0',0);
                    setInitialFund(0,moneySet);
                }
                else if(!strcmp(buf,"A")){
                    buf = strtok(NULL," ");
                    int moneySet = (int)strtol(buf,'\0',0);
                    setInitialFund(1,moneySet);
                }
                else if(!strcmp(buf,"S")){
                    buf = strtok(NULL," ");
                    int moneySet = (int)strtol(buf,'\0',0);
                    setInitialFund(2,moneySet);
                }
                else if(!strcmp(buf,"J")){
                    buf = strtok(NULL," ");
                    int moneySet = (int)strtol(buf,'\0',0);
                    setInitialFund(3,moneySet);
                }
                continue;
            }

            else if(!strcmp(buf, "point"))
            {
                buf = strtok(NULL," ");
                if(!strcmp(buf,"Q")){
                    buf = strtok(NULL," ");
                    int pointSet = (int)strtol(buf,'\0',0);
                    setInitialPoint(0,pointSet);
                }
                else if(!strcmp(buf,"A")){
                    buf = strtok(NULL," ");
                    int pointSet = (int)strtol(buf,'\0',0);
                    setInitialPoint(1,pointSet);
                }
                else if(!strcmp(buf,"S")){
                    buf = strtok(NULL," ");
                    int pointSet = (int)strtol(buf,'\0',0);
                    setInitialPoint(2,pointSet);
                }
                else if(!strcmp(buf,"J")){
                    buf = strtok(NULL," ");
                    int pointSet = (int)strtol(buf,'\0',0);
                    setInitialPoint(3,pointSet);
                }
                continue;
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
                    setInitialItem(0,item,itemNum);
                }
                else if(!strcmp(buf,"A")){
                    /*获取道具*/
                    buf = strtok(NULL," ");
                    int item = (int)strtol(buf,'\0',0);
                    /*获取数量*/
                    buf = strtok(NULL," ");
                    int itemNum=(int)strtol(buf,'\0',0);
                    setInitialItem(1,item,itemNum);
                }
                else if(!strcmp(buf,"S")){
                    /*获取道具*/
                    buf = strtok(NULL," ");
                    int item = (int)strtol(buf,'\0',0);
                    /*获取数量*/
                    buf = strtok(NULL," ");
                    int itemNum=(int)strtol(buf,'\0',0);
                    setInitialItem(2,item,itemNum);
                }
                else if(!strcmp(buf,"J")){
                    /*获取道具*/
                    buf = strtok(NULL," ");
                    int item = (int)strtol(buf,'\0',0);
                    /*获取数量*/
                    buf = strtok(NULL," ");
                    int itemNum=(int)strtol(buf,'\0',0);
                    setInitialItem(3,item,itemNum);
                }
                continue;
            }

            else if(!strcmp(buf, "buff"))
            {
                buf = strtok(NULL," ");
                if(!strcmp(buf,"Q")){
                    buf = strtok(NULL," ");
                    int buffTimeSet = (int)strtol(buf,'\0',0);
                    setInitialBuff(0,buffTimeSet);
                }
                else if(!strcmp(buf,"A")){
                    buf = strtok(NULL," ");
                    int buffTimeSet = (int)strtol(buf,'\0',0);
                    setInitialBuff(1,buffTimeSet);
                }
                else if(!strcmp(buf,"S")){
                    buf = strtok(NULL," ");
                    int buffTimeSet = (int)strtol(buf,'\0',0);
                    setInitialBuff(2,buffTimeSet);
                }
                else if(!strcmp(buf,"J")){
                    buf = strtok(NULL," ");
                    int buffTimeSet = (int)strtol(buf,'\0',0);
                    setInitialBuff(3,buffTimeSet);
                }
                continue;
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
                    _buyEmptyBlock_(0,blockNum,level);
                }
                else if(!strcmp(whichPlayer,"A")){
                    _buyEmptyBlock_(1,blockNum,level);
                }
                else if(!strcmp(whichPlayer,"S")){
                    _buyEmptyBlock_(2,blockNum,level);
                }
                else if(!strcmp(whichPlayer,"J")){
                    _buyEmptyBlock_(3,blockNum,level);
                }
                continue;
            }

            // set unmap [n] // 将可购买的地皮n设置为未购买状态
            else if(!strcmp(buf,"unmap"))
            {
                buf = strtok(NULL," ");
                int blockNum = (int)strtol(buf,'\0',0);
                _sellOwnBlock_(blockNum);
                continue;
            }

            else if(!strcmp(buf, "bomb"))
            {
                buf = strtok(NULL, " ");
                int k = atoi(buf);      //data read from input.txt
                if(game.map[k].PlayerId == -1)
                {
                    _useBomb_(k);
                }
                continue;
            }

            else if(!strcmp(buf, "barrier"))
            {
                buf = strtok(NULL, " ");
                int k = atoi(buf);      //data read from input.txt
                if(game.map[k].PlayerId == -1)
                {
                    _useBlock_(k);
                }
                continue;
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
                        // mode +
                        if (buf[0] == '+')
                        {
                            buf++;
                            int n = atoi(buf);
                            _step(n, pPlayer);
                        }
                        // mode -
                        else if (buf[0] == '-')
                        {
                            buf++;
                            int n = atoi(buf);
                            n = MAPSIZE - n;
                            _step(n, pPlayer);
                        }
                        // mode set loc
                        else
                        {
                            int n = atoi(buf);
                            setUserLoc(pPlayer->Name[0], n);

                            // refresh state


                        }
                    }
                    pPlayer = pPlayer->next;
                }
                continue;
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
                        _setSleepTime_(pPlayer, atoi(buf));
                    }
                    pPlayer = pPlayer->next;
                }
                continue;
                
            }
        }

        else if(!strcmp(buf, "step"))
        {

            buf = strtok(NULL, " ");
            int i = atoi(buf);          //char -> int
            /* unit test */
            _playerChangePos(game.current_player, i);

            // round end
            // judge_sig = afterActionJudge(game.current_player);
            //round begin
            // onSiteActionJudge(game.current_player, judge_sig);

            // buyEmptyBlock(game.current_player);
            // char read_buf[MAX_LENGTH];
            // fgets(read_buf, MAX_LENGTH, file);
            // FILE* read_out_txt = fopen("read_buf.txt","w");
            // fprintf("%c",read_buf);
            // FILE* read_buf_txt = freopen("read_buf.txt", "r",stdin);
            
            // refresh state
            game.current_player = game.current_player->next;    // End the current round 

            continue;
        }


        else if(!strcmp(buf, "dump"))
        {
            // Dump();
            continue;
        }

        else if(!strcmp(buf, "quit"))
        {
            break;
        }
    }

    // j++;    //next test sample
    fclose(file);
    return ;
}

/* get game state & print into file */
void Dump(int j){
    PLAYER * pPlayer = game.player;
    FILE * fp = NULL;
    // static int j = 0;       // number of dump_.txt
    char n_j[10];
    char addr[50] = "dump/dump_";
    itoa(j, &n_j, 10);
    // addr[10] = n_j;
    strcat(addr, n_j);
    strcat(addr,".txt");

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

    // j++;    // next dump
    // printf("output success");
    fclose(fp);
    return ;
}


int main()
{
    DEBUGMODE = 1;
    for(int m=0;m<1000;m++)
    {
        inputGame(m);
        Dump(m);
    }
    return 0;
}
