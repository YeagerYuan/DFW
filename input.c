#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "input.h"
#include "play.h"
#include "display.h"

extern GAME game;

int getInput(int *num){
    printf("输入命令>>>");
    char inputString[BUF_SIZE] = {'\0'};
    char command[BUF_SIZE / 2] = {'\0'};
    char number[BUF_SIZE / 2] = {'\0'};
    int i = 0,j = 0;
    //int num = 0;
    char *ptr;

    _getString(inputString);
    _stringNormalize(inputString);

    while(inputString[i] != '\0'){
        if(inputString[i]==' ' || inputString[i]=='\t' || inputString[i]=='\n') break;
        command[i] = inputString[i];
        i++;
    }
    while(inputString[i] != '\0'){
        if (inputString[i]==' ' || inputString[i]=='\t' || inputString[i]=='\n') {
            i++;
            continue;
        }
        number[j] = inputString[i];
        i++,j++;
    }
    *num = (int)strtol(number,&ptr, 0);

    if(!strcmp(command,"ROLL") && number[0]=='\0'){
        return ROLL;
    }
    else if(!strcmp(command,"SELL")){
        return SELL;
    }
    else if(!strcmp(command,"BLOCK")){
        if(*num>10 || *num<-10 || *num==0){
            return ERROR;
        }
        else if(*ptr != '\0'){//数字后面有其他输入的情况
            return -1;
        }
        else{//输入正确 调用相关函数
        return BLOCK;
        }
    }
    else if(!strcmp(command,"BOMB")){
        if(*num>10 || *num<-10 || *num==0){
            return -1;
        }
        else if(*ptr != '\0'){//数字后面有其他输入的情况
            return -1;
        }
        else{//输入正确 调用相关函数
        return BOMB;
        }
    }
    else if(!strcmp(command,"ROBOT") && number[0]=='\0'){
        return ROBOT;
    }
    else if(!strcmp(command,"QUERY") && number[0]=='\0'){
        return QUERY;
    }
    else if(!strcmp(command,"HELP") && number[0]=='\0'){
        return HELP;
    }
    else if(!strcmp(command,"BUY") && number[0]=='\0'){
        return BUY;
    }
    else if(!strcmp(command,"UP") && number[0]=='\0'){
        return UP;
    }
    else if(!strcmp(command,"QUIT") && number[0]=='\0'){
        return QUIT;
    }
    else if(!strcmp(command,"STEP")){
        if(*num>6 || *num<1){
            return -1;
        }
        else if(*ptr != '\0'){//数字后面有其他输入的情况
            return -1;
        }
        else{//输入正确 调用相关函数
        return STEP;
        }
    }
    else{
        return -1;
    }
}

char* _stringNormalize(char* ptr){
    char *p = ptr;
    while(*p != '\0'){
        if(*p>='a' && *p<='z') *p = *p + 'A' -'a';
        p++;
    }
    return ptr;
}

char* _getString(char* inputString){
    char ch;
    char *p = inputString;
    ch = getchar();
    while(ch != '\n'){
        *p = ch;
        p++;
        ch = getchar();
    }
    *p = '\0';
    return inputString;
}

void cleanCommandWindow() {
    int i=0;
    _CUR_INPUT
    for(i=0;i<INPUT_LINE_SIZE;i++) {
        _CUR_CLEAR
        _CUR_DOWN
    }
    _CUR_SPAWN
}

/*更改为道具屋的购买输入函数*/
int getNumberInput_1() {
    int num=0;
    char input_buf[BUF_SIZE];
    while(1) {
        memset(input_buf, 0, BUF_SIZE * sizeof(char));
        gets(input_buf);
        if(strlen(input_buf) != 1) {
            printf("请输入单个数字或字母！\n");
            continue;
        }
        else {
            if(input_buf[0] < 48 || input_buf[0] > 57 || input_buf[0] != 70) {
                printf("请输入数字 0--9 或 F !\n");
                continue;
            }
            else {
                if(input_buf[0] >= 48 && input_buf[0] <= 57) {
                    num = input_buf[0] - '1';
                    break;
                }
                else if(input_buf[0] == 70) {
                    num=70; // 表示F
                    break;
                }
            }
        }
    }
    return num;
}

int getNumberInput_1_123() {
    int num=0;
    char input_buf[BUF_SIZE];
    while(1) {
        memset(input_buf, 0, BUF_SIZE * sizeof(char));
        gets(input_buf);
        if(strlen(input_buf) != 1) {
            printf("放弃机会！\n");
            return -1;
        }
        else {
            if(input_buf[0] < 48 || input_buf[0] > 57 || input_buf[0] != 70) {
                printf("放弃机会！\n");
                return -1;
            }
            else {
                if(input_buf[0] >= 49 && input_buf[0] <= 51) {
                    num = input_buf[0] - '1';
                    break;
                }
            }
        }
    }
    return num;
}

void getSelectedNumChar(int num) {
    char input_buf[BUF_SIZE];
    while(1) {
        memset(input_buf, 0, BUF_SIZE * sizeof(char));
        gets(input_buf);
        if(strlen(input_buf) != num) {
            printf("input length not match! please reinput.\n");
            continue;
        }
        else {
            if(input_buf[0] < 48 || input_buf[0] > 57) {
                printf("input must be 0--9, please reinput.\n");
                continue;
            }
            num = input_buf[0] - '1';
            break;
        }
    }
}
