#include <iostream>
using namespace std;
#include "workerManager.h"
#include "work.h"
#include "bose.h"
#include "emloyee.h"
#include "manager.h"

#define CeShi 0

int main(){
    
    #if CeShi
    //测试代码
    Worker *work1 = new Emloyee(1,"张三",1);
    Worker *work2 = new Bose(1,"张三",1);
    Worker *work3 = new Manager(1,"张三",1);

    work2 ->showInfo();
    work3 ->showInfo();
    work1 ->showInfo();

    #endif
    //实例化管理者对象
    WorkManager work;

    int cmd = 0 ; 
    while(1)
    {
        
        work.Show_Menu();
        cout << "请输入命令:";
        cin >> cmd;
        getchar();

        switch (cmd){
            case 1://1、退出管理種序
            {
                work.exitSystem();
                return 0;
                break;
            }
            case 2://2、增加职工信息
                work.Add_Emp();
                break;
            case 3://3、显示职工信息
                work.show_Empy();
                break;    
            case 4://4、删除离职职工
                work.Del_Emp();
                break;
            case 5://5、修改职工信息
                work.Mod_Emp();
                break;
            case 6://6、查找职工信息
                break;
            case 7://7、按照编号排序
                work.Sort_Emp();
                break;
            case 8://8、清空所有文档
                work.Delet_File();
                break;
            default:
                break;
        }

    }

    //system("cls");
    return 0;
}