#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "input.h"
#include "play.h"
#include "display.h"

extern GAME game;

/*
*�������ж��Ƿ��ǲ����õ�set����
*/
int getInput(int* num) {
    //printf("I am right 3\n");
    //getchar();
    printf("������>>>");
    char inputString[BUF_SIZE] = { '\0' };
    char inputStringTest[BUF_SIZE] = { '\0' };
    char command[BUF_SIZE / 2] = { '\0' };
    char number[BUF_SIZE / 2] = { '\0' };
    char* buf_1, * buf_2;
    int i = 0, j = 0;
    //int num = 0;
    char* ptr;
    _getString(inputString);
    //printf("I am right 0\n");
    //getchar();
    strcpy(inputStringTest, inputString);
    buf_1 = strtok(inputStringTest, " ");
    if (strcmp(buf_1, "set")) {
        _stringNormalize(inputString);
        while (inputString[i] != '\0') {
            if (inputString[i] == ' ' || inputString[i] == '\t' || inputString[i] == '\n') break;
            command[i] = inputString[i];
            i++;
        }
        while (inputString[i] != '\0') {
            if (inputString[i] == ' ' || inputString[i] == '\t' || inputString[i] == '\n') {
                i++;
                continue;
            }
            number[j] = inputString[i];
            i++, j++;
        }
        *num = (int)strtol(number, &ptr, 0);
        if (!strcmp(command, "ROLL") && number[0] == '\0') {
            return ROLL;
        }
        else if (!strcmp(command, "SELL")) {
            return SELL;
        }
        else if (!strcmp(command, "BLOCK")) {
            if (*num > 10 || *num < -10 || *num == 0) {
                return ERROR;
            }
            else if (*ptr != '\0') {//���ֺ�����������������
                return -1;
            }
            else {//������ȷ ������غ���
                return BLOCK;
            }
        }
        else if (!strcmp(command, "BOMB")) {
            if (*num > 10 || *num < -10 || *num == 0) {
                return -1;
            }
            else if (*ptr != '\0') {//���ֺ�����������������
                return -1;
            }
            else {//������ȷ ������غ���
                return BOMB;
            }
        }
        else if (!strcmp(command, "ROBOT") && number[0] == '\0') {
            return ROBOT;
        }
        else if (!strcmp(command, "QUERY") && number[0] == '\0') {
            return QUERY;
        }
        else if (!strcmp(command, "HELP") && number[0] == '\0') {
            return HELP;
        }
        else if (!strcmp(command, "BUY") && number[0] == '\0') {
            return BUY;
        }
        else if (!strcmp(command, "UP") && number[0] == '\0') {
            return UP;
        }
        else if (!strcmp(command, "QUIT") && number[0] == '\0') {
            return QUIT;
        }
        else if (!strcmp(command, "STEP")) {
            if (*num > MAPSIZE || *num < 0) {
                return -1;
            }
            else if (*ptr != '\0') {//���ֺ�����������������
                return -1;
            }
            else {//������ȷ ������غ���
                return STEP;
            }
        }
        else {
            return -1;
        }
    }
    else {
        //printf("I am right 1\n");
        //getchar();
        puts(inputString);
        buf_2 = strtok(inputString, " ");
        if (!strcmp(buf_2, "set")) {
            buf_2 = strtok(NULL, " ");
            if (!strcmp(buf_2, "money")) {
                buf_2 = strtok(NULL, " ");
                if (!strcmp(buf_2, "Q")) {
                    buf_2 = strtok(NULL, " ");
                    int inputNumber = (int)strtol(buf_2, '\0', 0);
                    *num = inputNumber;
                    return 100;
                }
                else if (!strcmp(buf_2, "A")) {
                    buf_2 = strtok(NULL, " ");
                    int inputNumber = (int)strtol(buf_2, '\0', 0);
                    *num = inputNumber;
                    return 101;
                }
                else if (!strcmp(buf_2, "S")) {
                    buf_2 = strtok(NULL, " ");
                    int inputNumber = (int)strtol(buf_2, '\0', 0);
                    *num = inputNumber;
                    return 102;
                }
                else if (!strcmp(buf_2, "J")) {
                    buf_2 = strtok(NULL, " ");
                    int inputNumber = (int)strtol(buf_2, '\0', 0);
                    *num = inputNumber;
                    return 103;
                }
            }
            else if (!strcmp(buf_2, "point")) {
                //printf("I am right 4\n");
                //getchar();
                buf_2 = strtok(NULL, " ");
                if (!strcmp(buf_2, "Q")) {
                    buf_2 = strtok(NULL, " ");
                    int inputNumber = (int)strtol(buf_2, '\0', 0);
                    *num = inputNumber;
                    return 200;
                }
                else if (!strcmp(buf_2, "A")) {
                    buf_2 = strtok(NULL, " ");
                    int inputNumber = (int)strtol(buf_2, '\0', 0);
                    *num = inputNumber;
                    return 201;
                }
                else if (!strcmp(buf_2, "S")) {
                    buf_2 = strtok(NULL, " ");
                    int inputNumber = (int)strtol(buf_2, '\0', 0);
                    *num = inputNumber;
                    return 202;
                }
                else if (!strcmp(buf_2, "J")) {
                    buf_2 = strtok(NULL, " ");
                    int inputNumber = (int)strtol(buf_2, '\0', 0);
                    *num = inputNumber;
                    return 203;
                }
            }
            else if (!strcmp(buf_2, "buff")) {
                buf_2 = strtok(NULL, " ");
                if (!strcmp(buf_2, "Q")) {
                    buf_2 = strtok(NULL, " ");
                    int inputNumber = (int)strtol(buf_2, '\0', 0);
                    *num = inputNumber;
                    return 300;
                }
                else if (!strcmp(buf_2, "A")) {
                    buf_2 = strtok(NULL, " ");
                    int inputNumber = (int)strtol(buf_2, '\0', 0);
                    *num = inputNumber;
                    return 301;
                }
                else if (!strcmp(buf_2, "S")) {
                    buf_2 = strtok(NULL, " ");
                    int inputNumber = (int)strtol(buf_2, '\0', 0);
                    *num = inputNumber;
                    return 302;
                }
                else if (!strcmp(buf_2, "J")) {
                    buf_2 = strtok(NULL, " ");
                    int inputNumber = (int)strtol(buf_2, '\0', 0);
                    *num = inputNumber;
                    return 303;
                }
            }
        }
        else {
        }
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

/*����Ϊ�����ݵĹ������뺯��*/
int getNumberInput_1() {
    int num=0;
    char input_buf[BUF_SIZE];
    while(1) {
        memset(input_buf, 0, BUF_SIZE * sizeof(char));
        gets(input_buf);
        if(strlen(input_buf) != 1) {
            printf("�����뵥�����ֻ���ĸ��\n");
            continue;
        }
        else {
            if((input_buf[0] < 48 || input_buf[0] > 57) && input_buf[0] != 70) {
                printf("���������� 0--9 �� F !\n");
                continue;
            }
            else {
                if(input_buf[0] >= 48 && input_buf[0] <= 57) {
                    num = input_buf[0] - '0';
                    break;
                }
                else if(input_buf[0] == 70) {
                    num=70; // ��ʾF
                    break;
                }
            }
        }
    }
    return num;
}

int getNumberInput_1_123(OP *o) {
    int num=0;
    char input_buf[BUF_SIZE];
    while(1) {
        memset(input_buf, 0, BUF_SIZE * sizeof(char));
        if(o == NULL) {
            gets(input_buf);
        }
        else {

            input_buf[0] = o->num[0]+'0';// ������ж�
        }
        if(strlen(input_buf) != 1) {
            printf("�������ᣡ\n");
            return -1;
        }
        else {
            if((input_buf[0] < 48 || input_buf[0] > 57) && input_buf[0] != 70) {
                printf("�������ᣡ\n");
                return -1;
            }
            else {
                if(input_buf[0] >= 49 && input_buf[0] <= 51) {
                    num = input_buf[0] - '0';
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
            printf("���볤�Ȳ�ƥ��!����������..\n");
            continue;
        }
        else {
            if(input_buf[0] < 48 || input_buf[0] > 57) {
                printf("�������Ϊ0--9,����������.\n");
                continue;
            }
            num = input_buf[0] - '1';
            break;
        }
    }
}