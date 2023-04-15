#include "workerManager.h"

WorkManager::WorkManager()
{
    ifstream ifs;
    ifs.open(FILENAME,ios::in);

    //如果没有文件
    if( !ifs.is_open() )
    {
        cout << "文件不存在" << endl;

        this ->m_EmpArry = NULL;
        this ->m_EmpNum = 0;
        this->m_fileIsEmpty = true;
        
        ifs.close();
        return;
    }

    //文件存在 并且没有记录
    char ch;
    ifs >> ch;
    if( ifs.eof() )
    {
        cout << "文件存在但为空" << endl;

        this ->m_EmpArry = NULL;
        this ->m_EmpNum = 0;
        this->m_fileIsEmpty = true;
        
        ifs.close();
        return;
    }

    //有人就显示几个人
    else 
    {
        cout <<"文件存在里面有 " << this ->get_EmpNum() << " 个员工" << endl;

        this ->m_EmpNum = this ->get_EmpNum();
        this ->init_Emp();

        ifs.close();
        return;
    }
}

WorkManager::~WorkManager()
{
    if(this ->m_EmpArry)
    {
        this ->m_EmpNum = 0;
        delete[] this ->m_EmpArry;
        this ->m_EmpArry = NULL;
    }
}

void WorkManager::Show_Menu()
{
    cout << "*********" <<"**************" << "**********" <<endl;
    cout << "*********" <<"欢迎欢迎热烈欢迎" << "********" <<endl;
    cout << "*********" <<"1、退出管理種序" << "*********" <<endl;
    cout << "*********" <<"2、增加职工信息" << "*********" <<endl;
    cout << "*********" <<"3、显示职工信息" << "*********" <<endl;
    cout << "*********" <<"4、删除离职职工" << "*********" <<endl;
    cout << "*********" <<"5、修改职工信息" << "*********" <<endl;
    cout << "*********" <<"6、查找职工信息" << "*********" <<endl;
    cout << "*********" <<"7、按照编号排序" << "*********" <<endl;
    cout << "*********" <<"8、清空所有文档" << "*********" <<endl;
    cout << "*********" <<"**************" << "**********" <<endl;
    cout << endl;
}

void WorkManager::exitSystem()
{
    cout << "欢迎下次使用" << endl;
    //system("pause");
    //system("cls");
    //exit(0);
}

//添加员工
void WorkManager::Add_Emp()
{
    cout << "请输入添加的人数:";
    int people_num;
    cin >> people_num;

    if(people_num >0)
    {
        int oldSize = this ->m_EmpNum;//老空间的人数
        int newSize = this ->m_EmpNum + people_num;//新空间的人数
        
        //开辟空间
        Worker ** NewSpace = new Worker *[newSize];
        if(this ->m_EmpNum)//拷贝数据
        {
            for(int i= 0 ; i < this ->m_EmpNum ; i ++)
            {
                NewSpace[i] = this ->m_EmpArry[i];
            }
        }

        for(int i = 0 ; i < people_num ; i ++)
        {
            int id;//职工编号
            string name;//姓名
            int dept_id;//部门的选择

            cout << "请输入第 " << i+1  << " 个职工的编号:";
            cin >> id;
            cout << "请输入第 " << i+1 << " 个职工的姓名:";
            cin >> name;
            cout << "部门规则如下:" << endl;
            cout << "1.老板" << endl;
            cout << "2.经理" << endl;
            cout << "3.员工" << endl;
            cout << "请输入第 " << i+1 << " 个职工的部门:";
            cin >> dept_id;

            switch (dept_id)
            {
            case 1:
                NewSpace[oldSize + i] = new Bose(id,name,dept_id);
                break;
            case 2:
                NewSpace[oldSize + i] = new Manager(id,name,dept_id);
                break;
            case 3:
                NewSpace[oldSize + i] = new Emloyee(id,name,dept_id);
                break;
            
            default:
                break;
            }
            cout << "第 " << i+1 << " 位职工添加成功!!!" << endl;
            cout << endl;
        }
        if(this ->m_EmpArry)//如果原来就有这个空间
        {
            cout << "123" << endl;
            delete []this ->m_EmpArry;//销毁原有空间
        }
        
        this ->m_EmpArry = NewSpace;//新的空间
        this ->m_EmpNum = newSize;//新的长度
        this ->Save();

        cout <<"全部添加成功!!!" << endl;
        
    }
    else
    {
        cout << "输入有误,已退出当前操作" << endl;
    }

}

    //保存文件
void WorkManager::Save()
{
    ofstream ofs;
    ofs.open(FILENAME,ios::out | ios::trunc);

    for(int i = 0 ; i < this ->m_EmpNum ; i ++)
    {
        ofs << this ->m_EmpArry[i]->m_id << " "
            << this ->m_EmpArry[i]->m_Name << " "
            << this ->m_EmpArry[i]->m_DeptId << endl;
    }

    ofs.close();
}

int WorkManager::get_EmpNum()
{
    ifstream ifs;
    ifs.open(FILENAME,ios::in);

    int id;
    string name;
    int dept_id;

    int num = 0;

    while(ifs >> id && ifs >> name && ifs >> dept_id)
    {

        num += 1;
    }
    
    // ifs.seekg(0,ios::beg);
    // while(ifs >> id && ifs >> name && ifs >> dept_id)
    // {

    //     num += 1;
    // }
    

    ifs.close();
    return num;

}

//初始化员工
void WorkManager::init_Emp()
{
    ifstream ifs;
    ifs.open(FILENAME,ios::in);

    int id;
    string name;
    int dept_id;

    Worker ** NewSpace = new Worker *[this ->m_EmpNum];
    this ->m_EmpArry = NewSpace;

    int i = 0;
    while(ifs >> id && ifs >> name && ifs >> dept_id)
    {
        switch (dept_id)
        {
            case 1:
                NewSpace[i] = new Bose(id,name,dept_id);
                break;
            case 2:
                NewSpace[i] = new Manager(id,name,dept_id);
                break;
            case 3:
                NewSpace[i] = new Emloyee(id,name,dept_id);
                break;
            default:
                break;
            
        }
        i++;
    }
}

//显示职工信息
void WorkManager::show_Empy()
{
    if(!this ->m_EmpNum || !this ->m_EmpArry)
    {
        cout << "没有数据!" << endl;
    }
    for(int i = 0 ; i < this ->m_EmpNum ; i ++)
    {
        cout << this ->m_EmpArry[i] ->m_id << " "
             << this ->m_EmpArry[i] ->m_Name << " "
             << this ->m_EmpArry[i] ->m_DeptId
             << endl;
    }
}

//判断职工是否存在 存在返回职工位置 不存在返回-1
int WorkManager::IsExist(int id)
{
    int index = -1;
    if(!this ->m_EmpArry)
    { 
        cout << "文件不存在或者记录为空" << endl;
        return index;
    }
    
    for(int i = 0 ; i < this ->m_EmpNum ; i ++)
    {
        if(this ->m_EmpArry[i] ->m_id == id)
        {
            index = i;
            //cout << "cunzai" << endl;
            return index;
        }
    }
    //cout << "bucunzai" << endl;
    return index;
}

//删除职工函数
void WorkManager::Del_Emp()
{
    int num = -1;
    cout << "要删除哪位:";
    cin >> num;
    int del_num = this ->IsExist(num);

    //如果没找到
    if(del_num < 0)
    {
        cout << "未查询到此人!已退出操作!" << endl;
        return;
    }
    //如果找到了
    //现将这个数据跟最后一个交换 然后删除最后一个数据
    this ->m_EmpArry[del_num] = this ->m_EmpArry[this ->get_EmpNum() -1];
    this ->m_EmpNum--;
    this ->Save();
    cout << "删除成功!!!" << endl;
}

//修改员工参数
void WorkManager::Mod_Emp()
{
    if(!this ->m_EmpArry)
    { 
        cout << "文件不存在或者记录为空" << endl;
        return;
    }
    int num = -1;
    cout << "要修改哪位:";
    cin >> num;
    int mod_num = this ->IsExist(num);

    //如果没找到
    if(mod_num < 0)
    {
        cout << "未查询到此人!已退出操作!" << endl;
        return;
    }
    //如果找到了
    //现将这个数据跟最后一个交换 然后删除最后一个数据
    int m_id;
    int dept_id;

    cout << "修改 " << this ->m_EmpArry[mod_num] ->m_Name << " 的 ID 为:";
    cin >> m_id;
    this ->m_EmpArry[mod_num] ->m_id = m_id;
    cout << "修改 " << this ->m_EmpArry[mod_num] ->m_Name << " 的 dep_id 为:";
    cin >> dept_id;
    this ->m_EmpArry[mod_num] ->m_DeptId = dept_id;
    cout << "修改成功!!" << endl;

    this ->Save();

}

//修改员工参数
void WorkManager::Find_EMP()
{
    //我感觉太简单了就不写了
    //问题是 如何做到员工的 id 唯一
    //并且在 名字相同 的同时 把名字相同的都显示出来
}

//排序
void WorkManager::Sort_Emp()
{
    if(!this ->m_EmpArry)
    {
        cout << "文件不存在或者保存的数据为空" << endl;
        return;
    }

    cout << "选择要排序的方法:" << endl
         << "1.升序" << endl
         << "2.降序" << endl;

    int select_num = 1;
    cin >> select_num;
    while(select_num != 1 && select_num != 2)
    {
        cout << "输入有误重新输入" << endl;
        cin >> select_num;
    }

    if(select_num == 1)
    {
        for(int i = 0 ; i <= this ->m_EmpNum ; i ++)
        {
            int min = i;
            for(int j = i + 1 ; j < this ->m_EmpNum ; j ++)
            {
                if(this ->m_EmpArry[min] ->m_id > this ->m_EmpArry[j] ->m_id)
                {
                    min = j;
                }
            }
            if(min != i)//如果后面有比他大的 那么就交换
            {
                Worker * temp = this ->m_EmpArry[i];
                this ->m_EmpArry[i] = this ->m_EmpArry[min];
                this ->m_EmpArry[min] = temp;
            }
        }
    }
    else
    {

    }
    

    this ->Save();
    cout << "排序成功" << endl;
}

//清空
void WorkManager::Delet_File()
{
    ofstream ofs;
    ofs.open(FILENAME,ios::trunc);

    if(this ->m_EmpArry)
    {
        for(int i = 0 ; i < this ->m_EmpNum ; i ++)
        {
            if(!this ->m_EmpArry[i])
            {
                delete this ->m_EmpArry[i];
            }
        }
        this ->m_EmpNum = 0;
        delete []this ->m_EmpArry;
        this ->m_EmpArry = NULL;
        this ->m_fileIsEmpty = true;
    }

    ofs.close();
    cout << "清空成功!" << endl;
}

