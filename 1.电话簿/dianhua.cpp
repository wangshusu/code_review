#include <iostream>
using namespace std;
#include "dianhua.h"


void help_choice()
{
    cout << "**********************" <<endl;
    cout << "****1、添加联系人******" <<endl;
    cout << "****2、显示联系人******" <<endl;
    cout << "****3、删除联系人******" <<endl;
    cout << "****4、修改联系人******" <<endl;
    cout << "****5、清空联系人******" <<endl;
    cout << "**********************" <<endl;
}

//初始化PHONE BOOK
phonebook *phonebook_Init(void)
{
    phonebook *pnew = (phonebook *)calloc(1,sizeof(phonebook));
    if(!pnew)
        return NULL;
    memset(pnew,0,sizeof(pnew));
    return pnew;
}

//创建并初始化 Linkman 节点 返回节点
linkman_node *Creat_linkman_node()
{
    linkman_node *pnew = (linkman_node *)calloc(1,sizeof(linkman_node));
    if(!pnew)
        return NULL;
    memset(pnew,0,sizeof(pnew));
    return pnew;
}

//添加联系人
void Add_Linkman(phonebook *PB,int i)
{
    string m_Name;
    string m_PhoneNumber;
    int m_Age;
    int m_Sex;
    string m_Address;
    linkman_node *LinkNode = Creat_linkman_node();

    cout << "正在载入添加联系人....." << endl;
    cout << "请输入联系人的姓名：";
    cin >> m_Name;
    LinkNode ->m_Name = m_Name;

    cout << "请输入联系人的性别(1:男性 2:女性)：";
    cin >> m_Sex;
    while(m_Sex != 1 && m_Sex != 2)
    {
        cout << "请重新输入性别(1:男性 2:女性)：" ;
        cin >> m_Sex;
    }
    LinkNode ->m_Sex = m_Sex;

    cout << "请输入联系人的年龄：";
    cin >> m_Age;
    while(m_Age < 0 || m_Age > 150)
    {
        cout << "年龄有误，请重新输入年龄：" ;
        cin >> m_Age;
    }
    LinkNode ->m_Age = m_Age;

    cout << "请输入联系人的地址：";
    cin >> m_Address;
    LinkNode ->m_Address = m_Address;

    cout << "请输入联系人的电话：";
    cin >> m_PhoneNumber;
    LinkNode ->m_PhoneNumber = m_PhoneNumber;

    PB ->Person_Array[i] = LinkNode;
    PB ->phone_num++;
}

//显示全部联系人
void Show_All_Linkman(phonebook *PB)
{
    if(!PB)
    {
        cout << "里面没有联系人" << endl;
        return;
    }
    int i ;
    for(i = 0 ; i < PB ->phone_num ; i ++)
    {
        cout << "姓名：" << PB ->Person_Array[i] ->m_Name << "\t";
        cout << "性别：" << (PB ->Person_Array[i] ->m_Sex == 1 ?"男":"女" )<< "\t";
        cout << "年龄：" << PB ->Person_Array[i] ->m_Age << "\t";
        cout << "电话：" << PB ->Person_Array[i] ->m_PhoneNumber << "\t";
        cout << "住址：" << PB ->Person_Array[i] ->m_Address << endl;
    }
}