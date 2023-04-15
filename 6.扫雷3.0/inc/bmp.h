#ifndef __BMP_H__
#define __BMP_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

/******************* 此程序下的bmp文件说明 *******************

1.所有的bmp文件存在板子的 "./1.landminebmp/" 目录下
2.雷区的显示图片(40*40)  由 1.bmp ~ 8.bmp 
                        雷 mine1.bmp    <--- 红色叉叉雷
                        雷 mine2.bmp    <--- 普通雷
                        空 blank.bmp    <--- 无点开的空白区域
                        旗子 flag.bmp   
3.时间显示图片(23*40) 以及点开的空白个数图片
                     由 00.bmp ~ 09.bmp
4.笑脸显示图片(69*69)   由 Face1.bmp    微笑   <-----------如今显示还有问题 需要改进
                   Face2.bmp    惊讶
                   Face3.bmp    失败
                   Face4.bmp    墨镜
 
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