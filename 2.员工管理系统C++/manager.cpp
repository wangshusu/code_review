#include "manager.h"

//���캯��
Manager::Manager(int id , string name , int dep_id)
{
    this ->m_id = id;
    this ->m_DeptId = dep_id;
    this ->m_Name = name;
}

    //��ʾ������Ϣ  ���麯��
void Manager::showInfo()
{
    cout << "��λ���:" << this ->m_id
    << "\tְ������:" << this ->m_Name
    << "\t��λ:" << this ->getDeptName()
    << "\t��λְ��:����ϰ彻��������" << endl;
}

    //��ȡ��λ����  ���麯��
string Manager::getDeptName()
{
    return string("����");
}