#ifndef __RUNLANDMINE_H__
#define __RUNLANDMINE_H__

#include "touch.h"
#include "bmp.h"
#include "lcd.h"
#include "getpath.h"
#include "showphoto.h"

typedef struct Click_xy//用来保存 单击双击 点的数据(x,y)
{
    int x;
    int y;

}Click_xy;


/*
    Renew_Point_Data:更新cl_xy的数据 将触摸点变成 触摸数组值 (功能区专属)
                    例如: (750,465) ---> (1,5)
                    我们设置的分割线 分割线的左边 是会出现负数的
*/
void Renew_Point_Data1(Click_xy *cl_xy);

/*
    Renew_Point_Data2:返回触摸的功能位置
        竖状,从右到左          7    1
                              8    2
    返回值:(int) 如上所示      ↑
*/
int Renew_Point_Data2(Click_xy *cl_xy);

/*
    Show_Tool_picture:显示功能区 功能图片函数
*/
void Show_Tool_picture();

/*
    StarMplayer:播放器启动准备函数
                负责准备播放器的环境
*/
void StarMplayer();

#endif