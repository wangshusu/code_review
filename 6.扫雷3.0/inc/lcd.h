
#ifndef __LCD_H__
#define __LCD_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include <string.h>


/************* 宏定义区 *****************/
#define HEIGHT 480
#define WIDTH 800
#define X_Max 16
#define Y_Max 11

/************* 全局变量区 **************/
extern int LineColor;
extern int BackColor;
extern int arr[800][480];//像素颜色
extern int arr_x[16];//每个方块的x位置 右上角
extern int arr_y[11];//每个方块的y位置 右上角

/*
    打开屏幕函数
*/
void lcd_open(void);

/*
    关闭屏幕函数
*/
void lcd_close(void);

/*
    lcd_draw_point: 画点(给特定的像素点一个指定的颜色值)
    @x: 点的x轴坐标
    @y: 点的y轴坐标
    @color: 颜色值
*/
void lcd_draw_point(int x, int y, int color);

/*
    lcd_clear_screen:清屏每一个像素点，显示相同的颜色
    @int color:颜色
*/
void lcd_clear_screen(int color);

/*
    lcd_draw_point_form0: 基础刷屏 粗暴刷屏 0
*/
void lcd_draw_point_form0(int colorbuf[WIDTH][HEIGHT],
                          int x0,int y0,
                          int width,int hight);

/*
    lcd_draw_rectangle:画正方形
    @x0,y0:起始点
    @w , h:长方体的 宽度 高度
    @color:颜色
*/
void lcd_draw_rectangle(int x0, int y0,
                        int w,  int h,
                        int color);

void lcd_draw_circle(int x0, int y0,
                     double r, int color);

/*
    @Divide_rectangle:划分间隔函数
                     顶部底部 宽度为 head_hight = 10;
                     左部右部 宽度为 head_width = 23;
*/
void Divide_rectangle();

//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24/32
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(int x,int y,unsigned char num,unsigned char size,unsigned char mode,int color);

//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//(*p):字符串起始地址		   
//color:字体颜色
void LCD_ShowString(int x,int y,int width,int height,unsigned char size,unsigned char *p,int color);

//在指定位置显示一个汉字
//x,y:起始坐标
//num:要显示的汉字在字库中的位置
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowHanzi(int x,int y,unsigned char num,int size,unsigned char mode,int color);

//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//(*p):字符串起始地址		  
//color:字体颜色
void LCD_ShowHanzis(int x,int y,int width,int height,int size,unsigned char *p,int color);


#endif