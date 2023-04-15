/*
 * @Author: WYy (������)
 * @LastEditors: WYy (���һ���޸���)
 * @description:   (����˵��)
 * @Date: 2022-05-20 20:10:29
 * @LastEditTime: 2022-06-27 09:44:05
 */
#ifndef __TEXT_H__
#define __TEXT_H__	 

#include <string.h>

#include "lcd.h"


//#define GBK_FON_PATH "/home/wzx/word/"  //���壬�ɿ�����������
#define GBK_FON_PATH "/wzx/word/"  //���壬�ɿ�����������
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

//��Ҫ���߳���ʾ���ֲ�Ȼ������


//���ֱ�ӵ��ñ�ע�͵��ĺ������޷���ʾ��û���ļ���
//int Get_HzMat(unsigned char *code,unsigned char *mat,u8 size);			            //�õ����ֵĵ�����
//void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode,int color);					    //��ָ��λ����ʾһ������

//������������ֱ�ӵ��ü��п�����ʾ���֣�lcd�򿪣�
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode,int color);	//��ָ��λ����ʾһ���ַ��� 
void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u16 len,int color);                    //��ָ�����ȵ��м���ʾ�ַ���

#endif


