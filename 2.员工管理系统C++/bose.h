#pragma once 
#include <iostream>
#include <string>
#include "work.h"
using namespace std;


class Bose:public Worker 
{
public:
    //���캯��
    Bose(int id , string name , int dep_id);

    //��ʾ�ϰ���Ϣ
    virtual void showInfo();

    //��ȡ�ϰ�ְ����
    virtual string getDeptName();
};