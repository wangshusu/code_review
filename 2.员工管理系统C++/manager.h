#pragma once
#include <iostream>
#include <string>
#include "work.h"
using namespace std;

class Manager:public Worker
{
public:
    //���캯��
    Manager(int id , string name , int dep_id);

    //��ʾ������Ϣ  ���麯��
    virtual void showInfo();

    //��ȡ��λ����  ���麯��
    virtual string getDeptName();

};