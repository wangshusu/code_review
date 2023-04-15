
#ifndef __RUN_H__
#define __RUN_H__

#include "lcd.h"
#include "landmine.h"
#include "landtouch.h"
#include "bmp.h"
#include "displaychar.h"
#include "text.h"

#define GameOver0 "��Ϸʧ��!"
#define GameOver2 "���سɹ�!"
#define GameOver1 "���������ʼ��һ��..."

/****************** ���ֺ���ע������˵�� *******************
1.Renew_Point_Data(Click_xy *cl_xy)
  �������֮��ṹ�������(x,y)�洢���Ǵ������Ӧ�� �����ָ� ��λ��
  Max(x,y) = (10,15)  �����������������ֵ
  �������ͬ�������ŷ��ع������Ĺ��� ����������ǹ�������Ӧ��λ��
  ���ﷵ�ص�ֵ����:
                �ؿ�:(77,77)
                �˳�:(88,88)
**********************************************************/

typedef struct Click_xy//�������� ����˫�� �������(x,y)
{
    int x;
    int y;

}Click_xy;

/*
    Run_Start:��������ǰ����׼������
*/
void Run_Start(void);

/*
    Run_End:��Ϸ�˳���β����
*/
void Run_End(void);

/*
    Failure_Game:�㵽�� ��Ϸʧ�ܺ���
*/
void Failure_Game(Click_xy cl_xy);

/*
    Success_Game:���سɹ�����
*/
void Success_Game(void);

/*
    Open_Cell_CLICK:�����򿪸���
    @Click_xy cl_xy:�ṹ�� (�Ѿ��Ǹ��ӵ���)
    @����ֵ:
        ������� 0  --->�Ǿ���������
        ������� -1 --->��Ϸ����,��Ҫ������һ��
*/
int Open_Cell_CLICK(Click_xy cl_xy);

/*
    Open_Cell_LongPress:����ִ�в����Ӻ���
    @Click_xy cl_xy:�ṹ�� (�Ѿ��Ǹ��ӵ���)
*/
void Open_Cell_LongPress(Click_xy cl_xy);

/*  
    Renew_Point_Data:������������ ת��Ϊ ���ӵ���
    @Click_xy cl_xy:�ṹ�� (���Ǵ�������)
*/
void Renew_Point_Data(Click_xy *cl_xy);

/*
    Open_Cell_DBCLICK:˫������ ��
    @Click_xy cl_xy:�ṹ�� (�Ѿ��Ǹ��ӵ���)
    @����ֵ:
        ������� 0  --->�Ǿ���������
        ������� -1 --->��Ϸ����,��Ҫ������һ��
*/
int Open_Cell_DBCLICK(Click_xy cl_xy);

/*
    Show_FindLandNullNum:�ҳ��Ŀհ����ĸ���
*/
void Show_FindLandNullNum(int cmd);

/*
    Run_LandMine:������������!
*/
int Run_LandMine(void);

/*
    Show_Blank:��ʾδ����������
*/
void Show_Blank(void);

/*
    Show_Time:ʱ�书������ʾ����
*/
void Show_Time(void);

/*
    Show_Face:Ц����ʾ����
        @int num:ѡ����ʾ�ĸ�Ц��
*/
void Show_Face(int num);

/*
    Selext_Num:ѡ����ʾ����ͼƬ
        @int num:�ĸ�����
        @int x  :x���λ��
        @int y  :y���λ��
*/
void Selext_Num(int num,int x,int y);
#endif