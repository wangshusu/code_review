#ifndef __TOUCH_H__
#define __TOUCH_H__

#include <linux/input.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "lcd.h"
#include "LinkedListQueue.h"
#include "showphoto.h"


#define Touch_Path "/dev/input/event0"

enum Move_Dir//������Ļ�Ĳ�����ö��
{
    RIGHT,//�һ�
    LIFT,//��
    UP,//�ϻ�
    DOWN,//�»�
    DBCLICK,//˫��
    CLICK,//����
    DIR_ERROR//����
};

/*
    @enum Move_Dir GetMove_Dir():��ȡ��Ļ��������
    @��������
    ����ֵ:
        ����enum Move_Dir����ķ����ж�
*/
enum Move_Dir GetMove_Dir(Node *ptr);

/*
    @enum Move_Dir Decide_Dir:�жϷ�����
    @���� ��ʼ �� ��ֹ ��(x,y)����
    ����ֵ:
        ����enum Move_Dir����ķ����ж�
*/
enum Move_Dir Decide_Dir(int x0,int x1,int y0,int y1);

#endif