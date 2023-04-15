#pragma once
#include <iostream>
#include <string>
#include "work.h"
using namespace std;

class Emloyee:public Worker{

public:

    //构造函数
    Emloyee(int id , string name , int dep_id);

    //显示个人信息
    virtual void showInfo();

    //获取职工单位名称表
    virtual string getDeptName();
};