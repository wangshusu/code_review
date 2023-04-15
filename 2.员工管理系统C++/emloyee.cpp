#include "emloyee.h"

Emloyee::Emloyee(int id , string name , int dep_id)
{
    this ->m_id = id;
    this ->m_DeptId = dep_id;
    this ->m_Name = name;
}

string Emloyee::getDeptName()
{
    return string("员工");
}

void Emloyee::showInfo()
{
    cout << "岗位编号:" << this ->m_id
    << "\t职工姓名:" << this ->m_Name
    << "\t岗位:" << this ->getDeptName()
    << "\t岗位职责:完成经理交代的任务" << endl;
}