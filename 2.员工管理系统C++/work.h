#pragma once
#include <iostream>
#include <string>
using namespace std;

class Worker{

public:

    //��ʾ������Ϣ  ���麯��
    virtual void showInfo() = 0;

    //��ȡ��λ����  ���麯��
    virtual string getDeptName() = 0 ;

    int m_id;//ְ�����
    string m_Name;//ְ������
    int m_DeptId;//ְ�����ڲ��ű��

};