
#ifndef __DISPALYCHAR_H__
#define __DISPALYCHAR_H__

#include <stdio.h>
#include <lcd.h>


/*
   void lcd_draw_word:在位置为(x0,y0)处显示宽为w,高为h的汉字,颜色为 color	
      @int x0,int y0   :在哪个点开始显示
      @int w,int h     :字的 宽度 和 高度
      @char *data      :字的数据存储地
      @int color       :字的颜色   0xffffff(白色)
*/
void lcd_draw_word(int x0,int y0,int w,int h,char *data,int color);


#endif