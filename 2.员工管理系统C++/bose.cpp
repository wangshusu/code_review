#include "bose.h"

    //���캯��
Bose::Bose(int id , string name , int dep_id)
{
    this ->m_id = id;
    this ->m_Name = name;
    this ->m_DeptId = dep_id;
}

    //��ʾ�ϰ���Ϣ
void Bose::showInfo()
{
    cout << "��λ���:" << this ->m_id
    << "\tְ������:" << this ->m_Name
    << "\t��λ:" << this ->getDeptName()
    << "\t��λְ��:�������Ϳ�����·�����" << endl;
}

    //��ȡ�ϰ�ְ����
string Bose::getDeptName()
{
    return string("�ϰ�");
}