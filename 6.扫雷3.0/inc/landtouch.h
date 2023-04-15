#ifndef __TOUCH_H__
#define __TOUCH_H__

#include <stdio.h>

#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "landrun.h"

#define TouchPath "/dev/input/event0"   /* 屏幕文件所在地 */

enum Move_Dir//描述屏幕的操作的枚举
{
    RIGHT,//右滑
    LIFT,//左滑
    UP,//上滑
    DOWN,//下滑
    DBCLICK,//双击
    CLICK,//单击
    LongPress,//长按
    DIR_ERROR//错误
};

/* 用来保存点的数据(x,y) */
typedef struct Dorp
{
    int start_x;
    int start_y;

    int end_x;
    int end_y;

    unsigned int start_time;    /* 开始的时间 */
    unsigned int end_time;      /* 结束的时间 */
    unsigned int pass_time;     /* 消耗的时间 */

}Dorp;

typedef struct Dorp_Click//用来保存 单击双击 点的数据(x,y)
{
    int dorp_x;
    int dorp_y;

}Dorp_Click;

/******************************
            全局变量区
 ******************************/
int while_StartTime;    //while循环开始时间
int while_EndTime;      //while结束的时间
Dorp drp;               //屏幕数据结构体
Dorp_Click cl;          //单击双击点的坐标
int last_ms;            //记录最后一次单击时的毫秒数
int fp_open;     //屏幕文件描述符


/*
    touch_Open:触控文件打开函数
*/
void touch_Open(void);

/*
    touch_Close:触控文件关闭函数
*/
void touch_Close(void);

/*
    @GetTouchDrop_Silther():获取点的函数
    @Dorp * drp:全局变量 drp 的地址
    @int while_PassTime :读取多少 毫秒 (MAX = u32 /1000)
    返回值:
        (int) -1:出错
        (int)  0:成功
*/
int GetTouchDrop_Silther(Dorp * drp , int while_PassTime);//适用于滑动分析

/*
    @Analyse_Drop_Dir(Dorp * drp):分析滑动方向 如果是 单击/双击 在全局变量 cl 中保存 (x,y)
    @Dorp * drp:struct Drop(结构体)
    返回值: 
        返回enum Move_Dir里面的方向判断
*/
enum Move_Dir Analyse_Drop_Dir(Dorp * drp);                        

/*
    @int gettimeofday(struct timeval*tv, struct timezone *tz);
    @struct timeval*tv----->tv是保存获取时间结果的结构体
    @struct timezone *tz--->用于保存时区结果（若不使用则传入NULL即可）。
    返回值:未知
    struct timeval {  
　　long tv_sec; // 秒数  
    long tv_usec; //微秒数  
    }  
*/

#endif