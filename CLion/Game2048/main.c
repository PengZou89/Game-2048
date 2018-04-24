/*########################################*\
############################################
##|  __ \###############|___  /#############
##| |__) |##############   / / #############
##|  ___/ _ \  _ \ / _  | / / / _ \| | | |##
##| |  |  __/ | | | (_| |/ /_| (_) | |_| |##
##|_|   \___|_| |_|\__  /_____\___/ \____|##
################### __/ |###################
###################|___/####################
############################################
## @brief    Peng Zou's Game-2048         ##
## @author   Peng Zou                     ##
## @date     2018-04-23                   ##
## @version  V1.0                         ##
## Copyright 2018 Peng Zou                ##
\*########################################*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>


#define N 4
#define TG (N * N)
#define RATE 90

int map[N][N];
int blankGrids[TG + 1]; //空格点坐标映射列表，blankGrids[TG]为空格点数量
int score;
bool gameOverFlg;

void gameInit();
bool isGameOver();
void manualPrint();
void gameOverPrint();
void mapPrint();
void gridInit();
int inputHandle(char c);
void blankGridsHandle();
void randInit();
int randPosInt(int bound);
int rand2or4(int rate);

int main() {
    randInit();
    gameInit();

    while (true) {
        inputHandle(getch());

        system("cls");
        manualPrint();
        mapPrint();

        if (isGameOver()) {
            gameOverPrint();
        }
    }

    return 0;
}

void gameInit() {
    manualPrint();

    for (int i = 0; i < TG; i++) {
        map[i / N][i % N] = 0;
        blankGrids[i] = i;
    }
    blankGrids[TG] = TG;
    score = 0;
    gameOverFlg = false;
    gridInit();
    gridInit();

    mapPrint();
}

bool isGameOver() {
    if (blankGrids[TG] != 0) return false;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - 1; j++) {
            if (map[i][j] == map[i][j + 1]) return false;
            if (map[j][i] == map[j + 1][i]) return false;
        }
    }

    gameOverFlg = true;
    return true;
}

void manualPrint() {
    printf("GAME-2048 V1.0 by Peng Zou\n");
    printf("Follow: https://github.com/PengZou89/Game-2048\n");
    printf("Manual:\n");
    printf("  Press 'h'/'j'/'k'/'l' to move left/down/up/right;\n");
    printf("  Press 'R' to Restart;\n");
    printf("  Press 'Q' to Quit;\n\n");
}

void gameOverPrint() {
    printf("Game Over!\n");
    printf("Your score is %d!\n", score);
    printf("Press 'R' to Restart or 'Q' to Quit!\n");
}

void mapPrint() {
    for (int i = 0; i < N; i++) {
        printf("\t");
        for (int j = 0; j < N; j++) {
            if (map[i][j] == 0) {
                printf("-     ");
                continue;
            }
            printf("%-6d", map[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void gridInit() {
    if (blankGrids[TG] <= 0) return;

    //在map里随机取一个空白格点
    int bgPos = randPosInt(blankGrids[TG]);

    //在blank grids里面取map坐标，再随机生成2或4
    int mapPos = blankGrids[bgPos];
    map[mapPos / N][mapPos % N] = rand2or4(RATE);

    blankGridsHandle();
}


int inputHandle(char c) {
    if (gameOverFlg == false) {
        // 1. 有空格点，进行了移位
        // 2. 有相等格点，进行了相加
        bool gridNeedInitFlg = false;
        switch (c) {
            case 'h':
                for (int i = 0; i < N; i++) {
                    //left-justify the nums
                    int k = 0;
                    for (int j = 0; j < N; j++) {
                        if (map[i][j] == 0) continue;
                        // 分情况讨论:
                        // 1. 如果前面没0，如[2 4 8 0]，则不进行前移操作，也就不能init a grid
                        // 2. 如果前面或中间有0，如[0 2 0 4]，则需要init a grid
                        if (map[i][k] != map[i][j]) {
                            map[i][k] = map[i][j];
                            gridNeedInitFlg = true;
                        }
                        k++;
                    }
                    //add the back num to the front one if they are equal
                    for (int j = 0; j < k - 1; j++) {
                        if (map[i][j] == map[i][j + 1]) {
                            map[i][j] += map[i][j + 1];
                            score += map[i][j];
                            map[i][j + 1] = 0;
                            gridNeedInitFlg = true;
                            break;
                        }
                    }
                    //left-justify the nums and set the other nums zero
                    int m = 0;
                    for (int j = 0; j < k; j++) {
                        if (map[i][j] == 0) continue;
                        map[i][m++] = map[i][j];
                    }
                    for (int j = m; j < N; j++) {
                        map[i][j] = 0;
                    }
                }
                //每个case都重复了以下两句，用来防止错误按键更改map及blankGrids状态
                blankGridsHandle();
                if (gridNeedInitFlg) {
                    gridNeedInitFlg = false;
                    gridInit();
                }
                break;
            case 'j':
                for (int j = 0; j < N; j++) {
                    int k = N - 1;
                    for (int i = N - 1; i >= 0; i--) {
                        if (map[i][j] == 0) continue;
                        if (map[k][j] != map[i][j]) {
                            map[k][j] = map[i][j];
                            gridNeedInitFlg = true;
                        }
                        k--;
                    }
                    for (int i = N - 1; i > k + 1; i--) {
                        if (map[i][j] == map[i - 1][j]) {
                            map[i][j] += map[i - 1][j];
                            score += map[i][j];
                            map[i - 1][j] = 0;
                            gridNeedInitFlg = true;
                            break;
                        }
                    }
                    int m = N - 1;
                    for (int i = N - 1; i > k; i--) {
                        if (map[i][j] == 0) continue;
                        map[m--][j] = map[i][j];
                    }
                    for (int i = m; i >=0; i--) {
                        map[i][j] = 0;
                    }
                }
                blankGridsHandle();
                if (gridNeedInitFlg) {
                    gridNeedInitFlg = false;
                    gridInit();
                }
                break;
            case 'k':
                for (int j = 0; j < N; j++) {
                    int k = 0;
                    for (int i = 0; i < N; i++) {
                        if (map[i][j] == 0) continue;
                        if (map[k][j] != map[i][j]) {
                            map[k][j] = map[i][j];
                            gridNeedInitFlg = true;
                        }
                        k++;
                    }
                    for (int i = 0; i < k - 1; i++) {
                        if (map[i][j] == map[i + 1][j]) {
                            map[i][j] += map[i + 1][j];
                            score += map[i][j];
                            map[i + 1][j] = 0;
                            gridNeedInitFlg = true;
                            break;
                        }
                    }
                    int m = 0;
                    for (int i = 0; i < k; i++) {
                        if (map[i][j] == 0) continue;
                        map[m++][j] = map[i][j];
                    }
                    for (int i = m; i < N; i++) {
                        map[i][j] = 0;
                    }
                }
                blankGridsHandle();
                if (gridNeedInitFlg) {
                    gridNeedInitFlg = false;
                    gridInit();
                }
                break;
            case 'l':
                for (int i = 0; i < N; i++) {
                    int k = N - 1;
                    for (int j = N - 1; j >= 0; j--) {
                        if (map[i][j] == 0) continue;
                        if (map[i][k] != map[i][j]) {
                            map[i][k] = map[i][j];
                            gridNeedInitFlg = true;
                        }
                        k--;
                    }
                    for (int j = N - 1; j > k + 1; j--) {
                        if (map[i][j] == map[i][j - 1]) {
                            map[i][j] += map[i][j - 1];
                            score += map[i][j];
                            map[i][j - 1] = 0;
                            gridNeedInitFlg = true;
                            break;
                        }
                    }
                    int m = N - 1;
                    for (int j = N - 1; j > k; j--) {
                        if (map[i][j] == 0) continue;
                        map[i][m--] = map[i][j];
                    }
                    for (int j = m; j >= 0; j--) {
                        map[i][j] = 0;
                    }
                }
                blankGridsHandle();
                if (gridNeedInitFlg) {
                    gridNeedInitFlg = false;
                    gridInit();
                }
                break;
            default:
                break;
        }

    }
    switch (c) {
        case 'R':
            gameInit();
            break;
        case 'Q':
            exit(0);
        default:
            break;
    }
}

//将空格点坐标顺序映射到blankGrids列表，以便于随机选取空格点生成2或4
void blankGridsHandle() {
    int bp = 0;
    for (int i = 0; i < TG; i++) {
        if (map[i / N][i % N] == 0) {
            blankGrids[bp++] = i;
        }
    }
    blankGrids[TG] = bp;
    for (int i = bp; i < TG; i++) {
        blankGrids[i] = 0;
    }
}

void randInit() {
    srand((unsigned)time(NULL));
}

int randPosInt(int bound) {
    if (bound < 0) return -1;
    
    return (int)(rand() * 1.0 / RAND_MAX * bound);
}

int rand2or4(int rate) {
    if (rate < 0) rate = 0;
    if (rate > 100) rate = 100;
    
    int res = (int)(rand() * 1.0 / RAND_MAX * 100);

    if (res < rate) return 2;
    return 4;
}


