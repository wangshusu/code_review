#pragma once//��ֹͷ�ļ����ظ�����
#include <iostream>
#include <fstream>
using namespace std;
#include "work.h"
#include "bose.h"
#include "manager.h"
#include "emloyee.h"

#define FILENAME "2.empFile.txt"

class WorkManager{

public:

    //���캯��
    WorkManager();

    //��ʾ��������
    void Show_Menu();

    //�˳�����
    void exitSystem();

    //����ְ����Ϣ
    void Add_Emp();

    //�����ļ�
    void Save();

    //��ȡ��¼��ְ������
    int get_EmpNum();

    //��ʼ��Ա��
    void init_Emp();

    //��ʾְ����Ϣ
    void show_Empy();

    //ɾ��ְ������
    void Del_Emp();

    //�ж�ְ���Ƿ���� ���ڷ���ְ��λ�� �����ڷ���-1
    int IsExist(int id);

    //�޸�Ա������
    void Mod_Emp();

    //����Ա��
    void Find_EMP();

    //����
    void Sort_Emp();

    //���
    void Delet_File();

    //��������
    ~WorkManager();

    //��־�ļ��Ƿ�Ϊ��
    bool m_fileIsEmpty;

    //��¼�ļ��е���������
    int m_EmpNum;

    //Ա�������ָ��
    Worker ** m_EmpArry;

};

