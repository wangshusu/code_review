#ifndef __BMP_H__
#define __BMP_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


/*
    ��ʾbmpͼƬ����
    @char *bmp_file:·��
    @int x0, int y0 : ���ĸ�����ʾͼƬ
    @int cmd:��ʾ��Ч
    �޷���ֵ
*/
void display_bmp(char *bmp_file, int x0, int y0, int cmd);


#endif