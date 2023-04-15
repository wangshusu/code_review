#include "run.h"

/*
    Renew_Point_Data:更新cl_xy的数据 将触摸点变成 触摸数组值 (功能区专属)
                    例如: (750,465) ---> (1,5)
                    我们设置的分割线 分割线的左边 是会出现负数的
*/
void Renew_Point_Data1(Click_xy *cl_xy)
{
    cl_xy ->x = (cl.dorp_x - 632)/80;
    cl_xy ->y = (cl.dorp_y - 10)/78;
    if(cl_xy ->x > 2){cl_xy ->x = 2;}
    if(cl_xy ->y > 6){cl_xy ->y = 6;}
}

/*
    Renew_Point_Data2:返回触摸的功能位置
        竖状,从右到左              1    7
                                  2    8
*/
int Renew_Point_Data2(Click_xy *cl_xy)
{
    if(cl_xy ->x == 0 && cl_xy ->y == 0){return(1);}
    else if(cl_xy ->x == 0 && cl_xy ->y == 1){return(2);}
    else if(cl_xy ->x == 0 && cl_xy ->y == 2){return(3);}
    else if(cl_xy ->x == 0 && cl_xy ->y == 3){return(4);}
    else if(cl_xy ->x == 0 && cl_xy ->y == 4){return(5);}
    else if(cl_xy ->x == 0 && cl_xy ->y == 5){return(6);}
    else if(cl_xy ->x == 1 && cl_xy ->y == 0){return(7);}
    else if(cl_xy ->x == 1 && cl_xy ->y == 1){return(8);}
    else if(cl_xy ->x == 1 && cl_xy ->y == 2){return(9);}
    else if(cl_xy ->x == 1 && cl_xy ->y == 3){return(10);}
    else if(cl_xy ->x == 1 && cl_xy ->y == 4){return(11);}
    else if(cl_xy ->x == 1 && cl_xy ->y == 5){return(12);}
}


/*
    Show_Tool_picture:显示功能区 功能图片函数
*/
void Show_Tool_picture()
{
    display_bmp("./2.mplayerbmp/1.bmp", arr_x[0],arr_y[0], 0);//上一首
    display_bmp("./2.mplayerbmp/2.bmp", arr_x[0],arr_y[1], 0);//后退
    display_bmp("./2.mplayerbmp/3.bmp", arr_x[0],arr_y[2], 0);//暂停
    display_bmp("./2.mplayerbmp/4.bmp", arr_x[0],arr_y[3], 0);//音量加

    display_bmp("./2.mplayerbmp/6.bmp", arr_x[0],arr_y[5], 0);//音量加
    display_bmp("./2.mplayerbmp/7.bmp", arr_x[1],arr_y[0], 0);//下一首
    display_bmp("./2.mplayerbmp/8.bmp", arr_x[1],arr_y[1], 0);//快进
    display_bmp("./2.mplayerbmp/9.bmp", arr_x[1],arr_y[2], 0);//继续
    display_bmp("./2.mplayerbmp/10.bmp", arr_x[1],arr_y[3], 0);//继续

    display_bmp("./2.mplayerbmp/12.bmp", arr_x[1],arr_y[5], 0);//退出
}


/*
    StarMplayer:播放器启动准备函数
                负责准备播放器的环境
*/
void StarMplayer()
{
    lcd_open();
    Divide_rectangle();
    Show_Tool_picture();
}

/*
    Run_Mplayer:播放器启动函数
*/
void Run_Mplayer()
{
    
}


