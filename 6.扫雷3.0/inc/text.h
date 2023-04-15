/*
 * @Author: WYy (创建者)
 * @LastEditors: WYy (最后一次修改者)
 * @description:   (作用说明)
 * @Date: 2022-05-20 20:10:29
 * @LastEditTime: 2022-06-27 09:44:05
 */
#ifndef __TEXT_H__
#define __TEXT_H__	 

#include <string.h>

#include "lcd.h"



//#define GBK_FON_PATH "/home/wzx/word/"  //宋体，可考虑其他字体
#define GBK_FON_PATH "/wzx/word/"  //宋体，可考虑其他字体
#define GBK12FON_file "GBK12.FON"
#define GBK16FON_file "GBK16.FON"
#define GBK20FON_file "GBK20.FON"
#define GBK24FON_file "GBK24.FON"
#define GBK28FON_file "GBK28.FON"
#define GBK32FON_file "GBK32.FON"
#define GBK36FON_file "GBK36.FON"
#define GBK40FON_file "GBK40.FON"
#define GBK44FON_file "GBK44.FON"
#define GBK48FON_file "GBK48.FON"

typedef unsigned char u8;
typedef unsigned int u16; 

//不要多线程显示汉字不然会乱码


//如果直接调用被注释掉的函数将无法显示（没打开文件）
//int Get_HzMat(unsigned char *code,unsigned char *mat,u8 size);			            //得到汉字的点阵码
//void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode,int color);					    //在指定位置显示一个汉字

//下面两个函数直接调用极有可能显示汉字（lcd打开）
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode,int color);	//在指定位置显示一个字符串 
void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u16 len,int color);                    //在指定宽度的中间显示字符串

#endif


