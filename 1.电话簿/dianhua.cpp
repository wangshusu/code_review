#include <iostream>
using namespace std;
#include "dianhua.h"


void help_choice()
{
    cout << "**********************" <<endl;
    cout << "****1�������ϵ��******" <<endl;
    cout << "****2����ʾ��ϵ��******" <<endl;
    cout << "****3��ɾ����ϵ��******" <<endl;
    cout << "****4���޸���ϵ��******" <<endl;
    cout << "****5�������ϵ��******" <<endl;
    cout << "**********************" <<endl;
}

//��ʼ��PHONE BOOK
phonebook *phonebook_Init(void)
{
    phonebook *pnew = (phonebook *)calloc(1,sizeof(phonebook));
    if(!pnew)
        return NULL;
    memset(pnew,0,sizeof(pnew));
    return pnew;
}

//��������ʼ�� Linkman �ڵ� ���ؽڵ�
linkman_node *Creat_linkman_node()
{
    linkman_node *pnew = (linkman_node *)calloc(1,sizeof(linkman_node));
    if(!pnew)
        return NULL;
    memset(pnew,0,sizeof(pnew));
    return pnew;
}

//�����ϵ��
void Add_Linkman(phonebook *PB,int i)
{
    string m_Name;
    string m_PhoneNumber;
    int m_Age;
    int m_Sex;
    string m_Address;
    linkman_node *LinkNode = Creat_linkman_node();

    cout << "�������������ϵ��....." << endl;
    cout << "��������ϵ�˵�������";
    cin >> m_Name;
    LinkNode ->m_Name = m_Name;

    cout << "��������ϵ�˵��Ա�(1:���� 2:Ů��)��";
    cin >> m_Sex;
    while(m_Sex != 1 && m_Sex != 2)
    {
        cout << "�����������Ա�(1:���� 2:Ů��)��" ;
        cin >> m_Sex;
    }
    LinkNode ->m_Sex = m_Sex;

    cout << "��������ϵ�˵����䣺";
    cin >> m_Age;
    while(m_Age < 0 || m_Age > 150)
    {
        cout << "���������������������䣺" ;
        cin >> m_Age;
    }
    LinkNode ->m_Age = m_Age;

    cout << "��������ϵ�˵ĵ�ַ��";
    cin >> m_Address;
    LinkNode ->m_Address = m_Address;

    cout << "��������ϵ�˵ĵ绰��";
    cin >> m_PhoneNumber;
    LinkNode ->m_PhoneNumber = m_PhoneNumber;

    PB ->Person_Array[i] = LinkNode;
    PB ->phone_num++;
}

//��ʾȫ����ϵ��
void Show_All_Linkman(phonebook *PB)
{
    if(!PB)
    {
        cout << "����û����ϵ��" << endl;
        return;
    }
    int i ;
    for(i = 0 ; i < PB ->phone_num ; i ++)
    {
        cout << "������" << PB ->Person_Array[i] ->m_Name << "\t";
        cout << "�Ա�" << (PB ->Person_Array[i] ->m_Sex == 1 ?"��":"Ů" )<< "\t";
        cout << "���䣺" << PB ->Person_Array[i] ->m_Age << "\t";
        cout << "�绰��" << PB ->Person_Array[i] ->m_PhoneNumber << "\t";
        cout << "סַ��" << PB ->Person_Array[i] ->m_Address << endl;
    }
}