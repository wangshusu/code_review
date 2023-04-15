#pragma once
#include <iostream>
#include <string>
using namespace std;

class Worker{

public:

    //显示个人信息  纯虚函数
    virtual void showInfo() = 0;

    //获取岗位名称  纯虚函数
    virtual string getDeptName() = 0 ;

    int m_id;//职工编号
    string m_Name;//职工姓名
    int m_DeptId;//职工所在部门编号

};