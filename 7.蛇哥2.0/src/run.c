#include "run.h"

/************* 全局变量区 ***********/
extern int arr_x[X_Max];//每个方块的x位置 右上角
extern int arr_y[Y_Max];//每个方块的y位置 右上角
extern int movedir;//滑动方向 默认右滑
pthread_t pthid1;//创建线程号
int ret;
int cmd = 0;

struct timeval while_time;//持续的时间
struct timeval start_time;//持续的时间
struct timeval end_time;//持续的时间

#define snake_speed 500

//线程
void *pthid_callback(void *agr)
{
    while(1)
    {
    ret = GetTouchDrop_Silther(&drp,15000);//这里获取超长的时间   足够蛇哥滑动了

    movedir = Analyse_Drop_Dir(&drp);
    //如果获取到点数进行了分析  //并且 分析结果是 滑动 和 单击的一种
    if(ret != -1 &&  (movedir == RIGHT\
      ||  movedir == LIFT\
      ||  movedir == UP\
      ||  movedir == DOWN))
    {
        //那么我们才将这个数值返回给我们的屏幕
        cmd =  movedir;
    }
    else
    {
        movedir =  cmd;//否则还是原来的
    }
    
printf("movedir %d\n",movedir);
usleep(100000);
    }
}
//写一个函数  开始之后 计算秒数
int  get_time()
{
    int while_time;
    struct timeval ti;

    gettimeofday(&ti,NULL);
    while_time = (ti.tv_sec - start_time.tv_sec) *1000 + (ti.tv_usec - start_time.tv_usec) / 1000;

    if(while_time >= snake_speed)
    {
        start_time.tv_usec = start_time.tv_usec + snake_speed*1000;
        if(start_time.tv_usec > 1000000)
        {
            start_time.tv_sec++;
            start_time.tv_usec -= 1000000;
        }
        return 1;//不需要更新蛇的信息[-=0]
    }
    return 0;//需要更新蛇的信息
}


//启动前准备
void Star_En(void)
{
    int ret;
    srand((unsigned int)time(NULL)); //这里是拿根据时间改变的数
    lcd_open();
    Star_touch();
    
    Init_Screen();
    Init_Snake();
    DrawSnake(1);
    RandFood();
    pthread_create(&pthid1,NULL,pthid_callback,NULL);//线程的创建
    
}

//结束收尾
void End_En(void)
{
    lcd_close();
    End_touch();
}

//上
int Snake_Up(void)
{
    int back = JudgeFunc( 0, -1 );
            MoveSnake( 0, -1 );
    return back;
}

//下
int Snake_Down(void)
{
    int back = JudgeFunc( 0, +1 );
            MoveSnake( 0, +1 );
    return back;
}

//左
int Snake_Lift(void)
{
    int back = JudgeFunc( -1, 0 );
            MoveSnake( -1, 0 );
    return back;
}

//右
int Snake_Right(void)
{
    int back = JudgeFunc( 1, 0 );
            MoveSnake( 1, 0 );
    return back;
}




//蛇哥跑起来了 蛇哥默认右滑
void Run_Snake()
{
    Star_En();
    int game = 1;

    while(game)
    {
        //Print();//上帝视角
        //cmd = movedir;//通过cmd记录之前的移动方向 // 默认右移
        printf("默认的方向是: %d %d \n",cmd ,movedir);
        // if(get_time() == 1)
        // {
            cmd = movedir;
            printf("cmd = %d\n",cmd);
            
            switch (cmd)
            {
            case 0:// UP  上
                game = Snake_Up( );
                /* code */
                break;
            case 1://DOWN 下
                game = Snake_Down( );
                /* code */
                break;
            case 2://LIFT 左
                game = Snake_Lift( );
                /* code */
                break;
            case 3://RIGHT 右
                game = Snake_Right();
                /* code */
                break;
            case LongPress:
                /* code */
                break;
            
            default:
                break;
            }
            usleep(100000);//单位是 微妙 = 千分之一毫秒  //这里暂停 500 毫秒 == 0.5 秒
        // }
    }
    End_En();
    return;
}