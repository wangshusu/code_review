#ifndef __RUN_H__
#define __RUN_H__

#include <unistd.h>
#include <pthread.h> //线程头文件
#include <sys/time.h>
#include "snake.h"


//启动前准备
void Star_En(void);
//结束收尾
void End_En(void);

//上
int Snake_Up(void);
//下
int Snake_Down(void);
//左
int Snake_Lift(void);
//右
int Snake_Right(void);

//蛇哥跑起来了
void Run_Snake();

#endif