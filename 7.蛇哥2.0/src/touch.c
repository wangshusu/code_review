#include "touch.h"

/************** 全局变量区 **********/
int fp_open;//屏幕文件指针
struct Dorp drp;
int movedir = 0;

//touch 环境准备函数
void Star_touch(void)
{
    //1.先打开屏幕文件 --> 打开就要关闭
    fp_open = open(TouchPath,O_RDONLY | O_NONBLOCK);//这里使用的是非阻塞
    if(fp_open == -1)
    {
        perror("open TouchPath failed");
        close(fp_open);
        return;//出错返回NULL值
    }
}

//touch 资源回收
void End_touch(void)
{
    close(fp_open);//没关掉就让系统给我们关吧
}



/*
    @GetTouchDrop_Silther():获取点的函数
    @Dorp * drp:全局变量 drp 的地址
    @int while_PassTime :读取多少 毫秒 (MAX = u32 /1000)
    返回值:
        (int) -1:出错
        (int)  0:成功
*/
int GetTouchDrop_Silther(Dorp * drp , int while_PassTime)
{
    
    //2.初始化
    //memset(drp,0,sizeof(Dorp));
    drp ->end_x = drp ->start_x = -1; //x轴
    drp ->end_y = drp ->start_y = -1; //y轴
    drp ->end_time = -1;
    drp ->start_time = -1;
    drp ->pass_time = -1;
    
    int flag_while = -1;
    int flag_x = -1; //判断是否是 start
    int flag_y = -1;
    struct input_event ev; //这个结构体是用来保存 屏幕数据的
    struct timeval while_time;//用来保存 循环开始的时间
    struct timeval star_time; //= calloc(1,sizeof(struct timeval));//这个结构体是用来保存 时间数据的
    struct timeval end_time; //= calloc(1,sizeof(struct timeval));//结束
    int while_EndTime = 0;

    //这里使用的 timeval 结构体中
    //tv_sec 代表多少秒
    //tv_usec 代表多少微秒 1000000 微秒 = 1秒
    //这里传入的 while_PassTime 的单位 是毫秒
    
    while (1)
    {
        if(-1 == flag_while)//这里只在进入函数的时候获取一次 保证我们的 while_StartTime 获取成功 有且获取一次
        {
            flag_while = 1;//标志修改

            gettimeofday(&while_time,NULL);//获取开始时间
            while_StartTime = while_EndTime = while_time.tv_usec / 1000 +  while_time.tv_sec * 1000;
        }

        //3.读取屏幕的数据<读取到 ev 中去> --- 这里使用的是非阻塞的方式
        int fp_rd = read(fp_open,&ev,sizeof(ev));
        if(fp_rd == -1 && while_EndTime - while_StartTime <= while_PassTime)//在规定的时间内持续的读文件
        {
            usleep(50);//单位是 微妙 = 千分之一毫秒  //这里暂停 2 毫秒

            gettimeofday(&while_time,NULL);//获取时间
            while_EndTime = while_time.tv_usec / 1000 +  while_time.tv_sec * 1000;//更新 持续的时间

            continue;//这里就是一直读 知道读到文件里面有内容为止
        }
        else if(while_EndTime - while_StartTime > while_PassTime)//如果在规定的时间内没有读到文件
        {
            //printf("read is down!\n");
            //close(fp_open);//----------------------------------------< 这里线程的话 我们就不关闭了  最后关闭
            return -1;
        }   
        else if(fp_rd != -1)
        {
                    //4.如果读到文件了,我们就开始判断
            if(ev.type = EV_ABS)//<1>这里是滑动判断
            {
                if(ev.code == ABS_X && ev.value > 0)//先传x的坐标 >0 的我才接收 一般不会触摸到边上
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
                    //printf("start_time = %d\n",drp ->start_time);
                    //printf("end_time   = %d\n",drp ->end_time);
                    //printf("pass_time  = %d\n",drp ->pass_time);
                    break;
                }
            }
        }
    

    }

    //close(fp_open);
    return 0;
}

/*
    @Analyse_Drop_Dir(Dorp * drp):分析滑动方向 如果是 单击/双击 在全局变量 cl 中保存 (x,y)
    @Dorp * drp:struct Drop(结构体)
    返回值: 
        返回enum Move_Dir里面的方向判断
*/
enum Move_Dir Analyse_Drop_Dir(Dorp * drp)
{
    int dire = -1;//enum 保存结果 最后返回
    if(drp ->end_x != -1 && 
       drp ->end_y != -1 &&
       drp ->start_x != -1 &&
       drp ->start_y != -1 )//如果数据不完整不进入<判断滑动>
    {
        if((drp ->end_y - drp ->start_y) >= 0 && abs(drp ->end_x - drp ->start_x) <= abs(drp ->end_y - drp ->start_y))//下滑 方向下 y距离 > x距离 
        {
            printf("--------------------------------------down\n");
            dire = DOWN;
        }
        else if((drp ->end_y - drp ->start_y) < 0 && abs(drp ->end_x - drp ->start_x) <= abs(drp ->end_y - drp ->start_y))//上滑 方向上 y距离 > x距离 
        {
            printf("--------------------------------------up\n");
            dire = UP;
        }
        else if((drp ->end_x - drp ->start_x) > 0 && abs(drp ->end_x - drp ->start_x) > abs(drp ->end_y - drp ->start_y))//左滑  方向左 x距离 > y距离
        {
            printf("--------------------------------------right\n");
            dire =  RIGHT;
        }
        else if((drp ->end_x - drp ->start_x) <= 0 && abs(drp ->end_x - drp ->start_x) > abs(drp ->end_y - drp ->start_y))//右滑  方向右 x距离 > y距离
        {
            printf("--------------------------------------lift\n");
            dire = LIFT;
        }
        else
        {
            printf("--------------------------------------Analyse_Drop_Dir ERROR\n");
        }
    }
    else if(drp ->end_x == -1 && 
            drp ->end_y == -1 &&
            drp ->start_x != -1 &&
            drp ->start_y != -1 &&
            drp ->pass_time <= 500)
    {
        printf("CLICK\n");
        dire = CLICK;
        cl.dorp_x = drp ->start_x;
        cl.dorp_y = drp ->start_y;

        last_ms = drp ->end_time;
        // GetTouchDrop_Silther(drp,800);//第二次获取判断

        // if(drp ->start_time - last_ms < 800)
        // {
        //     printf("DBCLICK\n");
        //     dire = DBCLICK;
        // }
    }
    // else if(drp ->end_x == -1 && 
    //         drp ->end_y == -1 &&
    //         drp ->start_x != -1 &&
    //         drp ->start_y != -1 &&
    //         drp ->pass_time > 500)
    // {
    //     cl.dorp_x = drp ->start_x;
    //     cl.dorp_y = drp ->start_y;
    //     printf("LongPress\n");
    //     dire = LongPress;

    // }
    
    //如果用完了 那么我们就将这个分析数据的蓝本重置
    drp ->end_x = -1;
    drp ->end_y = -1;
    drp ->start_x = -1;
    drp ->start_y = -1;


    return dire;

}


//判断传入的点 是否在这个 jr 的区域中
int JudgeRegion(Judge_Region jr ,Dorp_Click dc)
{
    //如果判断 (x,y) 属于 这个区域
    if(dc.dorp_x < jr.end_x &&
       dc.dorp_y < jr.end_y &&
       dc.dorp_y > jr.start_y &&
       dc.dorp_y > jr.start_y )return 1;
    else return -1;
}