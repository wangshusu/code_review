#ifndef __LANDMINE_H__
#define __LANDMINE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "lcd.h"
#include "bmp.h"

#define Hight 10
#define Width 10

//全局变量区
extern int V1;
extern int V2;
extern int LandMineNum;
extern int LandNullNum;
extern int FindLandNullNum;
extern char buf[16][11];
extern int Line[16][11];//1为雷
extern int Mine[16][11];//周围雷的个数

/*
    InitLindMine:初始化雷区
    @int DegDiff :雷的个数  (1 = 15   , 2 = 20   , 3 = 25  )
    返回值:无
*/
void InitLindMine(int DegDiff);

/*
    Count_LandMine_Num:计算周围雷个数函数
    返回值:无
*/
void Count_LandMine_Num(void);

/*
    FindAndShowNoLandMine:找到并且标记无雷区
    @int i,j:要找的点的坐标
*/
void FindAndShowNoLandMine(int x, int y);

/*
    @Print_Arry:测试输出数组
*/
void Print_Arry(void);

/*
    clect_bmp:雷区小方块显示
*/
void clect_bmp(int x,int y);

#endif