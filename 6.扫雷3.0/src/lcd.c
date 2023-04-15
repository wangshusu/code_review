#include "lcd.h"
#include "ziku.h"

//                                                   printf("%s %d \n",__FUNCTION__,__LINE__);
int fb_fd = -1; //屏幕文件的文件描述符
int* plcd = NULL;//指向内核帧缓冲区的首地址
int LineColor;
int BackColor;

int arr[800][480];//像素颜色
int arr_x[16];//每个方块的x位置 右上角
int arr_y[11];//每个方块的y位置 右上角


void lcd_open(void)
{	
    int fd = open("/dev/fb0", O_RDWR);
	if (fd == -1)
	{
		printf("failed to open /dev/fb0\n");
		return ;
	}
    fb_fd = fd;

    plcd = mmap(
                NULL,
                800*480*4,
                PROT_WRITE,
                MAP_SHARED,
                fd,
                0    );
    if (plcd == MAP_FAILED)
    {
        printf("Sorry,failed to mmap\n");
        return ;
    }
    
    LineColor = 0xb0b0b0;
    BackColor = 0xffff;
}

void lcd_close(void)
{
    munmap(plcd, 800*480*4);//关闭映射
    close(fb_fd);//关闭屏幕文件
}

/*
    lcd_draw_point: 画点(给特定的像素点一个指定的颜色值)
    @x: 点的x轴坐标
    @y: 点的y轴坐标
    @color: 颜色值
    返回值：
        无。
*/
void lcd_draw_point(int x, int y, int color)
{
    if (x >= 0 && x < 800 && y >= 0 && y < 480)
    {
        *(plcd + y*800 +x) = color;
    }
}

/*
    lcd_draw_point_base: 基础 粗暴刷屏 1
    返回值：
        无。
*/
void lcd_draw_point_form0(int colorbuf[WIDTH][HEIGHT],int x0,int y0,int width,int hight)
{
    for(int i = 0 ; i < width ; i++)
    {
        for(int j = 0 ; j < hight ; j++)
        {                                                    
            *(plcd + (j + y0)*800 + i + x0) = colorbuf[i][j];//s++;
        }
    }
}


//"清屏": 给屏幕上每一个像素点，显示相同的颜色
void lcd_clear_screen(int color)
{
    int x, y;

    for (y = 0 ; y < 480; y++)
    {
        for (x = 0; x < 800; x++)
        {
            lcd_draw_point(x,y, color);
        }
    }
}

void lcd_draw_rectangle(int x0, int y0,
                        int w,  int h,
                        int color)
{
    //(x,y)为该矩形内的任意一个点的坐标
    //  x >= x0 &&  x < x0 + w
    //  y >= y0 &&  y < y0 + h
    int x,y;

    for (y = y0; y < y0 + h; y++)
    {
        for (x = x0; x < x0 + w; x++)
        {
            lcd_draw_point(x,y,color);
        }

    }
}

void lcd_draw_circle(int x0, int y0,
                     double r, int color)
{
    //(x,y)为圆内的任意一个点坐标
    // (x-x0)*(x-x0) + (y-y0)*(y-y0) <= r*r
    int x,y;

    for (y = 0; y < 480; y++)
    {
        for (x = 0; x < 800; x++)
        {
            if ((x-x0)*(x-x0) + (y-y0)*(y-y0) <= r*r)
            {
                lcd_draw_point(x,y,color);
            }
        }
    }

}

//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24/32
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(int x,int y,unsigned char num,unsigned char size,unsigned char mode,int color)
{
	int temp,t1;
	int y0,t;
	if(size%4 != 0 || (size/4) < 3 || (size/4) > 12)return;//不支持的size 支持12/16/20/24/28/32/36/40/44/48
	if(size > 40)
	{
		x+=(size-40)/2;//替换后居中
		y+=(size-40)/2;
		size = 40;
	}
	y0=y;
	int csize=( size/8 + ((size%8)?1:0)) * (size/2);//得到字体一个字符对应点阵集所占的字节数 size/2 半字显示
	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字模）

	for(t=0;t<csize;t++)
	{
		switch (size)
		{
		case 12:	temp=asc2_1206[num][t];break;//调用1206字体
		case 16:	temp=asc2_1608[num][t];break;//调用1608字体
		case 20:	temp=asc2_2010[num][t];break;//调用1608字体
		case 24:	temp=asc2_2412[num][t];break;//调用1608字体
		case 28:	temp=asc2_2814[num][t];break;//调用1608字体
		case 32:	temp=asc2_3216[num][t];break;//调用1608字体
		case 36:	temp=asc2_3618[num][t];break;//调用1608字体	
		case 40:	temp=asc2_4020[num][t];break;//调用1608字体
		//case 44:	temp=asc2_4422[num][t];break;//调用1608字体		(需要同步改动ziku.h、lcd.c 208)
		//case 48:	temp=asc2_4824[num][t];break;//调用1608字体
		default:
			return ;
		}
		
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80){lcd_draw_point(x,y,color);}
			else if(mode==0){lcd_draw_point(x,y,0xffffff);}//有背景色模式 默认白色背景色
			temp<<=1;
			y++;
			if(y>=480)return;//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=800)return;//超区域了
				break;
			}
		}
	}
}

//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//(*p):字符串起始地址		  
//color:字体颜色
void LCD_ShowString(int x,int y,int width,int height,unsigned char size,unsigned char *p,int color)
{

	int x0=x;
	width+=x;
	height+=y;
	if(size%4 != 0 || (size/4) < 3 || (size/4) > 12)//不支持的size 支持12/16/20/24/28/32/36/40/44/48
	{
		printf("字库中没有该字号字模\n");
		return;//不支持的size 
	}
	while((*p)!='\0')//判断是打印完成
	{
		if(x>=width){x=x0;y+=size;}//判断横向是否超出，如果超出换行
		if(y>=height)break;//退出
		LCD_ShowChar(x,y,*p,size,1,color);//0 白色字体背景 1 无背景色
		x+=size/2;//标记到下一个字的X轴位置
		p++;//下一个字
	}
}


//在指定位置显示一个汉字
//x,y:起始坐标
//num:要显示的汉字在字库中的位置
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowHanzi(int x,int y,unsigned char num,int size,unsigned char mode,int color)
{
	unsigned char temp,t1;
	int y0=y,t;
	int csize=( size/8 + ((size%8)?1:0)) * (size);//得到一个汉字对应点阵集所占的字节数

	for(t=0;t<csize;t++)
	{
		if(size==40)temp=Chinese_4040[num][t];//调用4040字体
		else if(size==48)temp=Chinese_4848[num][t];//调用4824字体
		else return;//没有的字库
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80){lcd_draw_point(x,y,color);}
			else if(mode==0){lcd_draw_point(x,y,0xffffff);}//有背景色模式 默认白色背景色
			temp<<=1;
			y++;
			if(y>=480)return;//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=800)return;//超区域了
				break;
			}
		}
	}
}

//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//(*p):字符串起始地址		  
//color:字体颜色
void LCD_ShowHanzis(int x,int y,int width,int height,int size,unsigned char *p,int color)
{
	int x0=x;
	width+=x;
	height+=y;

	while((*p)!='\0')//判断是打印完成
	{
		if(x>=width){x=x0;y+=size;}//判断横向是否超出，如果超出换行
		if(y>=height)break;//退出
		LCD_ShowHanzi(x,y,(*p - '0'),size,1,color);//0 白色字体背景 1 无背景色		现在利用字符串来辅助显示（有偏移量），后面可改其他方式显示
		x+=size;//标记到下一个字的X轴位置
		p++;//下一个字
	}
}

/*
    @Divide_rectangle:划分间隔函数
                     顶部底部 宽度为 head_hight = 10;
                     左部右部 宽度为 head_width = 23;
*/
void Divide_rectangle()
{
    lcd_clear_screen(BackColor);

    int hight = 2;    
    int head_hight = 10;
    int LineNum = head_hight;//顶部底部距离
    int LineLong_line = 716 ;//横线的长度 head_hight*2 + 40*Y_Max + 2*(Y_Max-1) 46+640+30
    printf("Linelong_line = %d\n",LineLong_line);

    lcd_draw_rectangle(0,0,LineLong_line,head_hight,LineColor);//横线
    for(int i = 0 ; i < Y_Max ; i++)
    {
                                                                        //printf("lcd\n");
        arr_y[i] = LineNum;
        LineNum += 40;
        lcd_draw_rectangle(0,LineNum,LineLong_line,hight,LineColor);//横线
        LineNum += hight;
    }
    lcd_draw_rectangle(0,LineNum,LineLong_line,head_hight - hight,LineColor);//横线

    int width = 2;
    int head_width = 23;
    int StrightNum = head_width;//左右距离

    lcd_draw_rectangle(0,0,head_width,480,LineColor);//线
    for(int i = 0 ; i < X_Max ; i++)
    {
        arr_x[i] = StrightNum;
        StrightNum += 40;
        lcd_draw_rectangle(StrightNum,0,width,480,LineColor);//线
        StrightNum += hight;
    }
    lcd_draw_rectangle(StrightNum,0,head_width - width,480,LineColor);//线

    printf("x = %d\n",arr_x[1]);
    printf("y = %d\n",arr_y[1]);

    //测试(x,y)是否拿到
    // for(int i = 0 ; i < 11 ; i++)
    // {
    //     lcd_draw_rectangle(arr_x[i],arr_y[i],40,40,0xffffff);
    // }
    // lcd_draw_rectangle(arr_x[15],arr_y[0],40,40,0xffffff);
    // lcd_draw_rectangle(arr_x[14],arr_y[1],40,40,0xffffff);

    /********************* 雷区美化 *******************/
    int dis1 = 4;
    //竖线修改美化
    lcd_draw_rectangle(arr_x[0]-dis1,arr_y[0]-dis1,2,480-head_hight*2 + 2 * dis1,0xffff);
    lcd_draw_rectangle(arr_x[X_Max - 1]+dis1+40-2,arr_y[0]-dis1,2,480-head_hight*2 + 2 * dis1,0xffff);
    //横线修改美化
    lcd_draw_rectangle(arr_x[0]-dis1, arr_y[0]-dis1, 800-80-head_width*2 - 4 + 2 * dis1, 2, 0xffff);
    lcd_draw_rectangle(arr_x[0]-dis1, arr_y[Y_Max - 1]+40+dis1-2, 800-80-head_width*2 - 4+ 2 * dis1, 2, 0xffff);

    /********************* 功能区分割 *******************/
    lcd_draw_rectangle(724,385,69,40,LineColor);//退出按钮  (724,385) ~ (793,425)
    lcd_draw_rectangle(724,430,69,40,LineColor);//重开按钮  (724,430) ~ (793,470)
    lcd_draw_rectangle(724,206,69,69,LineColor);//笑脸哥
}

