#include "landrun.h"

/*************** 外部声明区 **************/
extern unsigned char SchName1[4][40*35/8];
extern int LandMineNum;
extern int LandNullNum;
extern int FindLandNullNum;

/*************** 全局变量区 **************/
struct timeval sta_tv;//游戏开始的时候的时间
struct timeval cur_tv;//游戏当前的时间
Click_xy cl_xy;//更新专用
int game_over;

/*
    Run_Start:雷区启动前环境准备函数
*/
void Run_Start(void)
{
    game_over = 1;
    touch_Open();
    srand((unsigned int)time(NULL)); //这里是拿根据时间改变的数
    lcd_open();
    
    /********* 雷区分割 **********/
    Divide_rectangle();//区域分隔
    Show_Blank();//显示Blank区域格子

    /********* 功能区分割 **********/
    Show_FindLandNullNum(0);//
    lcd_draw_word(724,433,32,40,SchName1[0],0xffffff);//功能区 "退"
    lcd_draw_word(761,433,32,40,SchName1[1],0xffffff);//功能区 "出"
    lcd_draw_word(724,388,32,40,SchName1[2],0xffffff);//功能区 "重"
    lcd_draw_word(761,388,32,40,SchName1[3],0xffffff);//功能区 "开"
    
    Show_Face(1);//笑脸哥

    gettimeofday(&sta_tv,NULL);//获取开始时间
    //printf("当前秒数为 : %d\n\n",sta_tv.tv_sec);
}

/*
    Run_End:游戏退出收尾工作
*/
void Run_End(void)
{
    lcd_close();
    touch_Close();
    return;
}

/*
    Failure_Game:点到雷 游戏失败函数
*/
void Failure_Game(Click_xy cl_xy)
{
    Show_Face(3);//笑脸哥
    printf("game over!\n");
    for(int i = 0 ; i < 3 ; i ++)
    {
        display_bmp("./1.landminebmp/mine1.bmp",arr_x[cl_xy.x],arr_y[cl_xy.y],0);
        GetTouchDrop_Silther(&drp,300);
        display_bmp("./1.landminebmp/blank.bmp",arr_x[cl_xy.x],arr_y[cl_xy.y],0);
        GetTouchDrop_Silther(&drp,300);
    }

    display_bmp("./1.landminebmp/mine1.bmp",arr_x[cl_xy.x],arr_y[cl_xy.y],0);
    for(int i = 0 ; i < V1 ; i++)
    {
        for(int j = 0 ; j < V2 ; j++)
        {
            if(i == cl_xy.x && j == cl_xy.y)
            {
                //你猜猜这个是什么?
            }
            else if(1 == Line[i][j] )
            {
                display_bmp("./1.landminebmp/mine2.bmp",arr_x[i],arr_y[j],0);
            }
            
            //usleep(300);
        }
    }

    lcd_draw_rectangle(arr_x[4], arr_y[4],334,120,0xffffff);
    Show_Str_Mid(arr_x[2],arr_y[4],GameOver0,48,400,0x000000);
    GetTouchDrop_Silther(&drp,600);
    Show_Str_Mid(arr_x[5],arr_y[6],GameOver1,24,300,0x000000); 

    // 这里显示 按任意键继续
    GetTouchDrop_Silther(&drp,20000);

}

/*
    Success_Game:闯关成功函数
*/
void Success_Game(void)
{
    Show_Face(4);//笑脸哥
    printf("闯关成功!\n");
    //显示所有的雷的地点
    for(int i = 0 ; i < V1 ; i++)
    {
        for(int j = 0 ; j < V2 ; j++)
        {
            if(i == cl_xy.x && j == cl_xy.y)
            {
                //你猜猜这个是什么?
            }
            else if(1 == Line[i][j] )
            {
                display_bmp("./1.landminebmp/mine2.bmp",arr_x[i],arr_y[j],0);
            }
            
            //usleep(300);
        }
    }
    lcd_draw_rectangle(arr_x[4], arr_y[4],334,120,0xffffff);
    Show_Str_Mid(arr_x[2],arr_y[4],GameOver2,48,400,0x000000);
    GetTouchDrop_Silther(&drp,600);
    Show_Str_Mid(arr_x[5],arr_y[6],GameOver1,24,300,0x000000); 

    GetTouchDrop_Silther(&drp,20000);
    return;
}

/*
    Open_Cell_CLICK:单击打开格子
    @Click_xy cl_xy:结构体 (已经是格子点数)
    @返回值:
        如果返回 0  --->那就是正常打开
        如果返回 -1 --->游戏结束,需要开启下一盘
*/
int Open_Cell_CLICK(Click_xy cl_xy)
{
    //如果这个点已经插上了旗子,那么我就不准你在这里打开了
    if('1' == buf[cl_xy.x][cl_xy.y])
    {
        printf("这里已经插上了旗子!!\n");
        return 0;
    }
    else if(0 == Line[cl_xy.x][cl_xy.y])//开cell成功
    {
        FindAndShowNoLandMine(cl_xy.x,cl_xy.y);
        //FindLandNullNum = Judge_LandNum();
        Show_FindLandNullNum(1);
        printf("FindLandNullNum = %d\n",FindLandNullNum);
    }
    else if(1 == Line[cl_xy.x][cl_xy.y])//闯关失败
    {
        Failure_Game(cl_xy);
        return -1;
    }
    if(FindLandNullNum == LandNullNum)//闯关成功
    {
       Success_Game();
       return -1;
    }
    return 0;
}

/*
    Open_Cell_LongPress:长按执行插旗子函数
    @Click_xy cl_xy:结构体 (已经是格子点数)
*/
void Open_Cell_LongPress(Click_xy cl_xy)
{
    printf("已经进入了长按范围!\n");
    //如果是 2 的就不显示
    // printf("长按的格子是 (%d,%d)\n",cl_xy.x,cl_xy.y);
    // printf("line的格子是 %d\n",Line[cl_xy.x][cl_xy.y]);
    if(2 != Line[cl_xy.x][cl_xy.y])
    {
        printf("该点没有显示过(%d,%d)!\n",cl_xy.x,cl_xy.y);
        if('0' == buf[cl_xy.x][cl_xy.y])//如果还没有显示过 那就显示
        {
            buf[cl_xy.x][cl_xy.y] = '1';
            display_bmp("./1.landminebmp/flag.bmp",arr_x[cl_xy.x],arr_y[cl_xy.y],0);
        }
        else if('1' == buf[cl_xy.x][cl_xy.y])
        {
            buf[cl_xy.x][cl_xy.y] = '0';
            display_bmp("./1.landminebmp/blank.bmp",arr_x[cl_xy.x],arr_y[cl_xy.y],0);
        }
        
    }
}

/*
    Open_Cell_DBCLICK:双击快速 打开
    @Click_xy cl_xy:结构体 (已经是格子点数)
    @返回值:
        如果返回 0  --->那就是正常打开
        如果返回 -1 --->游戏结束,需要开启下一盘
*/
int Open_Cell_DBCLICK(Click_xy cl_xy)
{
    //1.现在就要先判断  你双击的这个点 周围的旗子数 == Mine[x][y]
    int sum_line = 0;
    printf("双击的点数是:(%d,%d)\n",cl_xy.x,cl_xy.y);
    
    int i = cl_xy.x;
    int j = cl_xy.y;
    //如果这个点的周围有棋子  那么这个点就 +1  Line Mine
    if(j - 1 >= 0 && buf[i][j - 1] == '1'){sum_line++;}//上
    if(j + 1 < V2 && buf[i][j + 1] == '1'){sum_line++;}//下
    if(i - 1 >= 0 && buf[i - 1][j] == '1'){sum_line++;}//左
    if(i + 1 < V1 && buf[i + 1][j] == '1'){sum_line++;}//右
    if(j - 1 >= 0 && i - 1 >= 0 && buf[i - 1][j - 1] == '1'){sum_line++;}//上左
    if(j - 1 >= 0 && i + 1 < V1 && buf[i + 1][j - 1] == '1'){sum_line++;}//上右
    if(j + 1 < V2 && i - 1 >= 0 && buf[i - 1][j + 1] == '1'){sum_line++;}//下左
    if(j + 1 < V2 && i + 1 < V1 && buf[i + 1][j + 1] == '1'){sum_line++;}//下右


    //2.如果相等 那么我们就打开这个雷区
    if(sum_line == Mine[cl_xy.x][cl_xy.y])
    {
        //但是如果 你的 旗子 插错了 也就是双击的点周围 的雷没有被你全部插上棋子 那么你的游戏就失败了
        //判断周围的点 插旗的位置 是否都是雷
        //如果这个点的周围有雷  那么这个点就 +1  Line Mine
        //FindAndShowNoLandMine(cl_xy.x,cl_xy.y);//先打开自己
                                                         //这里先更新一下触雷的点数                                  //游戏失败 返回-1
        if(j - 1 >= 0 && buf[i][j - 1] == '0' && Line[i][j - 1] == 1 )//&& Line[i][j - 1] != 2)//上
        {
            cl_xy.x = i; 
            cl_xy.y = j - 1 ;
            Failure_Game(cl_xy);
            return -1;
        }
        else if(j - 1 >= 0)//接下来就打开这个点
        {
            cl_xy.x = i; 
            cl_xy.y = j - 1 ;
            FindAndShowNoLandMine(cl_xy.x,cl_xy.y);
        }

        if(j + 1 < V2 && buf[i][j + 1] == '0' && Line[i][j + 1] == 1 )//&& Line[i][j + 1] != 2)//下
        {
            cl_xy.x = i; 
            cl_xy.y = j + 1 ;
            Failure_Game(cl_xy);
            return -1;
            }
        else if(j + 1 < V2)
        {
            cl_xy.x = i; 
            cl_xy.y = j + 1 ;
            FindAndShowNoLandMine(cl_xy.x,cl_xy.y);
            }

        if(i - 1 >= 0 && buf[i - 1][j] == '0' && Line[i - 1][j] == 1 )//&& Line[i - 1][j] != 2)//左
        {
            cl_xy.x = i - 1; 
            cl_xy.y = j ;
            Failure_Game(cl_xy);
            return -1;
            }
        else if(i - 1 >= 0)
        {
            cl_xy.x = i - 1; 
            cl_xy.y = j ;
            FindAndShowNoLandMine(cl_xy.x,cl_xy.y);
            }

        if(i + 1 < V1 && buf[i + 1][j] == '0' && Line[i + 1][j] == 1 )//&& Line[i + 1][j] != 2)//右
        {
            cl_xy.x = i + 1; 
            cl_xy.y = j ;
            Failure_Game(cl_xy);
            return -1;
            }
        else if(i + 1 < V1)//接下来就打开这个点
        {
            cl_xy.x = i + 1; 
            cl_xy.y = j ;
            FindAndShowNoLandMine(cl_xy.x,cl_xy.y);
            }

        if(j - 1 >= 0 && i - 1 >= 0 && buf[i - 1][j - 1] == '0' && Line[i - 1][j - 1] == 1 )//&& Line[i - 1][j - 1] != 2)//上左
        {
            cl_xy.x = i - 1; 
            cl_xy.y = j - 1 ;
            Failure_Game(cl_xy);
            return -1;
            }
        else if(j - 1 >= 0 && i - 1 >= 0)//接下来就打开这个点
        {
            cl_xy.x = i - 1; 
            cl_xy.y = j - 1 ;
            FindAndShowNoLandMine(cl_xy.x,cl_xy.y);
            }

        if(j - 1 >= 0 && i + 1 < V1 && buf[i + 1][j - 1] == '0' && Line[i + 1][j - 1] == 1 )//&& Line[i + 1][j - 1] != 2)//上右
        {
            cl_xy.x = i + 1; 
            cl_xy.y = j - 1 ;
            Failure_Game(cl_xy);
            return -1;
            }
        else if(j - 1 >= 0 && i + 1 < V1)//接下来就打开这个点
        {
            cl_xy.x = i + 1; 
            cl_xy.y = j - 1 ;
            FindAndShowNoLandMine(cl_xy.x,cl_xy.y);
            }

        if(j + 1 < V2 && i - 1 >= 0 && buf[i - 1][j + 1] == '0' && Line[i - 1][j + 1] == 1 )//&& Line[i - 1][j + 1] != 2)//下左
        {
            cl_xy.x = i - 1; 
            cl_xy.y = j + 1 ;
            Failure_Game(cl_xy);
            return -1;
            }
        else if(j + 1 < V2 && i - 1 >= 0)//接下来就打开这个点
        {
            cl_xy.x = i - 1; 
            cl_xy.y = j + 1 ;
            FindAndShowNoLandMine(cl_xy.x,cl_xy.y);
            }

        if(j + 1 < V2 && i + 1 < V1 && buf[i + 1][j + 1] == '0' && Line[i + 1][j + 1] == 1 )//&& [i + 1][j + 1] != 2)//下右
        {
            cl_xy.x = i + 1; 
            cl_xy.y = j + 1 ;
            Failure_Game(cl_xy);
            return -1;
            }
        else if(j + 1 < V2 && i + 1 < V1)//接下来就打开这个点
        {
            cl_xy.x = i + 1; 
            cl_xy.y = j + 1 ;
            FindAndShowNoLandMine(cl_xy.x,cl_xy.y);
            }
    }



    if(FindLandNullNum == LandNullNum)//闯关成功
    {
       Success_Game();
       return -1;
    }
        
    
    else
    {
        printf("计算的雷数 和 插的旗子数量 不相等!!!\n");
        printf("计算的雷数:%d\n",Mine[cl_xy.x][cl_xy.y]);
        printf("插的旗子数:%d\n",sum_line);
    }

    //正常退出的话  那么就返回 0 
    return 0;
}

/*  
    Renew_Point_Data:将触摸点数据 转换为 格子点数  雷区专用
    @Click_xy cl_xy:结构体 (仍是触摸点数)
*/
void Renew_Point_Data(Click_xy *cl_xy)
{
    printf("触摸的点数是:(%d,%d)",cl.dorp_x,cl.dorp_y);
    if(cl.dorp_x < 720 && cl.dorp_x > 15)
    {
        cl_xy ->x = (cl.dorp_x - 10)/42;
        cl_xy ->y = (cl.dorp_y - 23)/42;
        if(cl_xy ->x > 17){cl_xy ->x = 17;}
        if(cl_xy ->y > 10){cl_xy ->y = 10;}
    }
    else if(cl.dorp_x > 720)
    {
        if(cl.dorp_x >= 724 && cl.dorp_x <= 793
        && cl.dorp_y >= 385 && cl.dorp_y <= 425)
        {
            cl_xy ->x = 77;
            cl_xy ->y = 77;
            printf("重开函数!\n");
        }
        else if(cl.dorp_x >= 724 && cl.dorp_y >= 430 )
        {
            cl_xy ->x = 88;
            cl_xy ->y = 88;
            printf("退出函数!\n");
            
        }
    }
    

    printf("点击的格子是:(%d,%d)\n",cl_xy ->x,cl_xy ->y);
}

/*
    Show_Blank:显示未打开雷区函数
*/
void Show_Blank(void)
{
    for(int i = 0 ; i < X_Max ; i++)
    {
        for(int j = 0 ; j < Y_Max ; j++)
        {
            display_bmp("./1.landminebmp/blank.bmp",arr_x[i],arr_y[j],0);
        }
    }
}

/*
    Show_Face:笑脸显示函数
        @int num:选择显示哪个笑脸
*/
void Show_Face(int num)
{
    switch (num)
    {
    case 1:
        display_bmp("./1.landminebmp/Face1.bmp",724,206,0);
        break;
    case 2:
        display_bmp("./1.landminebmp/Face2.bmp",724,206,0);
        break;
    case 3:
        display_bmp("./1.landminebmp/Face3.bmp",724,206,0);
        break;
    case 4:
        display_bmp("./1.landminebmp/Face4.bmp",724,206,0);
        break;
    
    default:
        break;
    }
 
}

/*
    Show_Time:时间功能区显示函数
*/
void Show_Time(void)
{
    
    gettimeofday(&cur_tv,NULL);//获取开始时间
    int time = cur_tv.tv_sec - sta_tv.tv_sec;
    time = time % 1000;
    int  ge = time%10;
    int shi = time/10%10;
    int bai = time/100;

    // printf("Gewei  = %d\n",ge);
    // printf("Shiwei = %d\n",shi);
    // printf("Baiwei = %d\n",bai);

    Selext_Num(bai,724,10);
    Selext_Num(shi,747,10);
    Selext_Num(ge,770,10);
}

/*
    Show_FindLandNullNum:找出的空白区的个数
*/
void Show_FindLandNullNum(int cmd)
{
    if(cmd)
    {
        int num = FindLandNullNum;
        int  ge = num%10;
        int shi = num/10%10;
        int bai = num/100;

        // printf("Gewei  = %d\n",ge);
        // printf("Shiwei = %d\n",shi);
        // printf("Baiwei = %d\n",bai);

        Selext_Num(bai,724,55);
        Selext_Num(shi,747,55);
        Selext_Num(ge,770,55);
    }
    else
    {
        Selext_Num(0,724,55);
        Selext_Num(0,747,55);
        Selext_Num(0,770,55);
    }
    
}

/*
    Selext_Num:选择显示数字图片
        @int num:哪个数字
        @int x  :x轴的位置
        @int y  :y轴的位置
*/
void Selext_Num(int num,int x,int y)
{
    switch (num)
    {
    case 0:
        display_bmp("./1.landminebmp/00.bmp",x,y,0);//第 1 个位置
        break;
    case 1:
        display_bmp("./1.landminebmp/01.bmp",x,y,0);//第 1 个位置
        break;
    case 2:
        display_bmp("./1.landminebmp/02.bmp",x,y,0);//第 1 个位置
        break;
    case 3:
        display_bmp("./1.landminebmp/03.bmp",x,y,0);//第 1 个位置
        break;
    case 4:
        display_bmp("./1.landminebmp/04.bmp",x,y,0);//第 1 个位置
        break;
    case 5:
        display_bmp("./1.landminebmp/05.bmp",x,y,0);//第 1 个位置
        break;
    case 6:
        display_bmp("./1.landminebmp/06.bmp",x,y,0);//第 1 个位置
        break;
    case 7:
        display_bmp("./1.landminebmp/07.bmp",x,y,0);//第 1 个位置
        break;
    case 8:
        display_bmp("./1.landminebmp/08.bmp",x,y,0);//第 1 个位置
        break;
    case 9:
        display_bmp("./1.landminebmp/09.bmp",x,y,0);//第 1 个位置
        break;
    default:
        break;
    }
}


/*
    Judge_LandNum:判断点开了多少个格子
        @返回值:(int)点开雷的个数
*/
int Judge_LandNum()
{   
    int num = 0;
    for(int i = 0; i < V1; i++)
    {
        for(int j = 0; j < V2; j++)
        {
            if(Line[i][j] == 2)
            {
                num++;
            }
        }
    }
    return num;
}

/*
    Run_LandMine:雷区跑起来啦!
*/
int Run_LandMine(void)
{
    Run_Start();//雷区环境准备函数
    InitLindMine(2);
    Count_LandMine_Num();
    Print_Arry();
    //Show_Face(1);

    //这里需要一个选择难度函数
    
    while(1)
    {
        Show_Time();
        GetTouchDrop_Silther(&drp,20000);
        int i = Analyse_Drop_Dir(&drp);


        if(i == CLICK || i == LongPress || i == DBCLICK)//长按 单击 双击 进入
        {
            Renew_Point_Data(&cl_xy);

            /**************** 功能区按钮 ***************/
            if(cl_xy.x == 88 && cl_xy.y == 88)//退出
            {
                Run_End();
                return -1;
            }
            else if(cl_xy.x == 77 && cl_xy.y == 77)//游戏刷新
            {
                Run_End();
                return 0;
            }
            /**************** 游戏区按钮 ***************/
            if(i == CLICK)//单击打开该格子
            {
                int op = Open_Cell_CLICK(cl_xy);
                if(-1 == op){break;}
            }
            else if (i == LongPress)//长按插旗子
            {
                Open_Cell_LongPress(cl_xy);
            }
            else if ( i == DBCLICK)//双击快速打开
            {
                int op = Open_Cell_DBCLICK(cl_xy);
                if(-1 == op){break;}//如果游戏退出了
                    Show_FindLandNullNum(1);
                    printf("FindLandNullNum = %d\n",FindLandNullNum);
            }
        }
        Print_Arry();
    }
    Run_End();
    return 0;
}
