#ifndef __TOUCH_H__
#define __TOUCH_H__

#include <stdio.h>

#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/time.h>

#include <time.h>//�¼�����

#include <string.h>
#include <stdlib.h>

#define TouchPath "/dev/input/event0"


enum Move_Dir//������Ļ�Ĳ�����ö��
{
    RIGHT,//�һ�
    LIFT,//��
    UP,//�ϻ�
    DOWN,//�»�
    DBCLICK,//˫��
    CLICK,//����
    LongPress,//����
    DIR_ERROR//����
};

typedef struct Dorp//��������������(x,y)
{
    int start_x;
    int start_y;

    int end_x;
    int end_y;

    unsigned int start_time;//(����)
    unsigned int end_time;
    unsigned int pass_time;

}Dorp;

typedef struct Dorp_Click//�������� ����˫�� �������(x,y)
{
    int dorp_x;
    int dorp_y;

}Dorp_Click;

typedef struct Judge_Region
{
    int start_x;
    int start_y;
    int end_x;
    int end_y;

}Judge_Region;

/*
    ȫ�ֱ�����
*/
int while_StartTime;//whileѭ����ʼʱ��
Dorp drp;//��Ļ���ݽṹ��
Dorp_Click cl;//����˫���������
int flag_dbclick;//��¼��������
int last_ms;//��¼���һ�ε���ʱ�ĺ�����

/*
    @GetTouchDrop_Silther():��ȡ��ĺ���
    @Dorp * drp:ȫ�ֱ��� drp �ĵ�ַ
    @int while_PassTime :��ȡ���� ���� (MAX = u32 /1000)
    ����ֵ:
        (int) -1:����
        (int)  0:�ɹ�
*/
int GetTouchDrop_Silther(Dorp * drp , int while_PassTime);//�����ڻ�������

/*
    @Analyse_Drop_Dir(Dorp * drp):������������ ����� ����/˫�� ��ȫ�ֱ��� cl �б��� (x,y)
    @Dorp * drp:struct Drop(�ṹ��)
    ����ֵ: 
        ����enum Move_Dir����ķ����ж�
*/
enum Move_Dir Analyse_Drop_Dir(Dorp * drp);                        

int JudgeRegion(Judge_Region jr ,Dorp_Click dc);
/*
    @int gettimeofday(struct timeval*tv, struct timezone *tz);
    @struct timeval*tv----->tv�Ǳ����ȡʱ�����Ľṹ��
    @struct timezone *tz--->���ڱ���ʱ�����������ʹ������NULL���ɣ���
    ����ֵ:δ֪
    struct timeval {  
����long tv_sec; // ����  
    long tv_usec; //΢����  
    }  
*/

#endif