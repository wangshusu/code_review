#pragma once 
#include <iostream>
#include <string>
#include "work.h"
using namespace std;


class Bose:public Worker 
{
public:
    //构造函数
    Bose(int id , string name , int dep_id);

    //显示老板信息
    virtual void showInfo();

    //获取老板职工表
    virtual string getDeptName();
};