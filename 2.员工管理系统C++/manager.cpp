#include "manager.h"

//构造函数
Manager::Manager(int id , string name , int dep_id)
{
    this ->m_id = id;
    this ->m_DeptId = dep_id;
    this ->m_Name = name;
}

    //显示个人信息  纯虚函数
void Manager::showInfo()
{
    cout << "岗位编号:" << this ->m_id
    << "\t职工姓名:" << this ->m_Name
    << "\t岗位:" << this ->getDeptName()
    << "\t岗位职责:完成老板交代的任务" << endl;
}

    //获取岗位名称  纯虚函数
string Manager::getDeptName()
{
    return string("经理");
}