#pragma once
#include <iostream>
#include <string>
#include "work.h"
using namespace std;

class Manager:public Worker
{
public:
    //构造函数
    Manager(int id , string name , int dep_id);

    //显示个人信息  纯虚函数
    virtual void showInfo();

    //获取岗位名称  纯虚函数
    virtual string getDeptName();

};