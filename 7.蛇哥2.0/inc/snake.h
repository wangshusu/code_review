
#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <stdio.h>

#include "bmp.h"
#include "touch.h"


/************* 宏定义区 *****************/
#define height 20  //格子的高度
#define width 20   //格子的宽度
#define X_Max 40   //x轴最多的数量
#define Y_Max 24   //y轴最多的数量
#define COL  35
#define ROW  24

#define KONG 0 //标记空（什么也没有）
#define WALL 1 //标记墙
#define FOOD 2 //标记食物
#define HEAD 3 //标记蛇头
#define BODY 4 //标记蛇身

#define AGAIN 32 //暂停
#define SPACE 32 //暂停
#define ESC 27 //退出

/////******************* 下面的函数蛇哥专用 ****************////

//蛇的数据:

//蛇头
struct Snake
{
	int len; //记录蛇身长度
	int x; //蛇头横坐标
	int y; //蛇头纵坐标
}snake;

//蛇身
struct Body
{
	int x; //蛇身横坐标
	int y; //蛇身纵坐标
}body[COL*ROW]; //开辟足以存储蛇身的结构体数组

//初始化屏幕 并将位置存入数组
void Init_Screen(void);

//初始化蛇
//蛇的长度 为 2
void Init_Snake();

//随机生成食物
void RandFood();

//打印蛇与覆盖蛇
//传入 1 打印蛇
//传入 0 覆盖蛇
void DrawSnake(int flag);

//移动蛇
void MoveSnake(int x, int y);

//判断得分与结束
int JudgeFunc(int x, int y);

void Print(void);

#endif