#include "bose.h"

    //构造函数
Bose::Bose(int id , string name , int dep_id)
{
    this ->m_id = id;
    this ->m_Name = name;
    this ->m_DeptId = dep_id;
}

    //显示老板信息
void Bose::showInfo()
{
    cout << "岗位编号:" << this ->m_id
    << "\t职工姓名:" << this ->m_Name
    << "\t岗位:" << this ->getDeptName()
    << "\t岗位职责:给广大的劳苦大众下发任务" << endl;
}

    //获取老板职工表
string Bose::getDeptName()
{
    return string("老板");
}