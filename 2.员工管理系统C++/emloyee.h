#pragma once
#include <iostream>
#include <string>
#include "work.h"
using namespace std;

class Emloyee:public Worker{

public:

    //���캯��
    Emloyee(int id , string name , int dep_id);

    //��ʾ������Ϣ
    virtual void showInfo();

    //��ȡְ����λ���Ʊ�
    virtual string getDeptName();
};