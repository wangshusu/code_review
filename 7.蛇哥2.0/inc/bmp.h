#ifndef __BMP_H__
#define __BMP_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "lcd.h"



/******************* 此程序下的bmp文件说明 *******************

1.所有的bmp文件存在板子的 "./1.landminebmp/" 目录下

**********************************************************/

/*
    显示bmp图片函数
    @char *bmp_file:路径
    @int x0, int y0 : 在哪个点显示图片
    @int form:显示特效
    无返回值
*/
void display_bmp(char *bmp_file, int x0, int y0, int form);


#endif