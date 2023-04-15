#ifndef __DIANHUA_H__
#define __DIANHUA_H__
#include <cstring>
#include <windows.h>
#define Person_MAX 100

typedef struct linkman_node
{
    string m_Name;
    string m_PhoneNumber;
    int m_Age;
    int m_Sex;
    string m_Address;
}linkman_node;

typedef struct phonebook
{
    int phone_num;
    linkman_node *Person_Array[Person_MAX];
}phonebook;

//显示帮助信息
void help_choice(void);

//初始化 PHONE BOOK 返回初始化的节点
phonebook *phonebook_Init(void);

//创建并初始化 Linkman 节点 返回节点
linkman_node *Creat_linkman_node();

//添加联系人
void Add_Linkman(phonebook *PB,int i);

//显示全部联系人
void Show_All_Linkman(phonebook *PB);

//查找联系人
void Found_Linkman(phonebook *PB);

//修改联系人
void Change_Linkman(phonebook *PB);

//删除联系人

#endif