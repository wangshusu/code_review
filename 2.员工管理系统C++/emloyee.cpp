#include "emloyee.h"

Emloyee::Emloyee(int id , string name , int dep_id)
{
    this ->m_id = id;
    this ->m_DeptId = dep_id;
    this ->m_Name = name;
}

string Emloyee::getDeptName()
{
    return string("Ա��");
}

void Emloyee::showInfo()
{
    cout << "��λ���:" << this ->m_id
    << "\tְ������:" << this ->m_Name
    << "\t��λ:" << this ->getDeptName()
    << "\t��λְ��:��ɾ�����������" << endl;
}