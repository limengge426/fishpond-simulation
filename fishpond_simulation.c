//鱼塘仿真软件:鱼塘中的小鱼(用 ><(('>表示)能够从左向右游(五条)。鱼饵(用*表示)一边下沉一边向左移动(五个)。另外会有一个鱼钩(用J表示)从鱼塘的底部被向上拉。
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define FISH_COUNT 5
#define BAIT_COUNT 5
#define POND_WIDTH 32
#define POND_HEIGHT 8
#define WATER '~'
#define FISH "><(('>"
#define BAIT '*'
#define FISHHOOK 'J'
#define max(a,b) ((a) >= (b) ? (a) : (b))
#define min(a,b) ((a) >= (b) ? (b) : (a))

void generateRandomFishPositions(int fish_positions[][2]);
void generateRandomBaitPositions(int bait_positions[][2]);
void generateRandomFishhookPositions(int fishhook[2]);
void fillTank(char tank[][POND_WIDTH]);
void placeFishInTank(char tank[][POND_WIDTH], int fish_positions[][2]);
void placeBaitInTank(char tank[][POND_WIDTH], int bait_positions[][2]);
void placeFishhook(char tank[][POND_WIDTH], int fishhook[2]);
void renderTank(char tank[][POND_WIDTH]);
void moveAllFish(int fish_positions[][2]);
void moveAllBait(int bait_positions[][2]);
void moveFishhook(int fishhook[2]);
int check(int fish_positions[][2], int bait_positions[][2], int fishhook[2]);


int check(int fish_positions[][2], int bait_positions[][2], int fishhook[2]) {
    //判断是否上钩：如果鱼的位置数组和鱼钩的位置数组相等
    int i, j;
    for (i = 0; i < FISH_COUNT; i++) {
        if (fish_positions[i][0] == fishhook[0] && fish_positions[i][1] == fishhook[1]) {
            printf("lose\n");   //输掉游戏
            return 1;   //返回1
        }
    }
    //判断是否中饵：如果鱼的位置数组和鱼饵的位置数组相等
    for (i = 0; i < FISH_COUNT; i++) {
        for (j = 0; j < BAIT_COUNT; j++) {
            if (fish_positions[i][0] == bait_positions[j][0] && fish_positions[i][1] == bait_positions[j][1]) {
                printf("win\n");    //游戏胜利
                return 1;   //返回1
            }
        }
    }
    return 0;   //鱼未上钩或中饵，返回0
}

void moveFishhook(int fishhook[2]) {
    fishhook[0] = max(fishhook[0] - 1, 0);  //将给定的鱼钩数组中的第一个元素减1，同时保证不小于0。
}

void placeFishhook(char tank[][POND_WIDTH], int fishhook[2]) {
    tank[fishhook[0]][fishhook[1]] = FISHHOOK;  //将鱼钩放置在指定的位置
}

void generateRandomFishhookPositions(int fishhook[2]) {
    fishhook[0] = POND_HEIGHT - 1;  //将鱼钩的第一个位置设为池塘的高度减1
    fishhook[1] = rand() % POND_WIDTH;  //将鱼钩的第二个位置设为池塘宽度的随机值
}

void moveAllBait(int bait_positions[][2]) {
    int i;
    for (i = 0; i < BAIT_COUNT; i++) {
        //获取鱼饵的位置
        bait_positions[i][0] = min(bait_positions[i][0] + 1, POND_HEIGHT - 1);
        //将鱼饵放在tank数组的相应位置
        bait_positions[i][1] = (bait_positions[i][1] + 1) % POND_WIDTH;
    }
}

void placeBaitInTank(char tank[][POND_WIDTH], int bait_positions[][2]) {
    int i;
    for (i = 0; i < BAIT_COUNT; i++) {
        int row = bait_positions[i][0], col = bait_positions[i][1]; //获取鱼饵的位置
        tank[row][col] = BAIT;  //将鱼饵放在tank数组的相应位置
    }
}

void generateRandomBaitPositions(int bait_positions[][2]) {
    int i, j, confirm = 0;
    for (i = 0; i < BAIT_COUNT; i++) {
        int col;
        while (1) {
            col = rand() % POND_WIDTH;
            //鱼饵位置不能重复, flag=0表示未重复，1表示重复
            int flag = 0;
            for (j = 0; j < confirm; j++) {
                if (bait_positions[j][1] == col) {  //如果当前生成的列位置与已确定的列位置重复，则重新生成
                    flag = 1;
                    break;
                }
            }
            if (flag == 0)  //表示当前生成的列位置是唯一的，退出循环
                break;
        }
        bait_positions[confirm++][1] = col;
    }
}

void moveAllFish(int fish_positions[][2]) {
    int i;
    for (i = 0; i < FISH_COUNT; i++) {
        fish_positions[i][1] = (fish_positions[i][1] + 1) % POND_WIDTH;
        //用模数运算符确保新的坐标值不会超过池塘的宽度
    }
}

void renderTank(char tank[][POND_WIDTH]) {
    int i, j;
    for (i = 0; i < POND_HEIGHT; i++) {
        for (j = 0; j < POND_WIDTH; j++) {
            printf("%c", tank[i][j]);   //打印鱼塘tank中的元素
        }
        printf("\n");
    }
}

void placeFishInTank(char tank[][POND_WIDTH], int fish_positions[][2]) {
    int i, j, len = strlen(FISH);     //获取小鱼字符串的长度
    for (i = 0; i < FISH_COUNT; i++) {
        int row = fish_positions[i][0], col = fish_positions[i][1];
        for (j = 0; j < len; j++) {
            //确保小鱼的字符被正确地放入池塘的边界内
            tank[row][(col + POND_WIDTH - j) % POND_WIDTH] = FISH[len - j - 1];
        }
    }
}

void generateRandomFishPositions(int fish_positions[][2]) {
    int i, j;
    //已确定的鱼数量
    int confirm = 0;
    for (i = 0; i < FISH_COUNT; i++) {
        int rand_row, rand_col;   //随机生成鱼的位置
        while (1) {
            rand_row = rand() % POND_HEIGHT;    
            rand_col = rand() % POND_WIDTH;
            //判断位置是否与已确定的鱼位置重复（列差的绝对值小于6，说明重复）
            int flag = 0;
            for (j = 0; j < confirm; j++) {
                int len = strlen(FISH);
                if (fish_positions[j][0] == rand_row && ((rand_col + POND_WIDTH - fish_positions[j][1]) % POND_WIDTH) < len) {
                    flag = 1;
                    break;
                }
            }
            if (flag == 0)  //如果新生成的鱼位置没有重复，则退出循环
                break;
        }
        //将新生成的鱼的位置存入数组
        fish_positions[confirm][0] = rand_row;
        fish_positions[confirm][1] = rand_col;
        confirm++;
    }
}

void fillTank(char tank[][POND_WIDTH]) {
    int i, j;
    for (i = 0; i < POND_HEIGHT; i++) {
        for (j = 0; j < POND_WIDTH; j++) {
            tank[i][j] = WATER;     //循环嵌套，用"~"填充数组
        }
    }
}

int main()
{
    srand((unsigned int)time((NULL)));
    int fish_positions[FISH_COUNT][2] = { 0 };  //鱼的位置
    int bait_positions[BAIT_COUNT][2] = { 0 };  //鱼饵的位置
    int fishhook[2] = { 0 };    //鱼钩位置
    char tank[POND_HEIGHT][POND_WIDTH];     //鱼塘
    generateRandomFishPositions(fish_positions);    //生成鱼随机位置
    generateRandomBaitPositions(bait_positions);    //生成鱼饵随机位置
    generateRandomFishhookPositions(fishhook);      //生成鱼钩随机位置
    while (1)//无限循环
    {
        fillTank(tank);     //将鱼塘充满水
        placeFishInTank(tank, fish_positions);  //放置鱼
        placeBaitInTank(tank, bait_positions); //放置鱼饵
        placeFishhook(tank, fishhook);  //放置鱼钩
        renderTank(tank);   //打印鱼塘
        //检查判断是否上钩或中饵
        if (check(fish_positions, bait_positions, fishhook)) {
            return 0;
        }
        moveAllFish(fish_positions);    //所有鱼前进一个字符
        moveAllBait(bait_positions);    //所有鱼饵向右下方移动一个字符
        moveFishhook(fishhook);     //鱼钩向上移动
        usleep(500000);
        system("clear");
    }
    return 0;
}
