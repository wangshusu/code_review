
#ifndef __RUN_H__
#define __RUN_H__

#include "lcd.h"
#include "landmine.h"
#include "landtouch.h"
#include "bmp.h"
#include "displaychar.h"
#include "text.h"

#define GameOver0 "游戏失败!"
#define GameOver2 "闯关成功!"
#define GameOver1 "按任意键开始下一盘..."

/****************** 部分函数注意事项说明 *******************
1.Renew_Point_Data(Click_xy *cl_xy)
  这里更新之后结构体里面的(x,y)存储的是触摸点对应的 雷区分割 的位置
  Max(x,y) = (10,15)  但是这是雷区的最大值
  这个函数同样承载着返回功能区的功能 如果触摸的是功能区对应的位置
  这里返回的值将是:
                重开:(77,77)
                退出:(88,88)
**********************************************************/

typedef struct Click_xy//用来保存 单击双击 点的数据(x,y)
{
    int x;
    int y;

}Click_xy;

/*
    Run_Start:雷区启动前环境准备函数
*/
void Run_Start(void);

/*
    Run_End:游戏退出收尾工作
*/
void Run_End(void);

/*
    Failure_Game:点到雷 游戏失败函数
*/
void Failure_Game(Click_xy cl_xy);

/*
    Success_Game:闯关成功函数
*/
void Success_Game(void);

/*
    Open_Cell_CLICK:单击打开格子
    @Click_xy cl_xy:结构体 (已经是格子点数)
    @返回值:
        如果返回 0  --->那就是正常打开
        如果返回 -1 --->游戏结束,需要开启下一盘
*/
int Open_Cell_CLICK(Click_xy cl_xy);

/*
    Open_Cell_LongPress:长按执行插旗子函数
    @Click_xy cl_xy:结构体 (已经是格子点数)
*/
void Open_Cell_LongPress(Click_xy cl_xy);

/*  
    Renew_Point_Data:将触摸点数据 转换为 格子点数
    @Click_xy cl_xy:结构体 (仍是触摸点数)
*/
void Renew_Point_Data(Click_xy *cl_xy);

/*
    Open_Cell_DBCLICK:双击快速 打开
    @Click_xy cl_xy:结构体 (已经是格子点数)
    @返回值:
        如果返回 0  --->那就是正常打开
        如果返回 -1 --->游戏结束,需要开启下一盘
*/
int Open_Cell_DBCLICK(Click_xy cl_xy);

/*
    Show_FindLandNullNum:找出的空白区的个数
*/
void Show_FindLandNullNum(int cmd);

/*
    Run_LandMine:雷区跑起来啦!
*/
int Run_LandMine(void);

/*
    Show_Blank:显示未打开雷区函数
*/
void Show_Blank(void);

/*
    Show_Time:时间功能区显示函数
*/
void Show_Time(void);

/*
    Show_Face:笑脸显示函数
        @int num:选择显示哪个笑脸
*/
void Show_Face(int num);

/*
    Selext_Num:选择显示数字图片
        @int num:哪个数字
        @int x  :x轴的位置
        @int y  :y轴的位置
*/
void Selext_Num(int num,int x,int y);
#endif