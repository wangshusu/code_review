#include "landmine.h"
#include "lcd.h"

//全局变量区
int V1 = 16;//X轴
int V2 = 11;//Y轴
int LandMineNum;
int LandNullNum;
int FindLandNullNum;
char buf[16][11];
int Line[16][11];//1为雷
int Mine[16][11];//周围雷的个数

/*
    InitLindMine:初始化雷区
    @int DegDiff :雷的个数  (1 = 15   , 2 = 20   , 3 = 25  )
    返回值:无
*/
void InitLindMine(int DegDiff)
{
    switch (DegDiff)
    {
    case 1:
        DegDiff = 10;
        break;
    case 2:
        DegDiff = 18;
        break;
    case 3:
        DegDiff = 25;
        break;
    default:
        printf("LandSize error!\n");
        break;
    }
    
    //1.雷区的初始化
    memset(Line,0,sizeof(Line));//初始化为0
    for(int i = 0 ; i < DegDiff ; i++)//这里是雷区 赋值
    {
        int x = rand() % (V1 );
        int y = rand() % (V2 );
        while(1 == Line[x][y])//不得重复
        {
            x = rand() % (V1 );
            y = rand() % (V2 );
        }
        Line[x][y] = 1;
    }

    //memset(buf,0,sizeof(buf));//将棋子标记的数组初始化为 "0"  ------> 错误 原因:

    for(int i = 0; i < V1 ; i++)
    {
        for(int j = 0 ; j < V2 ; j++)
        {
            buf[i][j] = '0';
        }
    }

    LandMineNum = DegDiff;
    LandNullNum = V1*V2 - DegDiff;
    FindLandNullNum = 0;
    printf("%d\n",LandMineNum);
    printf("%d\n",LandNullNum);
    printf("%d\n",LandMineNum + LandNullNum);
    printf("%d\n",V1 * V2);
    return;
}

/*
    Count_LandMine_Num:计算周围雷个数函数
    返回值:无
*/
void Count_LandMine_Num(void)
{
    memset(Mine,0,sizeof(Mine)); //清除Mine 里面的内容
    for(int i = 0 ; i < V1 ; i++)
    {
        for(int j = 0 ; j < V2 ; j++)
        {
            //如果这个点的周围有雷  那么这个点就 +1  Line Mine
            //j - 1 >= 0
            //j + 1 < V2
            //i - 1 >= 0
            //i + 1 < V1
            if(j - 1 >= 0 && Line[i][j - 1] == 1){Mine[i][j]++;}//上
            if(j + 1 < V2 && Line[i][j + 1] == 1){Mine[i][j]++;}//下
            if(i - 1 >= 0 && Line[i - 1][j] == 1){Mine[i][j]++;}//左
            if(i + 1 < V1 && Line[i + 1][j] == 1){Mine[i][j]++;}//右
            if(j - 1 >= 0 && i - 1 >= 0 && Line[i - 1][j - 1] == 1){Mine[i][j]++;}//上左
            if(j - 1 >= 0 && i + 1 < V1 && Line[i + 1][j - 1] == 1){Mine[i][j]++;}//上右
            if(j + 1 < V2 && i - 1 >= 0 && Line[i - 1][j + 1] == 1){Mine[i][j]++;}//下左
            if(j + 1 < V2 && i + 1 < V1 && Line[i + 1][j + 1] == 1){Mine[i][j]++;}//下右
        }
    }
    return;
}

/*
    FindAndShowNoLandMine:找到并且标记无雷区
    @int i,j:要找的点的坐标
*/
void FindAndShowNoLandMine(int x, int y)
{
    if(2 == Line[x][y] || buf[x][y] == '1') //如果已经遍历 那么就停止遍历
    {
        return;//结束当前递归
    }
    
    else if(0 == Line[x][y])//如果是 无雷区 并且没有显示过
    {
        
        FindLandNullNum++;  

        printf("空白处+1\n");

        Line[x][y] = 2;//将这个点置为已经出现
        //printf("already show (x,y) = (%d,%d)\n",x,y);
        
        if(Mine[x][y] != 0)//这个点周围有雷 那么就显示这一个点
        {
            clect_bmp(x,y);
            return;
        }
        else
        {
            clect_bmp(x,y);
            if(x - 1 >= 0){FindAndShowNoLandMine(x - 1, y );}//上
            if(x + 1 < V1){FindAndShowNoLandMine(x + 1, y );}//下
            if(y - 1 >= 0){FindAndShowNoLandMine(x ,y - 1 );}//左
            if(y + 1 < V2){FindAndShowNoLandMine(x ,y + 1 );}//右
            if(x - 1 >= 0 && y - 1 >= 0){FindAndShowNoLandMine(x - 1, y - 1 );}//上左
            if(x + 1 < V1 && y - 1 >= 0){FindAndShowNoLandMine(x + 1, y - 1 );}//下左
            if(x - 1 >= 0 && y + 1 < V2){FindAndShowNoLandMine(x - 1, y + 1 );}//上右
            if(x + 1 < V1 && y + 1 < V2){FindAndShowNoLandMine(x + 1, y + 1 );}//下右
            return;//如果这个点周围都没有点了,那么就结束这个点
        }
    }
    else
    {
        printf("error = %d\n  %d  %d ",Line[x][y],x,y);
        printf("FindAndShowNoLandMine was error\n");
        return;
    }
    
}

/*
    @Print_Arry:测试输出数组
*/
void Print_Arry(void)
{
    int i,j;
    printf("\n");
    for(i = 0 ; i < V2 ; i++)
    {
        for(j = 0 ; j < V1 ; j++)
        {
            if(Line[j][i] == 0)
                printf(". ");
            else
                printf("%d ",Line[j][i]);
        }
        printf("\n");
    }
    printf("\n");

    printf("\n");
    for(i = 0 ; i < V2 ; i++)
    {
        for(j = 0 ; j < V1 ; j++)
        {   
            if(Mine[j][i] == 0)
                printf(". ");
            else
                printf("%d ",Mine[j][i]);
        }
        printf("\n");
    }
    printf("\n");
}

/*
    clect_bmp:雷区小方块显示
*/
void clect_bmp(int x,int y)
{
    switch (Mine[x][y])
    {
    case 0:
        display_bmp("./1.landminebmp/0.bmp",arr_x[x],arr_y[y],0);
        break;
    case 1:
        display_bmp("./1.landminebmp/1.bmp",arr_x[x],arr_y[y],0);
        break;
    case 2:
        display_bmp("./1.landminebmp/2.bmp",arr_x[x],arr_y[y],0);
        break; 
    case 3:
        display_bmp("./1.landminebmp/3.bmp",arr_x[x],arr_y[y],0);
        break; 
    case 4:
        display_bmp("./1.landminebmp/4.bmp",arr_x[x],arr_y[y],0);
        break; 
    case 5:
        display_bmp("./1.landminebmp/5.bmp",arr_x[x],arr_y[y],0);
        break; 
    case 6:
        display_bmp("./1.landminebmp/6.bmp",arr_x[x],arr_y[y],0);
        break; 
    case 7:
        display_bmp("./1.landminebmp/7.bmp",arr_x[x],arr_y[y],0);
        break; 
    case 8:
        display_bmp("./1.landminebmp/8.bmp",arr_y[y],arr_x[x],0);
        break;   
    case 9:
        display_bmp("./1.landminebmp/9.bmp",arr_y[y],arr_x[x],0);
        break; 
    default:
        break;
    }
}