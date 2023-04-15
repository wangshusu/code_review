#pragma once//防止头文件被重复包含
#include <iostream>
#include <fstream>
using namespace std;
#include "work.h"
#include "bose.h"
#include "manager.h"
#include "emloyee.h"

#define FILENAME "2.empFile.txt"

class WorkManager{

public:

    //构造函数
    WorkManager();

    //显示帮助函数
    void Show_Menu();

    //退出函数
    void exitSystem();

    //增加职工信息
    void Add_Emp();

    //保存文件
    void Save();

    //获取记录的职工人数
    int get_EmpNum();

    //初始化员工
    void init_Emp();

    //显示职工信息
    void show_Empy();

    //删除职工函数
    void Del_Emp();

    //判断职工是否存在 存在返回职工位置 不存在返回-1
    int IsExist(int id);

    //修改员工参数
    void Mod_Emp();

    //查找员工
    void Find_EMP();

    //排序
    void Sort_Emp();

    //清空
    void Delet_File();

    //析构函数
    ~WorkManager();

    //标志文件是否为空
    bool m_fileIsEmpty;

    //记录文件中的人数个数
    int m_EmpNum;

    //员工数组的指针
    Worker ** m_EmpArry;

};

