#include "landtouch.h"

/*
    touch_Open:触控文件打开函数
*/
void touch_Open(void)
{
    /* 先打开屏幕文件 --> 打开就要关闭 */
    fp_open = open(TouchPath,O_RDONLY | O_NONBLOCK);    /* 这里使用的是非阻塞 */
    if(fp_open == -1)
    {
        perror("open TouchPath failed");
        return;
    }
}

/*
    touch_Close:触控文件关闭函数
*/
void touch_Close(void)
{
    close(fp_open);
}


/*
    @GetTouchDrop_Silther():获取点的函数
        @Dorp * drp:全局变量 drp 的地址
        @int while_PassTime :读取多少 毫秒 (MAX = u32 /1000)
        返回值:
            (int) -1:出错
            (int)  0:成功
*/
int GetTouchDrop_Silther(Dorp *drp, int while_PassTime)
{
    /* 初始化 全部为 -1 */
    drp ->end_x = drp ->start_x = -1; //x轴
    drp ->end_y = drp ->start_y = -1; //y轴
    drp ->end_time = -1;
    drp ->start_time = -1;
    drp ->pass_time = -1;
    
    int flag_while = -1;
    int flag_x = -1;
    int flag_y = -1;
    struct input_event ev;      //保存 屏幕数据的
    struct timeval ti;          //获取时间特用
    struct timeval star_time;   //保存开始时间
    struct timeval end_time;    //结束结束时间
    
    /* -----开始读取----- */
    while (1)
    {
        if(-1 == flag_while)    /* 如果是第一次进入 */
        {
            flag_while = 1;     /* 标志修改 */
            gettimeofday(&ti, NULL);     /* 获取开始时间 */
            while_StartTime = while_EndTime = ti.tv_usec / 1000 +  ti.tv_sec * 1000;
        }

        //3.读取屏幕的数据<读取到 ev 中去>
        int fp_rd = read(fp_open,&ev,sizeof(ev));

        if(fp_rd == -1 && while_EndTime - while_StartTime <= while_PassTime)//如果没有读到文件
        {
            usleep(50);
            gettimeofday(&ti,NULL);     /* 获取时间 */
            while_EndTime = ti.tv_usec / 1000 +  ti.tv_sec * 1000;
            Show_Time();
            continue;       /* 一直读 直到读到文件里面有内容为止 */
        }
        else if(while_EndTime - while_StartTime > while_PassTime)
        {
            printf("read is down!\n");
            return -1;
        }   
        else if(fp_rd != -1)    /* 如果读到文件了,我们就开始判断 */
        {
                    
            if(ev.type = EV_ABS)//<1>这里是滑动判断
            {
                if(ev.code == ABS_X && ev.value > 0)    /* 先传x的坐标 >0 的我才接收 一般不会触摸到边上 */
                {
                    if(-1 == flag_x)//如果是刚开始的数据
                    {
                        gettimeofday(&star_time,NULL);//获取开始时间
                        drp ->start_time = star_time.tv_usec / 1000 +  star_time.tv_sec * 1000;
                        drp ->start_x = ev.value * 800 / 1024;
                        flag_x = 0;
                    }
                    else
                    {
                        drp ->end_x = ev.value * 800 / 1024; 
                    }
                }
                else if(ev.code == ABS_Y && ev.value > 0)
                {
                    if(-1 == flag_y)//如果是刚开始的数据
                    {
                        
                        drp ->start_y = ev.value * 480 /600;
                        flag_y = 0;
                    }
                    else
                    {
                        drp ->end_y = ev.value * 480 /600;
                    }
                }
                else if(ev.code == BTN_TOUCH && ev.value == 0)//如果离开屏幕
                {
                    gettimeofday(&end_time,NULL);//获取结束时间
                    drp ->end_time = end_time.tv_usec / 1000 +  end_time.tv_sec * 1000;
                    drp ->pass_time = drp ->end_time - drp ->start_time;
                    break;
                }
            }
        }
    

    }
    return 0;
}

/*
    Analyse_Drop_Dir(Dorp * drp):分析滑动方向 如果是 单击/双击 在全局变量 cl 中保存 (x,y)
        @Dorp * drp:struct Drop(结构体)
        返回值: 返回enum Move_Dir里面的方向判断
*/
enum Move_Dir Analyse_Drop_Dir(Dorp * drp)
{
    last_ms = 9999999;
    int dire = -1;  /* 保存结果 最后返回 */

    /* ----开始判断----- */
    if (drp ->end_x != -1 && 
       drp ->end_y != -1 &&
       drp ->start_x != -1 &&
       drp ->start_y != -1 )    /* 如果数据不完整不进入 */
    {
        if ((drp ->end_y - drp ->start_y) >= 0 
        && abs(drp ->end_x - drp ->start_x) <= abs(drp ->end_y - drp ->start_y))//下滑 方向下 y距离 > x距离 
        {
            printf("down\n");
            dire = DOWN;
        }
        else if ((drp ->end_y - drp ->start_y) < 0 
        && abs(drp ->end_x - drp ->start_x) <= abs(drp ->end_y - drp ->start_y))//上滑 方向上 y距离 > x距离 
        {
            printf("up\n");
            dire = UP;
        }
        else if ((drp ->end_x - drp ->start_x) > 0 
        && abs(drp ->end_x - drp ->start_x) > abs(drp ->end_y - drp ->start_y))//左滑  方向左 x距离 > y距离
        {
            printf("right\n");
            dire =  RIGHT;
        }
        else if ((drp ->end_x - drp ->start_x) <= 0 
        && abs(drp ->end_x - drp ->start_x) > abs(drp ->end_y - drp ->start_y))//右滑  方向右 x距离 > y距离
        {
            printf("lift\n");
            dire = LIFT;
        }
        else
        {
            printf("Analyse_Drop_Dir ERROR\n");
        }
    }
    else if(drp ->end_x == -1 &&
            drp ->end_y == -1 &&
            drp ->start_x != -1 &&
            drp ->start_y != -1 &&
            drp ->pass_time <= 300)     /* 单击判断 */
    {
        printf("CLICK\n");
        dire = CLICK;
        cl.dorp_x = drp ->start_x;
        cl.dorp_y = drp ->start_y;
        
        last_ms = drp ->end_time;
        GetTouchDrop_Silther(drp,500); /* 双击判断 */

        if(drp ->start_time - last_ms < 500)
        {
            printf("DBCLICK\n");
            dire = DBCLICK;
        }
    }
    else if(drp ->end_x == -1 && 
            drp ->end_y == -1 &&
            drp ->start_x != -1 &&
            drp ->start_y != -1 &&
            drp ->pass_time > 500)  /* 长按判断 */
    {
        cl.dorp_x = drp ->start_x;
        cl.dorp_y = drp ->start_y;
        printf("LongPress\n");
        dire = LongPress;
    }

    /* 用完了要清除 */
    drp ->end_x = -1;
    drp ->end_y = -1;
    drp ->start_x = -1;
    drp ->start_y = -1;
    drp ->pass_time = -1;

    printf("\n");
    return dire;

}